#include "picoRTOS-SMP.h"
#include "picoRTOS-SMP_port.h"

/* CHECK FOR OBVIOUS ERRORS */

#if CONFIG_DEFAULT_STACK_COUNT < ARCH_MIN_STACK_COUNT
# error Default stack is too small
#endif

#define SMP_CORE_ANY ((1u << CONFIG_CORE_COUNT) - 1u)

/* SMP SCHEDULER main structures */

typedef enum {
    TASK_STATE_DISABLED,
    TASK_STATE_READY,
    TASK_STATE_BUSY,
    TASK_STATE_DONE,
    TASK_STATE_SLEEP,
    TASK_STATE_COUNT
} picoRTOS_task_state_t;

/* picoRTOS internal faults
 * These are negative to distinguish them from user faults
 */
#define FNONE      0
#define FINVALID  -1
#define FDEADLOCK -2 /* task starvation */
#define FNULLPTR  -3
#define FSTACKOVF -4
#define FSEGFAULT -5 /* access violation */

struct picoRTOS_task_core {
    /* state machine */
    /*@temp@*/ picoRTOS_stack_t *sp;
    picoRTOS_task_state_t state;
    picoRTOS_tick_t tick;
    int fault;
    picoRTOS_mask_t core_mask;
    /* checks */
    /*@temp@*/ picoRTOS_stack_t *stack_bottom;
    /*@temp@*/ picoRTOS_stack_t *stack_top;
    size_t stack_count;
    /* statistics */
    struct {
        picoRTOS_cycles_t counter;
        picoRTOS_cycles_t watermark_lo;
        picoRTOS_cycles_t watermark_hi;
        size_t deadline_miss_count;
    } stat;
    /* shared priority support */
    picoRTOS_priority_t prio;
    picoRTOS_priority_t sub_prio;
    /* deadline */
    size_t deadline_miss_count;
};

/* round-robin support */
struct picoRTOS_task_sub {
    picoRTOS_priority_t count;
};

/* user-defined tasks + idle */
#define TASK_COUNT     (CONFIG_TASK_COUNT + CONFIG_CORE_COUNT)
#define TASK_IDLE_PRIO CONFIG_TASK_COUNT
#define TASK_IDLE_PID  CONFIG_TASK_COUNT
/* shortcut for current task */
#define TASK_CURRENT_CORE(x) (picoRTOS.task[picoRTOS.index[x]])
#define TASK_CURRENT()       TASK_CURRENT_CORE(arch_core())
#define TASK_BY_PID(x)       (picoRTOS.task[(x)])
/* shortcut for current sub */
#define SUB_BY_PRIO(x) (picoRTOS.sub[(x)])

/* cache alignment */
#define L1_CACHE_ALIGN_MASK(x, mask) (((x) + (mask)) & ~(mask))
/* compliant with clang-tidy-18 */
static void *L1_CACHE_ALIGN(/*@returned@*/ const char *ptr, int align)
{
    picoRTOS_uintptr_t uintptr = (picoRTOS_uintptr_t)ptr;
    picoRTOS_uintptr_t bias = L1_CACHE_ALIGN_MASK(uintptr, (align - 1)) - uintptr;

    return (void*)(ptr + bias);
}

#define F_RUNNING   (1 << 0)
#define F_POSTPONED (1 << 1)

struct picoRTOS_SMP_core {
    int flags;
    picoRTOS_pid_t index[CONFIG_CORE_COUNT];
    picoRTOS_tick_t tick;
    picoRTOS_pid_t pid_count;
    picoRTOS_core_t core_counter;
    struct picoRTOS_task_core task[TASK_COUNT];
    struct picoRTOS_task_sub sub[TASK_COUNT];
} __attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

/* main core component */
static struct picoRTOS_SMP_core picoRTOS;
/* stacks *SHOULD* be provided by arch startup.S */

static void task_core_init(/*@out@*/ struct picoRTOS_task_core *task)
{
    /* state machine */
    task->sp = NULL;
    task->state = TASK_STATE_DISABLED;
    task->tick = 0;
    task->fault = FNONE;
    task->core_mask = 0;
    /* checks */
    task->stack_bottom = NULL;
    task->stack_top = NULL;
    task->stack_count = 0;
    /* stats */
    task->stat.counter = (picoRTOS_cycles_t)-1;
    task->stat.watermark_lo = (picoRTOS_cycles_t)-1;
    task->stat.watermark_hi = (picoRTOS_cycles_t)0;
    task->stat.deadline_miss_count = 0;
    /* shared priority support */
    task->prio = (picoRTOS_priority_t)TASK_COUNT;
    task->sub_prio = (picoRTOS_priority_t)0;
    /* deadline */
    task->deadline_miss_count = 0;
}

static bool task_core_is_available(const struct picoRTOS_task_core *task,
                                   picoRTOS_mask_t core_mask)
{
    /* task is ready and it's its turn */
    return (task->core_mask & core_mask) != 0 &&
           task->state == TASK_STATE_READY &&
           ((picoRTOS_priority_t)picoRTOS.tick %
            SUB_BY_PRIO(task->prio).count) == task->sub_prio;
}

static void task_core_quickcpy(/*@out@*/ struct picoRTOS_task_core *dst,
                               const struct picoRTOS_task_core *src)
{
    /* state machine */
    dst->sp = src->sp;
    dst->state = src->state;
    dst->core_mask = src->core_mask;
    /* checks */
    dst->stack_bottom = src->stack_bottom;
    dst->stack_top = src->stack_top;
    dst->stack_count = src->stack_count;
    /* shared priorities */
    dst->prio = src->prio;
}

static void task_core_quickswap(struct picoRTOS_task_core *t1,
                                struct picoRTOS_task_core *t2)
{
    struct picoRTOS_task_core tmp;

    task_core_quickcpy(&tmp, t1);
    task_core_quickcpy(t1, t2);
    task_core_quickcpy(t2, &tmp);
}

static inline void task_core_stat_start(struct picoRTOS_task_core *task)
{
    task->stat.counter = arch_counter(ARCH_COUNTER_CURRENT, 0);
}

static void task_core_stat_finish(struct picoRTOS_task_core *task)
{
    task->stat.counter = arch_counter(ARCH_COUNTER_SINCE, task->stat.counter);

    /* watermak lo */
    if (task->stat.counter < task->stat.watermark_lo)
        task->stat.watermark_lo = task->stat.counter;
    /* watermark hi */
    if (task->stat.counter > task->stat.watermark_hi)
        task->stat.watermark_hi = task->stat.counter;
}

static void task_sub_init(/*@out@*/ struct picoRTOS_task_sub *sub)
{
    sub->count = (picoRTOS_priority_t)1;
}

static void task_append(picoRTOS_pid_t pid,
                        struct picoRTOS_task *task,
                        picoRTOS_priority_t prio,
                        picoRTOS_mask_t core_mask)
{
    picoRTOS_assert(pid < (picoRTOS_pid_t)TASK_COUNT, return );
    picoRTOS_assert(prio < (picoRTOS_priority_t)TASK_COUNT, return );
    picoRTOS_assert(TASK_BY_PID(pid).state == TASK_STATE_DISABLED, return );

    /* state machine */
    TASK_BY_PID(pid).state = TASK_STATE_READY;
    TASK_BY_PID(pid).sp = arch_prepare_stack(task->stack, task->stack_count,
                                             task->fn, task->priv);
    /* checks */
    TASK_BY_PID(pid).stack_bottom = task->stack;
    TASK_BY_PID(pid).stack_top = task->stack + task->stack_count;
    TASK_BY_PID(pid).stack_count = task->stack_count;
    /* shared priorities */
    TASK_BY_PID(pid).prio = prio;
    /* SMP */
    TASK_BY_PID(pid).core_mask = core_mask;
}

static void task_idle_init(void)
{
    /* IDLE */
    struct picoRTOS_task idle;
    size_t core = (size_t)CONFIG_CORE_COUNT;

    static struct { picoRTOS_stack_t array[ARCH_MIN_STACK_COUNT]; }
    stack[CONFIG_CORE_COUNT] UNPRIVILEGED_STACK;

    while (core-- != 0) {
        /* ensure proper stack alignment */
        picoRTOS_task_init(&idle, (picoRTOS_task_fn)arch_idle, NULL, stack[core].array,
                           (size_t)ARCH_MIN_STACK_COUNT);

        /* similar to picoRTOS_add_task, but without count limit */
        task_append((picoRTOS_pid_t)(TASK_IDLE_PID + (int)core), &idle,
                    (picoRTOS_priority_t)TASK_IDLE_PRIO,
                    (picoRTOS_mask_t)(1u << core));
    }
}

/* linker mandatory sections */
/*@external@*/ extern const void* __pdata_start__[];
/*@external@*/ extern const size_t __pdata_len__[];
/*@external@*/ extern const void* __udata_start__[];
/*@external@*/ extern const size_t __udata_len__[];
/*@external@*/ extern const void* __ptext_start__[];
/*@external@*/ extern const size_t __ptext_len__[];
/* FIXME: partition even more */
/*@external@*/ extern const void* __utext_start__[];
/*@external@*/ extern const size_t __utext_len__[];

void picoRTOS_init(void)
{
    /* MPU */
    arch_mpu_init();
    arch_mpu_add_region(PID_KERNEL, (void*)__pdata_start__, (size_t)__pdata_len__, MM_PRW);
    arch_mpu_add_region(PID_KERNEL, (void*)__udata_start__, (size_t)__udata_len__, MM_URW);
    arch_mpu_add_region(PID_KERNEL, (void*)__ptext_start__, (size_t)__ptext_len__, MM_PRX);
    /* TODO: improve code partitioning */
    arch_mpu_add_region(PID_KERNEL, (void*)__utext_start__, (size_t)__utext_len__, MM_URX);

    /* reset pids */
    picoRTOS.pid_count = 0;

    /* zero all tasks */
    size_t n = (size_t)TASK_COUNT;

    while (n-- != 0) {
        task_core_init(&TASK_BY_PID(n));
        task_sub_init(&SUB_BY_PRIO(n));
    }

    /* IDLE */
    task_idle_init();
    picoRTOS.tick = (picoRTOS_tick_t)-1; /* 1st tick will be 0 */
    /* all cores start in idle */
    for (n = (size_t)CONFIG_CORE_COUNT; n-- != 0;)
        picoRTOS.index[n] = (picoRTOS_pid_t)(TASK_IDLE_PID + (int)n);

    /* RTOS status */
    picoRTOS.core_counter = 0;
    picoRTOS.flags = 0;
}

/* to avoid static inline in picoRTOS.h, this is duplicated */
void picoRTOS_task_init(struct picoRTOS_task *task,
                        picoRTOS_task_fn fn, void *priv,
                        picoRTOS_stack_t *stack,
                        size_t stack_count)
{
#define STACK_COUNT_MASK ((ARCH_L1_DCACHE_LINESIZE / sizeof(picoRTOS_stack_t)) - 1)
    picoRTOS_assert(stack_count >= (size_t)ARCH_MIN_STACK_COUNT, return );

    task->fn = fn;
    task->priv = priv;
    /* ensure page cache alignment */
    task->stack = (picoRTOS_stack_t*)L1_CACHE_ALIGN((char*)stack, ARCH_L1_DCACHE_LINESIZE);
    task->stack_count = (size_t)((stack + stack_count) - task->stack) & ~STACK_COUNT_MASK;
}

void picoRTOS_add_task(struct picoRTOS_task *task, picoRTOS_priority_t prio)
{
    /* check params */
    picoRTOS_assert(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT, return );
    picoRTOS_assert(picoRTOS.pid_count < (picoRTOS_pid_t)CONFIG_TASK_COUNT, return );
    task_append(picoRTOS.pid_count++, task, prio, (picoRTOS_mask_t)SMP_CORE_ANY);
}

picoRTOS_priority_t picoRTOS_get_next_available_priority(void)
{
    picoRTOS_pid_t pid = (picoRTOS_pid_t)0;
    picoRTOS_priority_t prio = (picoRTOS_priority_t)0;

    for (; pid < (picoRTOS_pid_t)CONFIG_TASK_COUNT; pid++)
        if (TASK_BY_PID(pid).prio == prio) {
            pid = (picoRTOS_pid_t)0; /* "recursive" */
            prio++;
        }

    /* no slot available */
    picoRTOS_assert(prio < (picoRTOS_priority_t)TASK_IDLE_PRIO,
                    return (picoRTOS_priority_t)-1);

    return prio;
}

picoRTOS_priority_t picoRTOS_get_last_available_priority(void)
{
    picoRTOS_pid_t pid = (picoRTOS_pid_t)0;
    picoRTOS_priority_t prio = (picoRTOS_priority_t)(TASK_IDLE_PRIO - 1);

    for (; pid < (picoRTOS_pid_t)CONFIG_TASK_COUNT; pid++)
        if (TASK_BY_PID(pid).prio == prio) {
            pid = (picoRTOS_pid_t)0; /* "recursive" */
            prio--;
        }

    /* no slot available: overflow */
    picoRTOS_assert(prio < (picoRTOS_priority_t)TASK_IDLE_PRIO,
                    return (picoRTOS_priority_t)-1);

    return prio;
}

/* Group: picoRTOS-SMP extensions */

/* Function: picoRTOS_SMP_add_task
 * Add a task to picoRTOS-SMP & assign it to specific core(s)
 *
 * Parameters:
 *  task - A pointer to already initialised task structure
 *  prio - The priority/identitifer of the task (MUST be < CONFIG_TASK_COUNT)
 *  core_mask - The cores to run the taks on in the form of a mask, where
 *  (1 << 0) is core 0, (1 << 1) is core 1, etc
 *
 * Example:
 * (start code)
 * picoRTOS_SMP_add_task(&task, (picoRTOS_priority_t)TASK_PRIO, (picoRTOS_mask_t)(1 << 1));
 * (end)
 *
 * Remarks:
 * picoRTOS-SMP will throw a debug exception and stall if the mask is 0, doesn't match the
 * exact number of cores, if prio is > CONFIG_TASK_COUNT or is not assigned yet
 */
void picoRTOS_SMP_add_task(struct picoRTOS_task *task,
                           picoRTOS_priority_t prio,
                           picoRTOS_mask_t core_mask)
{
    picoRTOS_assert(core_mask != 0, return );
    picoRTOS_assert(core_mask < (picoRTOS_mask_t)(1 << CONFIG_CORE_COUNT), return );
    picoRTOS_assert(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT, return );
    picoRTOS_assert(picoRTOS.pid_count < (picoRTOS_pid_t)CONFIG_TASK_COUNT, return );
    task_append(picoRTOS.pid_count++, task, prio, core_mask);
}

static void core_sort_tasks(void)
{
    /* selection sort (simple & in-place) */
    picoRTOS_pid_t i = (picoRTOS_pid_t)1;

    for (; i < (picoRTOS_pid_t)CONFIG_TASK_COUNT; i++) {

        picoRTOS_pid_t j = i;

        while (j > 0 && TASK_BY_PID(j - 1).prio > TASK_BY_PID(j).prio) {
            task_core_quickswap(&TASK_BY_PID(j), &TASK_BY_PID(j - 1));
            j--;
        }
    }
}

static void core_arrange_shared_priorities(void)
{
    picoRTOS_pid_t pid;
    picoRTOS_priority_t sub_prio = (picoRTOS_priority_t)0;

    /* count subs */
    for (pid = (picoRTOS_pid_t)1;
         pid < (picoRTOS_pid_t)CONFIG_TASK_COUNT; pid++) {
        /* get current prio */
        picoRTOS_priority_t prio = TASK_BY_PID(pid).prio;
        /* increment sub priority & count */
        if (prio == TASK_BY_PID(pid - 1).prio) {
            TASK_BY_PID(pid).sub_prio = ++sub_prio;
            SUB_BY_PRIO(prio).count = sub_prio + 1;
        }else
            sub_prio = (picoRTOS_priority_t)0;
    }
}

void picoRTOS_start(void)
{
#define CORE_MAX (CONFIG_CORE_COUNT - 1)
    picoRTOS_pid_t pid = (picoRTOS_pid_t)TASK_COUNT;
    picoRTOS_core_t core = (picoRTOS_core_t)CONFIG_CORE_COUNT;

    core_sort_tasks();
    core_arrange_shared_priorities();

    /* mpu configuration */
    while (pid-- != 0) {
        if (TASK_BY_PID(pid).state == TASK_STATE_DISABLED)
            continue;
        /* new region from stack */
        arch_mpu_add_region((int)pid, TASK_BY_PID(pid).stack_bottom,
                            TASK_BY_PID(pid).stack_count * sizeof(picoRTOS_stack_t),
                            MM_URW);
    }

    arch_smp_init();
    picoRTOS.flags |= F_RUNNING;

    /* start auxiliary cores first */
    while (core-- != (picoRTOS_core_t)1)
        arch_core_init(core, TASK_BY_PID(TASK_IDLE_PID + core).sp);

    /* start scheduler on core #0 */
    arch_flush_dcache(&picoRTOS, sizeof(picoRTOS));
    arch_mpu_enable();
    arch_mpu_restore_regions(TASK_IDLE_PID); /* FIXME */
    arch_start_first_task(TASK_BY_PID(TASK_IDLE_PID).sp);
}

/* KERNEL PANIC */

/*@noreturn@*/ static void fatal(void)
{
    for (;;)
        arch_break();
}

/* SYSCALLS */

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_switch_context(struct picoRTOS_task_core *task)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;
    picoRTOS_core_t core = arch_core();
    picoRTOS_mask_t mask = (picoRTOS_mask_t)(1u << core);

    /* stats */
    task_core_stat_finish(task);

    /* mark non-sleeping task as done */
    if (task->state == TASK_STATE_BUSY)
        task->state = TASK_STATE_DONE;

    while (deadlock-- != 0) {
        /* choose next task to run */
        do {
            picoRTOS.index[core]++;
            picoRTOS_assert(picoRTOS.index[core] < (picoRTOS_pid_t)TASK_COUNT, fatal());
            /* ignore sleeping, empty tasks & out-of-round sub-tasks */
        } while (!task_core_is_available(&TASK_CURRENT_CORE(core), mask));

        /* refresh current task pointer */
        task = &TASK_CURRENT_CORE(core);
        arch_mpu_restore_regions((int)picoRTOS.index[core]);   /* mpu */

        /* postponed tasks management */
        if (picoRTOS.index[core] >= (picoRTOS_pid_t)TASK_IDLE_PID &&
            (picoRTOS.flags & F_POSTPONED) != 0) {
            /* reset flags & index */
            picoRTOS.flags &= ~F_POSTPONED;
            picoRTOS.index[core] = (picoRTOS_pid_t)-1;
        }else{
            /* next task or idle */
            task->state = TASK_STATE_BUSY;
            break;
        }
    }

    picoRTOS_assert(deadlock != -1, fatal());
    task_core_stat_start(task); /* stats */

    arch_flush_dcache(&picoRTOS, sizeof(picoRTOS));
    arch_invalidate_dcache(task->sp, (size_t)task->stack_top - (size_t)task->sp);
    arch_spin_unlock();

    return task;
}

static void tick_fault(struct picoRTOS_task_core *task, int fault)
{
    task->state = TASK_STATE_DISABLED;
    task->fault = fault;
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_kill(struct picoRTOS_task_core *task, int fault)
{
    tick_fault(task, fault);
    return syscall_switch_context(task);
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_sleep(struct picoRTOS_task_core *task, picoRTOS_tick_t delay)
{
    if (delay > 0) {
        task->tick = picoRTOS.tick + delay;
        task->state = TASK_STATE_SLEEP;
    }else
        picoRTOS.flags |= F_POSTPONED;

    return syscall_switch_context(task);
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_sleep_until(/*@returned@*/ struct picoRTOS_task_core *task,
                    struct syscall_sleep_until *sc)
{
    picoRTOS_tick_t elapsed = picoRTOS.tick - sc->ref;

    if (elapsed < sc->period) {
        task->tick = sc->ref + sc->period;
        sc->ref = task->tick;
        task->state = TASK_STATE_SLEEP;
        /* error management */
        task->deadline_miss_count = 0;
        return syscall_switch_context(task);
    }

    /* missed the clock: retry until deadlock */
    if (++task->deadline_miss_count > (size_t)CONFIG_DEADLOCK_COUNT) {
        picoRTOS_assert_void(false);            /* force break in debug */
        return syscall_kill(task, FDEADLOCK);   /* kill out-of-control task */
    }

    /* stats */
    task->stat.deadline_miss_count++;
    arch_spin_unlock();
    return task; /* don't switch */
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_run(/*@returned@*/ struct picoRTOS_task_core *task, const bool *run)
{
    if (*run) arch_resume();
    else arch_suspend();
    /* don't switch context */
    arch_spin_unlock();
    return task;
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_get_tick(/*@returned@*/ struct picoRTOS_task_core *task, picoRTOS_tick_t *tick)
{
    *tick = picoRTOS.tick;
    arch_spin_unlock();
    return task;
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_get_pid(/*@returned@*/ struct picoRTOS_task_core *task, picoRTOS_pid_t *pid)
{
    *pid = picoRTOS.index[arch_core()];
    arch_spin_unlock();
    return task;
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_cacheop(/*@returned@*/ struct picoRTOS_task_core *task,
                const struct syscall_cacheop *op)
{
    if (op->invalidate) arch_invalidate_dcache(op->addr, op->n);
    if (op->flush) arch_flush_dcache(op->addr, op->n);
    arch_spin_unlock();
    return task;
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_mpu(/*@returned@*/ struct picoRTOS_task_core *task,
            const struct syscall_mpu *mpu)
{
    arch_mpu_add_region((int)picoRTOS.index[arch_core()],
                        mpu->addr, mpu->n, mpu->mode);
    arch_spin_unlock();
    return task;
}

picoRTOS_stack_t *picoRTOS_syscall(picoRTOS_stack_t *sp, syscall_t syscall, void *priv)
{
    /* FIXME: LOCK BUT LET THESE SUB-ROUTINES UNLOCK:
     * - syscall_switch_context()
     * - syscall_sleep_until()
     * - syscall_run()
     * - syscall_get_tick()
     * - syscall_get_pid()
     * - syscall_cacheop()
     * - syscall_mpu()
     */
    arch_spin_lock();
    arch_invalidate_dcache(&picoRTOS, sizeof(picoRTOS));

    struct picoRTOS_task_core *task = &TASK_CURRENT();

    picoRTOS_assert((picoRTOS.flags & F_RUNNING) != 0, fatal());
    picoRTOS_assert(sp >= task->stack_bottom, return syscall_kill(task, FSTACKOVF)->sp);
    picoRTOS_assert(sp < task->stack_top, return syscall_kill(task, FSTACKOVF)->sp);
    picoRTOS_assert(priv != NULL, return syscall_kill(task, FNULLPTR)->sp);

    /* store current sp & flush */
    task->sp = sp;
    arch_flush_dcache(task->sp, (size_t)task->stack_top - (size_t)task->sp);

    switch (syscall) {
    /* OS-related syscalls */
    case SYSCALL_RUN: return syscall_run(task, (bool*)priv)->sp;
    case SYSCALL_GETTICK: return syscall_get_tick(task, (picoRTOS_tick_t*)priv)->sp;
    case SYSCALL_CACHEOP: return syscall_cacheop(task, (struct syscall_cacheop*)priv)->sp;
    /* task-related syscalls */
    case SYSCALL_SLEEP:  return syscall_sleep(task, *(picoRTOS_tick_t*)priv)->sp;
    case SYSCALL_SLEEP_UNTIL: return syscall_sleep_until(task, (struct syscall_sleep_until*)priv)->sp;
    case SYSCALL_GETPID: return syscall_get_pid(task, (picoRTOS_pid_t*)priv)->sp;
    case SYSCALL_MPU: return syscall_mpu(task, (struct syscall_mpu*)priv)->sp;
    case SYSCALL_KILL: return syscall_kill(task, *(int*)priv)->sp;
    case SYSCALL_SEGFAULT: return syscall_kill(task, FSEGFAULT)->sp;
    default: break;
    }

    /* unhandled syscall */
    picoRTOS_assert_void(false);
    return syscall_kill(task, FINVALID)->sp;
}

/* TICK */

picoRTOS_stack_t *picoRTOS_tick(picoRTOS_stack_t *sp)
{
    arch_spin_lock();
    arch_invalidate_dcache(&picoRTOS, sizeof(picoRTOS));

    picoRTOS_core_t core = arch_core();
    struct picoRTOS_task_core *task = &TASK_CURRENT_CORE(core);

    picoRTOS_assert(sp >= task->stack_bottom, tick_fault(task, FSTACKOVF));
    picoRTOS_assert(sp < task->stack_top, tick_fault(task, FSTACKOVF));

    /* store current sp & flush */
    task->sp = sp;
    arch_flush_dcache(task->sp, (size_t)task->stack_top - (size_t)task->sp);

    /* mask task as immediately ready */
    task->state = TASK_STATE_READY;
    task_core_stat_finish(task);

    /* 1st core to catch tick */
    if (picoRTOS.core_counter++ == 0) {
        picoRTOS.tick++;                /* advance tick */
        picoRTOS.flags &= ~F_POSTPONED; /* reset flags */
    }

    /* quick pass on sleeping tasks + idle */
    picoRTOS_pid_t pid = (picoRTOS_pid_t)TASK_COUNT;
    picoRTOS_mask_t mask = (picoRTOS_mask_t)(1u << core);

    while (pid-- != 0) {

        task = &TASK_BY_PID(pid);

        if (task->state == TASK_STATE_SLEEP &&
            task->tick == picoRTOS.tick)
            /* task is ready to rumble */
            task->state = TASK_STATE_READY;

        /* reset done tasks */
        if (task->state == TASK_STATE_DONE)
            task->state = TASK_STATE_READY;

        /* select highest priority ready task */
        if (task_core_is_available(task, mask))
            picoRTOS.index[core] = pid;

        /* reset task counter */
        task->stat.counter = (picoRTOS_cycles_t)0;
    }

    /* refresh current task pointer */
    task = &TASK_CURRENT();
    task->state = TASK_STATE_BUSY;
    arch_mpu_restore_regions((int)picoRTOS.index[core]);
    task_core_stat_start(task);

    /* last core to leave tick */
    if (picoRTOS.core_counter == (picoRTOS_core_t)CONFIG_CORE_COUNT)
        picoRTOS.core_counter = 0;

    arch_flush_dcache(&picoRTOS, sizeof(picoRTOS));
    arch_invalidate_dcache(task->sp, (size_t)task->stack_top - (size_t)task->sp);
    arch_spin_unlock();

    return task->sp;
}

void picoRTOS_register_interrupt(picoRTOS_irq_t irq,
                                 picoRTOS_isr_fn fn,
                                 void *priv)
{
    /* supervisor only (no syscall needed) */
    arch_register_interrupt(irq, fn, priv);
}

void picoRTOS_set_interrupt(picoRTOS_irq_t irq, bool active)
{
    /* supervisor only (no syscall needed) */
    if (active) arch_enable_interrupt(irq);
    else arch_disable_interrupt(irq);
}

/* Function: picoRTOS_SMP_set_interrupt
 * Enables/disables an interrupt on the selected cores
 *
 * Parameters:
 *  irq - the irq number to enable
 *  core_mask - The core mask in the form of (1 << 0) is core 0, (1 <<1) is core 1, etc
 *  active - true of false to enable/disable the irq
 */
void picoRTOS_SMP_set_interrupt(picoRTOS_irq_t irq,
                                picoRTOS_mask_t core_mask,
                                bool active)
{
    picoRTOS_assert(core_mask != 0, return );
    picoRTOS_assert(core_mask < (picoRTOS_mask_t)(1 << CONFIG_CORE_COUNT), return );

    if (active) arch_smp_enable_interrupt(irq, core_mask);
    else arch_smp_disable_interrupt(irq, core_mask);
}

#include "picoRTOS.h"
#include "picoRTOS_port.h"

/* CHECK FOR OBVIOUS ERRORS */

#if CONFIG_DEFAULT_STACK_COUNT < ARCH_MIN_STACK_COUNT
# error Default stack is too small
#endif

#if (ARCH_L1_DCACHE_LINESIZE & ((ARCH_L1_DCACHE_LINESIZE)-1)) != 0
# error L1 cache line is not a power of 2
#endif

/* SCHEDULER main structures */

typedef enum {
    TASK_STATE_DISABLED,
    TASK_STATE_READY,
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
#define TASK_IDLE_PRIO (TASK_COUNT - 1)
#define TASK_IDLE_PID  (TASK_COUNT - 1)
/* shortcut for current task */
#define TASK_CURRENT() (picoRTOS.task[picoRTOS.index])
#define TASK_BY_PID(x) (picoRTOS.task[(x)])
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

struct picoRTOS_core {
    int flags;
    picoRTOS_pid_t index;
    picoRTOS_tick_t tick;
    picoRTOS_pid_t pid_count;
    struct picoRTOS_task_core task[TASK_COUNT];
    struct picoRTOS_task_sub sub[TASK_COUNT];
} __attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

/* main core component */
static struct picoRTOS_core picoRTOS;
/* system stack is provided by arch startup.S */

static void task_core_init(/*@out@*/ struct picoRTOS_task_core *task)
{
    /* state machine */
    task->sp = NULL;
    task->state = TASK_STATE_DISABLED;
    task->tick = 0;
    task->fault = FNONE;
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

static bool task_core_is_available(const struct picoRTOS_task_core *task)
{
    /* task is ready and it's its turn */
    return task->state == TASK_STATE_READY &&
           ((picoRTOS_priority_t)picoRTOS.tick %
            SUB_BY_PRIO(task->prio).count) == task->sub_prio;
}

static void task_core_quickcpy(/*@out@*/ struct picoRTOS_task_core *dst,
                               const struct picoRTOS_task_core *src)
{
    /* state machine */
    dst->sp = src->sp;
    dst->state = src->state;
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

static int task_append(picoRTOS_pid_t pid,
                       struct picoRTOS_task *task,
                       picoRTOS_priority_t prio)
{
    picoRTOS_assert(pid < (picoRTOS_pid_t)TASK_COUNT, return -1);
    picoRTOS_assert(prio < (picoRTOS_priority_t)TASK_COUNT, return -1);
    picoRTOS_assert(TASK_BY_PID(pid).state == TASK_STATE_DISABLED, return -1);

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

    return 0;
}

static void task_idle_init(void)
{
    /* IDLE */
    struct picoRTOS_task idle;
    static picoRTOS_stack_t stack[ARCH_MIN_STACK_COUNT] UNPRIVILEGED_STACK;

    /* ensure proper stack alignment */
    picoRTOS_task_init(&idle, (picoRTOS_task_fn)arch_idle, NULL, stack,
                       (size_t)ARCH_MIN_STACK_COUNT);

    /* similar to picoRTOS_add_task, but without count limit */
    (void)task_append((picoRTOS_pid_t)TASK_IDLE_PID, &idle,
                      (picoRTOS_priority_t)TASK_IDLE_PRIO);
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

/**
 * void **picoRTOS_init**(void);
 * > Initializes picoRTOS
 * ### NOTES
 * > This is the first function you need to call to be able to run picoRTOS.
 * > It will:
 * > - partition the memory (on mpu-supported systems)
 * > - init the idle task
 */
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
    picoRTOS.index = (picoRTOS_pid_t)TASK_IDLE_PID; /* first task */
    picoRTOS.tick = (picoRTOS_tick_t)-1;            /* 1st tick will be 0 */

    /* RTOS status */
    picoRTOS.flags = 0;
}

/**
 * void **picoRTOS_task_init**(**struct picoRTOS_task** \*<ins>task</ins>,
 * **picoRTOS_task_fn** <ins>fn</ins>, **void** \*<ins>priv</ins>, **picoRTOS_stack_t** \*<ins>stack</ins>,
 * **size_t** <ins>stack_count</ins>);
 * > Initialises a task structure with all relevant information.
 * ### RETURN
 * > picoRTOS will assert stack_count >= ARCH_MIN_STACK_COUNT & return immediately, leaving
 * > *<ins>task</ins> untouched if it fails
 * ### NOTES
 * > Here's your typical use case:
 * ```c
 *     struct picoRTOS_task task;
 *     struct picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];
 *
 *     picoRTOS_task_init(&task, task_entry_point, &task_context, stack, (size_t)CONFIG_DEFAULT_STACK_COUNT);
 * ```
 * > Remark: you can use the helper `PICORTOS_STACK_COUNT(stack)` to make your life easier
 */
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

/**
 * void **picoRTOS_add_task**(**struct picoRTOS_task** \*<ins>task</ins>,
 * **picoRTOS_priority_t** <ins>prio</ins>);
 * > Adds a <ins>task</ins> to picoRTOS & sets its priority <ins>prio</ins>.
 * ### NOTES
 * > Priorities goes from 0 (highest) to CONFIG_TASK_COUNT (lowest).<br>
 * > picoRTOS will assert prio < CONFIG_TASK_COUNT & return without adding
 * > the task to the system if it fails
 */
void picoRTOS_add_task(struct picoRTOS_task *task, picoRTOS_priority_t prio)
{
    picoRTOS_assert(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT, return );
    picoRTOS_assert(picoRTOS.pid_count < (picoRTOS_pid_t)CONFIG_TASK_COUNT, return );
    (void)task_append(picoRTOS.pid_count++, task, prio);
}

/**
 * picoRTOS_priority_t **picoRTOS_get_next_available_priority**(void);
 * > Gets the first available priority by ascending order (0 -> n)
 * ### NOTES
 * > This call will provide the next unused available priority & can be helpful to
 * > avoid managing priorities by hand & create potential involuntary round-robin
 * > situations. Example:
 * ```c
 *     picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
 * ```
 * > Round-robin can still be achieved this way:
 * ```c
 *    picoRTOS_priority_t prio = picoRTOS_get_next_available_priority();
 *    picoRTOS_add_task(&task1, prio);
 *    picoRTOS_add_task(&task2, prio);
 * ```
 * ### RETURN
 * > picoRTOS will return priority -1 if no more priorities are available
 */
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

/**
 * picoRTOS_priority_t **picoRTOS_get_last_available_priority**(void);
 * > Gets the first available priority by descending order (n -> 0)
 * ### NOTES
 * > This call will provide the next unused available priority & can be helpful to
 * > avoid managing priorities by hand & create potential involuntary round-robin
 * > situations. Example:
 * ```c
 *     picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());
 * ```
 * > Round-robin can still be achieved this way:
 * ```c
 *    picoRTOS_priority_t prio = picoRTOS_get_last_available_priority();
 *    picoRTOS_add_task(&task1, prio);
 *    picoRTOS_add_task(&task2, prio);
 * ```
 * > This call is usually used for low-priority refreshing tasks like watchdog
 * > management
 * ### RETURN
 * > picoRTOS will return priority -1 if no more priorities are available
 */
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

/**
 * void **picoRTOS_start**(void);
 * > Starts picoRTOS' scheduler
 * ### NOTES
 * > On memory-portected systems, call to unprivileged functions if forbidden
 * > after this one.
 */
void picoRTOS_start(void)
{
    picoRTOS_pid_t pid = (picoRTOS_pid_t)TASK_COUNT;

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

    arch_init();
    picoRTOS.flags |= F_RUNNING;

    arch_mpu_enable();
    arch_mpu_restore_regions(TASK_IDLE_PID);
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

    /* stats */
    task_core_stat_finish(task);

    while (deadlock-- != 0) {
        /* choose next task to run */
        do {
            picoRTOS.index++;
            picoRTOS_assert(picoRTOS.index < (picoRTOS_pid_t)TASK_COUNT, fatal());
            /* ignore sleeping, empty tasks & out-of-round sub-tasks */
        } while (!task_core_is_available(&TASK_CURRENT()));

        /* refresh current task pointer */
        task = &TASK_CURRENT();
        arch_mpu_restore_regions((int)picoRTOS.index);   /* mpu */

        /* postponed tasks management */
        if (picoRTOS.index == (picoRTOS_pid_t)TASK_IDLE_PID &&
            (picoRTOS.flags & F_POSTPONED) != 0) {
            /* reset flags & index */
            picoRTOS.flags &= ~F_POSTPONED;
            picoRTOS.index = (picoRTOS_pid_t)-1;
        }else
            /* next task or idle */
            break;
    }

    picoRTOS_assert(deadlock != -1, fatal());
    task_core_stat_start(task); /* stats */
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
    return task; /* don't switch */
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_run(/*@returned@*/ struct picoRTOS_task_core *task, const bool *run)
{
    if (*run) arch_resume();
    else arch_suspend();
    /* don't switch context */
    return task;
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_get_tick(/*@returned@*/ struct picoRTOS_task_core *task, picoRTOS_tick_t *tick)
{
    *tick = picoRTOS.tick;
    return task;
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_get_pid(/*@returned@*/ struct picoRTOS_task_core *task, picoRTOS_pid_t *pid)
{
    *pid = picoRTOS.index;
    return task;
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_cacheop(/*@returned@*/ struct picoRTOS_task_core *task,
                const struct syscall_cacheop *op)
{
    if (op->invalidate) arch_invalidate_dcache(op->addr, op->n);
    if (op->flush) arch_flush_dcache(op->addr, op->n);
    return task;
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_irqop(/*@returned@*/ struct picoRTOS_task_core *task,
              const struct syscall_irqop *op)
{
    if (op->enable) arch_enable_interrupt(op->irq);
    else arch_disable_interrupt(op->irq);
    return task;
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_mpu(/*@returned@*/ struct picoRTOS_task_core *task,
            const struct syscall_mpu *mpu)
{
    arch_mpu_add_region((int)picoRTOS.index, mpu->addr, mpu->n, mpu->mode);
    return task;
}

picoRTOS_stack_t *picoRTOS_syscall(picoRTOS_stack_t *sp, syscall_t syscall, void *priv)
{
    struct picoRTOS_task_core *task = &TASK_CURRENT();

    picoRTOS_assert((picoRTOS.flags & F_RUNNING) != 0, fatal());
    picoRTOS_assert(sp >= task->stack_bottom, return syscall_kill(task, FSTACKOVF)->sp);
    picoRTOS_assert(sp < task->stack_top, return syscall_kill(task, FSTACKOVF)->sp);
    picoRTOS_assert(priv != NULL, return syscall_kill(task, FNULLPTR)->sp);

    /* store current sp */
    task->sp = sp;

    switch (syscall) {
    /* task-related syscalls */
    case SYSCALL_SEGFAULT: return syscall_kill(task, FSEGFAULT)->sp;
    case SYSCALL_SLEEP:  return syscall_sleep(task, *(picoRTOS_tick_t*)priv)->sp;
    case SYSCALL_SLEEP_UNTIL: return syscall_sleep_until(task, (struct syscall_sleep_until*)priv)->sp;
    case SYSCALL_GETPID: return syscall_get_pid(task, (picoRTOS_pid_t*)priv)->sp;
    case SYSCALL_MPU: return syscall_mpu(task, (struct syscall_mpu*)priv)->sp;
    case SYSCALL_KILL: return syscall_kill(task, *(int*)priv)->sp;
    /* OS-related syscalls */
    case SYSCALL_RUN: return syscall_run(task, (bool*)priv)->sp;
    case SYSCALL_GETTICK: return syscall_get_tick(task, (picoRTOS_tick_t*)priv)->sp;
    case SYSCALL_CACHEOP: return syscall_cacheop(task, (struct syscall_cacheop*)priv)->sp;
    case SYSCALL_IRQOP: return syscall_irqop(task, (struct syscall_irqop*)priv)->sp;
    default: break;
    }

    /* unhandled syscall */
    picoRTOS_assert_void(false);
    return syscall_kill(task, FINVALID)->sp;
}

/* TICK */

picoRTOS_stack_t *picoRTOS_tick(picoRTOS_stack_t *sp)
{
    struct picoRTOS_task_core *task = &TASK_CURRENT();

    picoRTOS_assert(sp >= task->stack_bottom, tick_fault(task, FSTACKOVF));
    picoRTOS_assert(sp < task->stack_top, tick_fault(task, FSTACKOVF));

    /* stats */
    task_core_stat_finish(task);

    /* store current sp */
    task->sp = sp;

    /* advance tick & reset flags */
    picoRTOS.tick++;
    picoRTOS.flags &= ~F_POSTPONED;

    /* quick pass on sleeping tasks + idle */
    picoRTOS_pid_t pid = (picoRTOS_pid_t)TASK_COUNT;

    while (pid-- != 0) {

        task = &TASK_BY_PID(pid);

        if (task->state == TASK_STATE_SLEEP &&
            task->tick == picoRTOS.tick)
            /* task is ready to rumble */
            task->state = TASK_STATE_READY;

        /* select highest priority ready task */
        if (task_core_is_available(task))
            picoRTOS.index = pid;

        /* reset task counter */
        task->stat.counter = (picoRTOS_cycles_t)0;
    }

    /* refresh current task pointer */
    task = &TASK_CURRENT();
    arch_mpu_restore_regions((int)picoRTOS.index);

    /* stats */
    task_core_stat_start(task);

    return task->sp;
}

/**
 * void **picoRTOS_register_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>,
 * **picoRTOS_isr_fn** <ins>fn</ins>, **void** \*<ins>priv</ins>);
 * > Registers an <ins>irq</ins> handler.<br>
 * > On <ins>irq</ins>, the handler <ins>fn</ins> will be called with the param
 * > <ins>priv</ins>
 */
void picoRTOS_register_interrupt(picoRTOS_irq_t irq,
                                 picoRTOS_isr_fn fn,
                                 void *priv)
{
    /* supervisor only (no syscall needed) */
    arch_register_interrupt(irq, fn, priv);
}

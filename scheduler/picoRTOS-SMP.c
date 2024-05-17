#include "picoRTOS-SMP.h"
#include "picoRTOS-SMP_port.h"

/* CHECK FOR OBVIOUS ERRORS */

#ifndef CONFIG_SMP_CORES
# error SMP cores not configured
#endif

#if CONFIG_DEFAULT_STACK_COUNT < ARCH_MIN_STACK_COUNT
# error Default stack is too small
#endif

#define PICORTOS_SMP_CORE_ANY ((1u << CONFIG_SMP_CORES) - 1u)

/* SMP SCHEDULER main structures */

typedef enum {
    PICORTOS_TASK_STATE_EMPTY,
    PICORTOS_TASK_STATE_READY,
    PICORTOS_TASK_STATE_BUSY,
    PICORTOS_TASK_STATE_DONE,
    PICORTOS_TASK_STATE_SLEEP
} picoRTOS_task_state_t;

struct picoRTOS_task_core {
    /* state machine */
    /*@temp@*/ picoRTOS_stack_t *sp;
    picoRTOS_task_state_t state;
    picoRTOS_tick_t tick;
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
    picoRTOS_priority_t tick;
    picoRTOS_priority_t count;
};

/* user-defined tasks + idle */
#define TASK_COUNT     (CONFIG_TASK_COUNT + CONFIG_SMP_CORES)
#define TASK_IDLE_PRIO CONFIG_TASK_COUNT
#define TASK_IDLE_PID  CONFIG_TASK_COUNT
/* shortcut for current task */
#define TASK_CURRENT()       (picoRTOS.task[picoRTOS.index[arch_core()]])
#define TASK_CURRENT_CORE(x) (picoRTOS.task[picoRTOS.index[x]])
#define TASK_BY_PID(x)       (picoRTOS.task[(x)])
/* shortcut for current sub */
#define SUB_BY_PRIO(x) (picoRTOS.sub[(x)])

/* cache alignment */
#define L1_CACHE_ALIGN(x, a)         L1_CACHE_ALIGN_MASK((x), ((a) - 1))
#define L1_CACHE_ALIGN_MASK(x, mask) (((x) + (mask)) & ~(mask))

struct picoRTOS_SMP_core {
    bool is_running;
    picoRTOS_pid_t index[CONFIG_SMP_CORES];
    picoRTOS_tick_t tick;
    picoRTOS_pid_t pid_count;
    picoRTOS_core_t main_core;
    struct picoRTOS_task_core task[TASK_COUNT];
    struct picoRTOS_task_sub sub[TASK_COUNT];
} __ATTRIBUTE_ALIGNED__(ARCH_L1_DCACHE_LINESIZE);

struct picoRTOS_SMP_stack {
    /* core 0 already has a stack, this is a waste of memory
     * but we keep this empty space for simplicity */
    picoRTOS_stack_t core[ARCH_SMP_MIN_STACK_COUNT];
    picoRTOS_stack_t idle[ARCH_MIN_STACK_COUNT];
};

/* main core component */
static struct picoRTOS_SMP_core picoRTOS;
static struct picoRTOS_SMP_stack picoRTOS_SMP_stack[CONFIG_SMP_CORES];

static void task_core_init(/*@out@*/ struct picoRTOS_task_core *task)
{
    /* state machine */
    task->sp = NULL;
    task->state = PICORTOS_TASK_STATE_EMPTY;
    task->tick = 0;
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

static bool task_core_is_available(struct picoRTOS_task_core *task,
                                   picoRTOS_mask_t core_mask)
{
    /* task is ready and it's its turn */
    return (task->core_mask & core_mask) != 0 &&
           task->state == PICORTOS_TASK_STATE_READY &&
           SUB_BY_PRIO(task->prio).tick == task->sub_prio;
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
    sub->tick = 0;
    sub->count = (picoRTOS_priority_t)1;
}

static void task_sub_inc(struct picoRTOS_task_sub *sub)
{
    if (++sub->tick == sub->count)
        sub->tick = 0;
}

/* SCHEDULER functions */

static void task_idle_init(void)
{
    size_t i;

    for (i = 0; i < (size_t)CONFIG_SMP_CORES; i++) {

        static struct picoRTOS_task idle;
        picoRTOS_pid_t pid = (picoRTOS_pid_t)(TASK_IDLE_PID + (int)i);

        /* ensure proper stack alignment */
        picoRTOS_task_init(&idle, arch_idle, NULL, picoRTOS_SMP_stack[i].idle,
                           (size_t)ARCH_MIN_STACK_COUNT);

        /* similar to picoRTOS_add_task, but without count limit */
        TASK_BY_PID(pid).sp = arch_prepare_stack(idle.stack, idle.stack_count,
                                                 idle.fn, idle.priv);
        /* smp */
        TASK_BY_PID(pid).state = PICORTOS_TASK_STATE_READY;
        TASK_BY_PID(pid).core_mask = (picoRTOS_mask_t)(1u << i);
        /* checks */
        TASK_BY_PID(pid).stack_bottom = idle.stack;
        TASK_BY_PID(pid).stack_top = idle.stack + idle.stack_count;
        TASK_BY_PID(pid).stack_count = idle.stack_count;
        /* shared priorities, ignored by sort anyway */
        TASK_BY_PID(pid).prio = (picoRTOS_priority_t)TASK_IDLE_PRIO;
    }
}

/* to avoid static inline in picoRTOS.h, this is duplicated */
void picoRTOS_task_init(struct picoRTOS_task *task,
                        picoRTOS_task_fn fn, void *priv,
                        picoRTOS_stack_t *stack,
                        size_t stack_count)
{
#define STACK_COUNT_MASK ((ARCH_L1_DCACHE_LINESIZE / sizeof(picoRTOS_stack_t)) - 1)

    picoRTOS_assert_fatal(stack_count >= (size_t)ARCH_MIN_STACK_COUNT, return );

    task->fn = fn;
    task->priv = priv;
    /* ensure page cache alignment */
    task->stack = (picoRTOS_stack_t*)L1_CACHE_ALIGN((picoRTOS_intptr_t)stack, ARCH_L1_DCACHE_LINESIZE);
    task->stack_count = (size_t)((stack + stack_count) - task->stack) & ~STACK_COUNT_MASK;
}

void picoRTOS_init(void)
{
    /* reset pids */
    picoRTOS.pid_count = 0;

    /* zero all tasks with no memset */
    size_t n = (size_t)TASK_COUNT;

    while (n-- != 0) {
        task_core_init(&TASK_BY_PID(n));
        task_sub_init(&SUB_BY_PRIO(n));
    }

    /* IDLE */
    task_idle_init();

    picoRTOS.tick = (picoRTOS_tick_t)-1; /* 1st tick will be 0 */
    n = (size_t)CONFIG_SMP_CORES;

    /* all cores start in idle */
    while (n-- != 0)
        picoRTOS.index[n] = (picoRTOS_pid_t)(TASK_IDLE_PID + (int)n);

    /* set main core */
    picoRTOS.main_core = arch_core();

    /* RTOS status */
    picoRTOS.is_running = false;
}

void picoRTOS_add_task(struct picoRTOS_task *task, picoRTOS_priority_t prio)
{
    /* check params */
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT,
                          return );

    picoRTOS_SMP_add_task(task, prio, (picoRTOS_mask_t)PICORTOS_SMP_CORE_ANY);
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
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)TASK_IDLE_PRIO,
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
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)TASK_IDLE_PRIO,
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
    picoRTOS_assert_fatal(core_mask != 0, return );
    picoRTOS_assert_fatal(core_mask < (picoRTOS_mask_t)(1 << CONFIG_SMP_CORES), return );
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT, return );

    picoRTOS_pid_t pid = picoRTOS.pid_count;

    picoRTOS_assert_fatal(pid < (picoRTOS_pid_t)CONFIG_TASK_COUNT, return );
    picoRTOS_assert_fatal(TASK_BY_PID(pid).state == PICORTOS_TASK_STATE_EMPTY, return );

    /* state machine */
    TASK_BY_PID(pid).sp = arch_prepare_stack(task->stack, task->stack_count,
                                             task->fn, task->priv);
    /* smp */
    TASK_BY_PID(pid).state = PICORTOS_TASK_STATE_READY;
    TASK_BY_PID(pid).core_mask = core_mask;
    /* checks */
    TASK_BY_PID(pid).stack_bottom = task->stack;
    TASK_BY_PID(pid).stack_top = task->stack + task->stack_count;
    TASK_BY_PID(pid).stack_count = task->stack_count;
    /* shared priorities */
    TASK_BY_PID(pid).prio = prio;

    /* increment */
    picoRTOS.pid_count++;
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
    core_sort_tasks();
    core_arrange_shared_priorities();

    arch_smp_init();
    picoRTOS.is_running = true;

    /* start auxiliary cores first */
    picoRTOS_core_t i;

    for (i = (picoRTOS_core_t)1;
         i < (picoRTOS_core_t)CONFIG_SMP_CORES; i++) {
        /* allocate a master stack & idle */
        arch_core_init(i, picoRTOS_SMP_stack[i].core, (size_t)ARCH_MIN_STACK_COUNT,
                       TASK_BY_PID(TASK_IDLE_PID + i).sp);
    }

    /* start scheduler on core #0 */
    picoRTOS_flush_dcache(&picoRTOS, sizeof(picoRTOS));
    arch_start_first_task(TASK_BY_PID(TASK_IDLE_PID).sp);
}

void picoRTOS_suspend(void)
{
    picoRTOS_assert_fatal(picoRTOS.is_running, return );

    arch_suspend();
    arch_spin_lock();
}

void picoRTOS_resume(void)
{
    picoRTOS_assert_fatal(picoRTOS.is_running, return );

    arch_spin_unlock();
    arch_resume();
}

void picoRTOS_schedule(void)
{
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_TASK_STATE_BUSY, return );
    arch_syscall(SYSCALL_SWITCH_CONTEXT, NULL);
}

void picoRTOS_sleep(picoRTOS_tick_t delay)
{
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_TASK_STATE_BUSY, return );
    arch_syscall(SYSCALL_SLEEP, (struct syscall_sleep*)&delay);
}

void picoRTOS_sleep_until(picoRTOS_tick_t *ref, picoRTOS_tick_t period)
{
    picoRTOS_assert_fatal(period > 0, return );
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_TASK_STATE_BUSY, return );

    struct syscall_sleep_until sc = { *ref, period };

    arch_syscall(SYSCALL_SLEEP_UNTIL, &sc);
    *ref = sc.ref; /* update ref */
}

void picoRTOS_kill(void)
{
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_TASK_STATE_BUSY, return );
    arch_syscall(SYSCALL_KILL, NULL);
}

picoRTOS_priority_t picoRTOS_self(void)
{
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_TASK_STATE_BUSY,
                          return (picoRTOS_pid_t)-1);

    return (picoRTOS_priority_t)picoRTOS.index[arch_core()];
}

picoRTOS_tick_t picoRTOS_get_tick(void)
{
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_TASK_STATE_BUSY,
                          return (picoRTOS_tick_t)-1);

    return picoRTOS.tick;
}

/* SYSCALLS */

static void syscall_sleep(struct picoRTOS_task_core *task, struct syscall_sleep *sc)
{
    if (sc->delay > 0) {
        task->tick = picoRTOS.tick + sc->delay;
        task->state = PICORTOS_TASK_STATE_SLEEP;
    }
}

static void syscall_sleep_until(struct picoRTOS_task_core *task, struct syscall_sleep_until *sc)
{
    picoRTOS_tick_t elapsed = picoRTOS.tick - sc->ref;

    if (elapsed < sc->period) {
        task->tick = sc->ref + sc->period;
        sc->ref = task->tick;
        task->state = PICORTOS_TASK_STATE_SLEEP;
        /* error management */
        task->deadline_miss_count = 0;
        return;
    }

    /* missed the clock: retry until deadlock */
    if (++task->deadline_miss_count > (size_t)CONFIG_DEADLOCK_COUNT) {
        picoRTOS_break();
        /*@notreached@*/ sc->ref = picoRTOS.tick;
    }

    /* stats */
    task->stat.deadline_miss_count++;
}

static void syscall_kill(struct picoRTOS_task_core *task)
{
    task->state = PICORTOS_TASK_STATE_EMPTY;
}

/*@exposed@*/
static struct picoRTOS_task_core *
syscall_switch_context(struct picoRTOS_task_core *task)
{
    picoRTOS_core_t core = arch_core();
    picoRTOS_mask_t mask = (picoRTOS_mask_t)(1u << core);

    /* stats */
    task_core_stat_finish(task);

    /* mark non-sleeping task as done */
    if (task->state == PICORTOS_TASK_STATE_BUSY)
        task->state = PICORTOS_TASK_STATE_DONE;

    do {
        picoRTOS.index[core]++;
        picoRTOS_assert_void_fatal(picoRTOS.index[core] < (picoRTOS_pid_t)TASK_COUNT);
        /* ignore sleeping, empty tasks & out-of-round sub-tasks */
    } while (!task_core_is_available(&TASK_CURRENT_CORE(core), mask));

    /* round-robin management */
    task_sub_inc(&SUB_BY_PRIO(task->prio));
    /* refresh current task pointer & state */
    task = &TASK_CURRENT_CORE(core);
    task->state = PICORTOS_TASK_STATE_BUSY;

    /* stats */
    task_core_stat_start(task);

    return task;
}

picoRTOS_stack_t *picoRTOS_syscall(picoRTOS_stack_t *sp, syscall_t syscall, void *priv)
{
    picoRTOS_assert_fatal(syscall < SYSCALL_COUNT, return NULL);

    arch_spin_lock();
    picoRTOS_invalidate_dcache(&picoRTOS, sizeof(picoRTOS));

    struct picoRTOS_task_core *task = &TASK_CURRENT();

    picoRTOS_assert_fatal(sp >= task->stack_bottom, return NULL);
    picoRTOS_assert_fatal(sp < task->stack_top, return NULL);

    /* store current sp & flush */
    task->sp = sp;
    picoRTOS_flush_dcache(task->sp, (size_t)task->stack_top - (size_t)task->sp);

    switch (syscall) {
    case SYSCALL_SLEEP:
        picoRTOS_assert_fatal(priv != NULL, return NULL);
        syscall_sleep(task, (struct syscall_sleep*)priv);
        break;

    case SYSCALL_SLEEP_UNTIL:
        picoRTOS_assert_fatal(priv != NULL, return NULL);
        syscall_sleep_until(task, (struct syscall_sleep_until*)priv);
        break;

    case SYSCALL_KILL:
        syscall_kill(task);
        break;

    default:
        /* SYSCALL_SWITCH_CONTEXT */
        break;
    }

    task = syscall_switch_context(task);

    picoRTOS_flush_dcache(&picoRTOS, sizeof(picoRTOS));
    picoRTOS_invalidate_dcache(task->sp, (size_t)task->stack_top - (size_t)task->sp);
    arch_spin_unlock();

    return task->sp;
}

/* TICK */

static picoRTOS_pid_t picoRTOS_rewind_as_main(picoRTOS_core_t core)
{
    picoRTOS_assert_fatal(core == picoRTOS.main_core,
                          return (picoRTOS_pid_t)(TASK_IDLE_PID + (int)core));

    /* aux cores will take care of their own idle */
    picoRTOS_pid_t pid = (picoRTOS_pid_t)(TASK_IDLE_PID + 1);
    picoRTOS_mask_t mask = (picoRTOS_mask_t)(1u << core);

    /* rewind tasks for all cores */
    while (pid-- != 0) {

        struct picoRTOS_task_core *task = &TASK_BY_PID(pid);

        if (task->state == PICORTOS_TASK_STATE_SLEEP &&
            task->tick == picoRTOS.tick)
            /* task is ready to rumble */
            task->state = PICORTOS_TASK_STATE_READY;

        /* reset done tasks */
        if (task->state == PICORTOS_TASK_STATE_DONE)
            task->state = PICORTOS_TASK_STATE_READY;

        /* select highest priority ready task */
        if (task_core_is_available(task, mask))
            picoRTOS.index[core] = pid;

        /* reset task counter */
        task->stat.counter = (picoRTOS_cycles_t)0;
    }

    return picoRTOS.index[core];
}

static picoRTOS_pid_t picoRTOS_rewind_as_aux(picoRTOS_core_t core)
{
    picoRTOS_assert_fatal(core != picoRTOS.main_core,
                          return (picoRTOS_pid_t)(TASK_IDLE_PID + (int)core));

    /* we can make a quick pass as an auxiliary core */
    picoRTOS_pid_t pid = (picoRTOS_pid_t)TASK_COUNT;
    picoRTOS_mask_t mask = (picoRTOS_mask_t)(1u << core);

    while (pid-- != 0) {
        /* select highest priority ready task, no state alteration */
        if (task_core_is_available(&TASK_BY_PID(pid), mask))
            picoRTOS.index[core] = pid;
    }

    return picoRTOS.index[core];
}

picoRTOS_stack_t *picoRTOS_tick(picoRTOS_stack_t *sp)
{
    picoRTOS_pid_t pid;
    picoRTOS_core_t core = arch_core();

    arch_spin_lock();
    picoRTOS_invalidate_dcache(&picoRTOS, sizeof(picoRTOS));

    struct picoRTOS_task_core *task = &TASK_CURRENT_CORE(core);

    picoRTOS_assert_fatal(sp >= task->stack_bottom, return NULL);
    picoRTOS_assert_fatal(sp < task->stack_top, return NULL);

    /* store current sp & flush */
    task->sp = sp;
    picoRTOS_flush_dcache(task->sp, (size_t)task->stack_top - (size_t)task->sp);

    /* mask task as immediately ready */
    task->state = PICORTOS_TASK_STATE_READY;
    task_core_stat_finish(task);

    /* round-robin management */
    task_sub_inc(&SUB_BY_PRIO(task->prio));

    /* advance tick & propagate to auxiliary cores */
    if (core == picoRTOS.main_core) {
        picoRTOS.tick++;
        arch_propagate_tick();
        pid = picoRTOS_rewind_as_main(core);
    }else{
        arch_acknowledge_tick();
        pid = picoRTOS_rewind_as_aux(core);
    }

    /* refresh current task pointer */
    task = &TASK_BY_PID(pid);
    task->state = PICORTOS_TASK_STATE_BUSY;
    task_core_stat_start(task);

    picoRTOS_flush_dcache(&picoRTOS, sizeof(picoRTOS));
    picoRTOS_invalidate_dcache(task->sp, (size_t)task->stack_top - (size_t)task->sp);
    arch_spin_unlock();

    return task->sp;
}

void picoRTOS_register_interrupt(picoRTOS_irq_t irq,
                                 picoRTOS_isr_fn fn,
                                 void *priv)
{
    arch_register_interrupt(irq, fn, priv);
}

void picoRTOS_enable_interrupt(picoRTOS_irq_t irq)
{
    arch_enable_interrupt(irq);
}

void picoRTOS_disable_interrupt(picoRTOS_irq_t irq)
{
    arch_disable_interrupt(irq);
}

/* Function: picoRTOS_SMP_enable_interrupt
 * Enables an interrupt on the selected cores
 *
 * Parameters:
 *  irq - the irq number to enable
 *  core_mask - The core mask in the form of (1 << 0) is core 0, (1 <<1) is core 1, etc
 */
void picoRTOS_SMP_enable_interrupt(picoRTOS_irq_t irq,
                                   picoRTOS_mask_t core_mask)
{
    picoRTOS_assert_fatal(core_mask != 0, return );
    picoRTOS_assert_fatal(core_mask < (picoRTOS_mask_t)(1 << CONFIG_SMP_CORES), return );

    arch_smp_enable_interrupt(irq, core_mask);
}

/* Function: picoRTOS_SMP_disable_interrupt
 * Disables an interrupt on the selected cores
 *
 * Parameters:
 *  irq - the irq number to disable
 *  core_mask - The core mask in the form of (1 << 0) is core 0, (1 <<1) is core 1, etc
 */
void picoRTOS_SMP_disable_interrupt(picoRTOS_irq_t irq,
                                    picoRTOS_mask_t core_mask)
{
    picoRTOS_assert_fatal(core_mask != 0, return );
    picoRTOS_assert_fatal(core_mask < (picoRTOS_mask_t)(1 << CONFIG_SMP_CORES), return );

    arch_smp_disable_interrupt(irq, core_mask);
}

void picoRTOS_invalidate_dcache(/*@unused@*/ void *addr, size_t n)
{
    picoRTOS_assert_void_fatal(n > 0);
    arch_invalidate_dcache(addr, n);
}

void picoRTOS_flush_dcache(/*@unused@*/ void *addr, size_t n)
{
    picoRTOS_assert_void_fatal(n > 0);
    arch_flush_dcache(addr, n);
}

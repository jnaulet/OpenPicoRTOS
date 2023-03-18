#include "picoRTOS-SMP.h"
#include "picoRTOS-SMP_port.h"

/* CHECK FOR OBVIOUS ERRORS */

#if CONFIG_DEFAULT_STACK_COUNT < ARCH_MIN_STACK_COUNT
# error Default stack is too small
#endif

#if PICORTOS_CYCLES_PER_TICK < 1
# error Erroneous timing values
#endif

/* SMP SCHEDULER main structures */

typedef enum {
    PICORTOS_SMP_TASK_STATE_EMPTY,
    PICORTOS_SMP_TASK_STATE_READY,
    PICORTOS_SMP_TASK_STATE_RUNNING,
    PICORTOS_SMP_TASK_STATE_SLEEP
} picoRTOS_SMP_task_state_t;

struct picoRTOS_SMP_task_core {
    /* state machine */
    /*@temp@*/ picoRTOS_stack_t *sp;
    picoRTOS_SMP_task_state_t state;
    picoRTOS_tick_t tick;
    picoRTOS_mask_t core;
    /* checks */
    /*@temp@*/ picoRTOS_stack_t *stack;
    size_t stack_count;
    /* statistics */
    struct {
        picoRTOS_cycles_t counter;
        picoRTOS_cycles_t watermark_lo;
        picoRTOS_cycles_t watermark_hi;
    } stat;
    /* shared priority support */
    picoRTOS_priority_t prio;
    picoRTOS_priority_t sub;
    size_t sub_count;
};

/* user-defined tasks + idle */
#define TASK_COUNT     (CONFIG_TASK_COUNT + CONFIG_SMP_CORES)
#define TASK_IDLE_PRIO CONFIG_TASK_COUNT
#define TASK_IDLE_PID  CONFIG_TASK_COUNT
/* shortcut for current task */
#define TASK_CURRENT()  (picoRTOS.task[picoRTOS.index[arch_core()]])
#define TASK_BY_PID(x) (picoRTOS.task[(x)])

struct picoRTOS_SMP_core {
    bool is_running;
    picoRTOS_pid_t index[CONFIG_SMP_CORES];
    picoRTOS_tick_t tick;
    picoRTOS_core_t main_core;
    struct picoRTOS_SMP_task_core task[TASK_COUNT];
};

struct picoRTOS_SMP_stack {
    /* core 0 already has a stack, this is a waste of memory
     * but we keep this empty space for simplicity */
    picoRTOS_stack_t core[ARCH_SMP_MIN_STACK_COUNT];
    picoRTOS_stack_t idle[ARCH_MIN_STACK_COUNT];
};

/* main core component */
static struct picoRTOS_SMP_core picoRTOS;
static struct picoRTOS_SMP_stack picoRTOS_SMP_stack[CONFIG_SMP_CORES];

static void task_core_init(/*@out@*/ struct picoRTOS_SMP_task_core *task)
{
    /* state machine */
    task->sp = NULL;
    task->state = PICORTOS_SMP_TASK_STATE_EMPTY;
    task->tick = 0;
    task->core = 0;
    /* checks */
    task->stack = NULL;
    task->stack_count = 0;
    /* stats */
    task->stat.counter = (picoRTOS_cycles_t)0;
    task->stat.watermark_lo = (picoRTOS_cycles_t)PICORTOS_CYCLES_PER_TICK;
    task->stat.watermark_hi = (picoRTOS_cycles_t)0;
    /* shared priority support */
    task->prio = (picoRTOS_priority_t)TASK_COUNT;
    task->sub = (picoRTOS_priority_t)0;
    task->sub_count = (size_t)1;
}

static bool task_core_is_available(struct picoRTOS_SMP_task_core *task,
                                   picoRTOS_mask_t core)
{
    /* task is ready and it's its turn */
    return (task->core & core) != 0 &&
           (task->state == PICORTOS_SMP_TASK_STATE_READY) &&
           ((size_t)picoRTOS.tick % task->sub_count) == (size_t)task->sub;
}

static void task_core_quickcpy(/*@out@*/ struct picoRTOS_SMP_task_core *dst,
                               const struct picoRTOS_SMP_task_core *src)
{
    /* state machine */
    dst->sp = src->sp;
    dst->state = src->state;
    dst->core = src->core;
    /* checks */
    dst->stack = src->stack;
    dst->stack_count = src->stack_count;
    /* shared priorities */
    dst->prio = src->prio;
}

static void task_core_quickswap(struct picoRTOS_SMP_task_core *t1,
                                struct picoRTOS_SMP_task_core *t2)
{
    struct picoRTOS_SMP_task_core tmp;

    task_core_quickcpy(&tmp, t1);
    task_core_quickcpy(t1, t2);
    task_core_quickcpy(t2, &tmp);
}

static inline void task_core_stat_begin(struct picoRTOS_SMP_task_core *task)
{
    task->stat.counter = arch_counter();
}

static void task_core_stat_watermark(struct picoRTOS_SMP_task_core *task)
{
    if (task->stat.counter < task->stat.watermark_lo)
        task->stat.watermark_lo = task->stat.counter;

    if (task->stat.counter > task->stat.watermark_hi)
        task->stat.watermark_hi = task->stat.counter;
}

static void task_core_stat_switch(struct picoRTOS_SMP_task_core *task)
{
    task->stat.counter = arch_counter() - task->stat.counter;
    task_core_stat_watermark(task);
}

static void task_core_stat_preempt(struct picoRTOS_SMP_task_core *task)
{
    task->stat.counter = (picoRTOS_cycles_t)PICORTOS_CYCLES_PER_TICK -
                         task->stat.counter;

    task_core_stat_watermark(task);
}

/* SCHEDULER functions */

static void task_idle_init_all(void)
{
    size_t i;

    for (i = 0; i < (size_t)CONFIG_SMP_CORES; i++) {

        struct picoRTOS_task idle;
        picoRTOS_pid_t pid = (picoRTOS_pid_t)(TASK_IDLE_PID + (int)i);

        picoRTOS_task_init(&idle, arch_idle, NULL, picoRTOS_SMP_stack[i].idle,
                           (size_t)ARCH_MIN_STACK_COUNT);

        /* similar to picoRTOS_add_task, but without count limit */
        TASK_BY_PID(pid).sp = arch_prepare_stack(&idle);
        TASK_BY_PID(pid).state = PICORTOS_SMP_TASK_STATE_READY;
        TASK_BY_PID(pid).core = (picoRTOS_mask_t)(1u << i);
        /* checks */
        TASK_BY_PID(pid).stack = picoRTOS_SMP_stack[i].idle;
        TASK_BY_PID(pid).stack_count = idle.stack_count;
        /* shared priorities, ignored by sort anyway */
        TASK_BY_PID(pid).prio = (picoRTOS_priority_t)TASK_IDLE_PRIO;
    }
}

/* to avoid static inline in picoRTOS.h, this is duplicated */
void picoRTOS_task_init(struct picoRTOS_task *task,
                        picoRTOS_task_fn_t fn, void *priv,
                        picoRTOS_stack_t *stack,
                        size_t stack_count)
{
    picoRTOS_assert_fatal(stack_count >= (size_t)ARCH_MIN_STACK_COUNT);

    task->fn = fn;
    task->stack = stack;
    task->stack_count = stack_count;
    task->priv = priv;
}

void picoRTOS_init(void)
{
    /* zero all tasks with no memset */
    size_t n = (size_t)TASK_COUNT;

    while (n-- != 0)
        task_core_init(&TASK_BY_PID(n));

    /* IDLE */
    task_idle_init_all();

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
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT);
    picoRTOS_assert_fatal(TASK_BY_PID(prio).state == PICORTOS_SMP_TASK_STATE_EMPTY);

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
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)TASK_IDLE_PRIO);

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
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)TASK_IDLE_PRIO);

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
    picoRTOS_assert_fatal(core_mask != 0);
    picoRTOS_assert_fatal(core_mask < (picoRTOS_mask_t)(1 << CONFIG_SMP_CORES));
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT);

    static picoRTOS_pid_t pid = (picoRTOS_pid_t)0;

    picoRTOS_assert_fatal(pid < (picoRTOS_pid_t)CONFIG_TASK_COUNT);
    picoRTOS_assert_fatal(TASK_BY_PID(pid).state == PICORTOS_SMP_TASK_STATE_EMPTY);

    /* state machine */
    TASK_BY_PID(pid).sp = arch_prepare_stack(task);
    TASK_BY_PID(pid).state = PICORTOS_SMP_TASK_STATE_READY;
    TASK_BY_PID(pid).core = core_mask;
    /* checks */
    TASK_BY_PID(pid).stack = task->stack;
    TASK_BY_PID(pid).stack_count = task->stack_count;
    /* shared priorities */
    TASK_BY_PID(pid).prio = prio;

    /* increment */
    pid++;
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
    picoRTOS_pid_t i = (picoRTOS_pid_t)1;
    picoRTOS_priority_t sub = (picoRTOS_priority_t)0;

    /* count subs */
    for (; i < (picoRTOS_pid_t)CONFIG_TASK_COUNT; i++) {
        /* increment sub priority & count */
        if (TASK_BY_PID(i).prio == TASK_BY_PID(i - 1).prio) {
            TASK_BY_PID(i).sub = ++sub;
            TASK_BY_PID(i).sub_count = (size_t)sub + 1;
        }else
            sub = (picoRTOS_priority_t)0;
    }

    /* adjust sub_count */
    for (i = (picoRTOS_pid_t)CONFIG_TASK_COUNT; i-- != 0;)
        if (TASK_BY_PID(i).prio == TASK_BY_PID(i + 1).prio)
            TASK_BY_PID(i).sub_count = (size_t)TASK_BY_PID(i + 1).sub_count;
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
    arch_start_first_task(TASK_BY_PID(TASK_IDLE_PID).sp);
}

void picoRTOS_suspend()
{
    picoRTOS_assert_fatal(picoRTOS.is_running);

    arch_suspend();
    arch_spin_lock();
}

void picoRTOS_resume()
{
    picoRTOS_assert_fatal(picoRTOS.is_running);

    arch_spin_unlock();
    arch_resume();
}

void picoRTOS_schedule(void)
{
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_SMP_TASK_STATE_RUNNING);
    arch_syscall(PICORTOS_SYSCALL_SWITCH_CONTEXT, NULL);
}

void picoRTOS_sleep(picoRTOS_tick_t delay)
{
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_SMP_TASK_STATE_RUNNING);
    arch_syscall(PICORTOS_SYSCALL_SLEEP, (void*)delay);
}

void picoRTOS_sleep_until(picoRTOS_tick_t *ref, picoRTOS_tick_t period)
{
    picoRTOS_assert_fatal(period > 0);
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_SMP_TASK_STATE_RUNNING);

    picoRTOS_tick_t tick = picoRTOS_get_tick();
    picoRTOS_tick_t elapsed = tick - *ref;

    /* check the clock */
    if (elapsed < period) {
        picoRTOS_tick_t delay = period - elapsed;
        *ref = *ref + period;
        arch_syscall(PICORTOS_SYSCALL_SLEEP, (void*)delay);
    }else{
        /* missed the clock: reset to tick and signal */
        picoRTOS_break();
        /*@notreached@*/ *ref = tick;
    }
}

void picoRTOS_kill(void)
{
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_SMP_TASK_STATE_RUNNING);
    arch_syscall(PICORTOS_SYSCALL_KILL, NULL);
}

picoRTOS_priority_t picoRTOS_self(void)
{
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_SMP_TASK_STATE_RUNNING);
    return (picoRTOS_priority_t)picoRTOS.index[arch_core()];
}

picoRTOS_tick_t picoRTOS_get_tick(void)
{
    picoRTOS_assert_fatal(TASK_CURRENT().state == PICORTOS_SMP_TASK_STATE_RUNNING);
    return picoRTOS.tick;
}

/* SYSCALLS */

static void syscall_sleep(picoRTOS_tick_t delay)
{
    if (delay > 0) {
        struct picoRTOS_SMP_task_core *task = &TASK_CURRENT();
        task->tick = picoRTOS.tick + delay;
        task->state = PICORTOS_SMP_TASK_STATE_SLEEP;
    }
}

static void syscall_kill(void)
{
    TASK_CURRENT().state = PICORTOS_SMP_TASK_STATE_EMPTY;
}

/*@exposed@*/
static picoRTOS_stack_t *syscall_switch_context(picoRTOS_stack_t *sp)
{
    picoRTOS_core_t core = arch_core();
    picoRTOS_pid_t index = picoRTOS.index[core];
    picoRTOS_mask_t mask = (picoRTOS_mask_t)(1u << core);

    struct picoRTOS_SMP_task_core *task = &TASK_BY_PID(index);

    picoRTOS_assert_fatal(sp >= task->stack);
    picoRTOS_assert_fatal(sp < task->stack + task->stack_count);

    /* stats */
    task_core_stat_switch(task);

    /* store current sp */
    task->sp = sp;

    do {
        index++;
        /* ignore tasks ran by other cores or not available */
        if (task_core_is_available(&TASK_BY_PID(index), mask))
            break;
        /* jump out on idle anyway */
    } while (index < (picoRTOS_pid_t)(TASK_IDLE_PID + (int)core));

    /* make previous task available for other cores */
    if (task->state == PICORTOS_SMP_TASK_STATE_RUNNING)
        task->state = PICORTOS_SMP_TASK_STATE_READY;

    /* refresh current task pointer */
    picoRTOS.index[core] = index;
    task = &TASK_BY_PID(index);

    /* state machine */
    task->state = PICORTOS_SMP_TASK_STATE_RUNNING;

    /* stats */
    task_core_stat_begin(task);

    return task->sp;
}

picoRTOS_stack_t *picoRTOS_syscall(picoRTOS_stack_t *sp, picoRTOS_syscall_t syscall, void *priv)
{
    picoRTOS_assert_fatal(syscall < PICORTOS_SYSCALL_COUNT);

    picoRTOS_stack_t *ret;

    arch_spin_lock();

    switch (syscall) {
    case PICORTOS_SYSCALL_SLEEP: syscall_sleep((picoRTOS_tick_t)priv); break;
    case PICORTOS_SYSCALL_KILL: syscall_kill(); break;
    default: /* PICORTOS_SYSCALL_SWITCH_CONTEXT */ break;
    }

    ret = syscall_switch_context(sp);

    arch_spin_unlock();

    return ret;
}

/* TICK */

picoRTOS_stack_t *picoRTOS_tick(picoRTOS_stack_t *sp)
{
    picoRTOS_core_t core = arch_core();
    picoRTOS_pid_t index = picoRTOS.index[core];
    picoRTOS_mask_t mask = (picoRTOS_mask_t)(1u << core);

    struct picoRTOS_SMP_task_core *task = &TASK_BY_PID(index);

    picoRTOS_assert_fatal(sp >= task->stack);
    picoRTOS_assert_fatal(sp < (task->stack + task->stack_count));

    arch_spin_lock();

    /* stats */
    task_core_stat_preempt(task);

    /* store current sp */
    task->sp = sp;

    /* make preempted task available to other cores */
    if (task->state == PICORTOS_SMP_TASK_STATE_RUNNING)
        task->state = PICORTOS_SMP_TASK_STATE_READY;

    /* advance tick & propagate to auxiliary cores */
    if (core == picoRTOS.main_core) {
        picoRTOS.tick++;
        arch_propagate_tick();
    }

    /* quick pass on sleeping tasks + idle */
    picoRTOS_pid_t pid = (picoRTOS_pid_t)TASK_COUNT;

    while (pid-- != 0) {
        if (TASK_BY_PID(pid).state == PICORTOS_SMP_TASK_STATE_SLEEP &&
            TASK_BY_PID(pid).tick == picoRTOS.tick)
            /* task is ready to rumble */
            TASK_BY_PID(pid).state = PICORTOS_SMP_TASK_STATE_READY;

        /* select highest priority ready task */
        if (task_core_is_available(&TASK_BY_PID(pid), mask))
            index = pid;

        /* reset task counter */
        TASK_BY_PID(pid).stat.counter = (picoRTOS_cycles_t)0;
    }

    /* refresh current task pointer */
    picoRTOS.index[core] = index;
    task = &TASK_BY_PID(index);

    /* state machine */
    task->state = PICORTOS_SMP_TASK_STATE_RUNNING;

    /* stats */
    task_core_stat_begin(task);

    arch_spin_unlock();

    return task->sp;
}

void picoRTOS_register_interrupt(picoRTOS_irq_t irq,
                                 picoRTOS_isr_fn fn,
                                 void *priv)
{
    picoRTOS_assert_fatal(fn != NULL);
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
    picoRTOS_assert_fatal(core_mask != 0);
    picoRTOS_assert_fatal(core_mask < (picoRTOS_mask_t)(1 << CONFIG_SMP_CORES));

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
    picoRTOS_assert_fatal(core_mask != 0);
    picoRTOS_assert_fatal(core_mask < (picoRTOS_mask_t)(1 << CONFIG_SMP_CORES));

    arch_smp_disable_interrupt(irq, core_mask);
}

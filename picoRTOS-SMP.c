#include "picoRTOS-SMP.h"
#include "picoRTOS-SMP_port.h"

/* CHECK FOR OBVIOUS ERRORS */
#if CONFIG_DEFAULT_STACK_COUNT < ARCH_MIN_STACK_COUNT
# error Default stack is too small
#endif

#if PICORTOS_CYCLES_PER_TICK < 1
# error Erroneous timing values
#endif

/* TASKS */

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
    /* stats */
    struct {
        picoRTOS_cycles_t counter;
        picoRTOS_cycles_t watermark_lo;
        picoRTOS_cycles_t watermark_hi;
    } stat;
};

/* user tasks + idle */
#define TASK_COUNT     (CONFIG_TASK_COUNT + CONFIG_SMP_CORES)
#define TASK_IDLE_PRIO CONFIG_TASK_COUNT
/* shortcut for current task */
#define TASK_CURRENT()  (picoRTOS.task[picoRTOS.index[arch_core()]])
#define TASK_BY_PRIO(x) (picoRTOS.task[(x)])

struct picoRTOS_SMP_core {
    bool is_running;
    size_t index[CONFIG_SMP_CORES];
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

static void idle_tasks_setup(void)
{
    size_t i;

    for (i = 0; i < (size_t)CONFIG_SMP_CORES; i++) {

        struct picoRTOS_task idle;
        picoRTOS_priority_t prio = (picoRTOS_priority_t)(TASK_IDLE_PRIO + (int)i);

        picoRTOS_task_init(&idle, arch_idle, NULL, picoRTOS_SMP_stack[i].idle,
                           (size_t)ARCH_MIN_STACK_COUNT);

        /* similar to picoRTOS_add_task, but without count limit */
        TASK_BY_PRIO(prio).sp = arch_prepare_stack(&idle);
        TASK_BY_PRIO(prio).state = PICORTOS_SMP_TASK_STATE_READY;
        TASK_BY_PRIO(prio).core = (picoRTOS_mask_t)(1u << i);
        /* checks */
        TASK_BY_PRIO(prio).stack = picoRTOS_SMP_stack[i].idle;
        TASK_BY_PRIO(prio).stack_count = idle.stack_count;
    }
}

static void task_core_zero(/*@out@*/ struct picoRTOS_SMP_task_core *task)
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
        task_core_zero(&TASK_BY_PRIO(n));

    /* IDLE */
    idle_tasks_setup();

    picoRTOS.tick = 0;
    n = (size_t)CONFIG_SMP_CORES;

    /* all cores start in idle */
    while (n-- != 0)
        picoRTOS.index[n] = (size_t)TASK_IDLE_PRIO + n;

    /* set main core */
    picoRTOS.main_core = arch_core();

    /* RTOS status */
    picoRTOS.is_running = false;
}

void picoRTOS_add_task(struct picoRTOS_task *task, picoRTOS_priority_t prio)
{
    /* check params */
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT);
    picoRTOS_assert_fatal(TASK_BY_PRIO(prio).state == PICORTOS_SMP_TASK_STATE_EMPTY);

    /* state machine */
    TASK_BY_PRIO(prio).sp = arch_prepare_stack(task);
    TASK_BY_PRIO(prio).state = PICORTOS_SMP_TASK_STATE_READY;
    TASK_BY_PRIO(prio).core = PICORTOS_SMP_CORE_ANY;
    /* checks */
    TASK_BY_PRIO(prio).stack = task->stack;
    TASK_BY_PRIO(prio).stack_count = task->stack_count;
}

picoRTOS_priority_t picoRTOS_get_next_available_priority(void)
{
    picoRTOS_priority_t prio = (picoRTOS_priority_t)0;

    for (; prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT; prio++)
        if (TASK_BY_PRIO(prio).state == PICORTOS_SMP_TASK_STATE_EMPTY)
            return prio;

    /* no slot available */
    picoRTOS_assert_fatal(prio != (picoRTOS_priority_t)CONFIG_TASK_COUNT);
    /*@notreached@*/
    return (picoRTOS_priority_t)-1;
}

picoRTOS_priority_t picoRTOS_get_last_available_priority(void)
{
    picoRTOS_priority_t prio = (picoRTOS_priority_t)CONFIG_TASK_COUNT;

    while (prio-- != 0)
        if (TASK_BY_PRIO(prio).state == PICORTOS_SMP_TASK_STATE_EMPTY)
            return prio;

    /* no slot available */
    picoRTOS_assert_fatal(prio != (picoRTOS_priority_t)-1);
    /*@notreached@*/
    return (picoRTOS_priority_t)-1;
}

/* Group: picoRTOS-SMP extensions */

/* Function: picoRTOS_SMP_set_core_mask
 * Assign a task to specific core(s)
 *
 * Parameters:
 *  prio - The task priority/identifier
 *  core_mask - The cores to run the taks on in the form of a mask, where
 *  (1 << 0) is core 0, (1 << 1) is core 1, etc
 *
 * Example:
 * (start code)
 * picoRTOS_add_task(&task, (picoRTOS_priority_t)TASK_PRIO);
 * picoRTOS_SMP_set_core_mask((picoRTOS_priority_t)TASK_PRIO, (picoRTOS_mask_t)(1 << 1));
 * (end)
 *
 * Remarks:
 * picoRTOS-SMP will throw a debug exception and stall if the mask is 0, doesn't match the
 * exact number of cores, if prio is > CONFIG_TASK_COUNT or is not assigned yet
 */
void picoRTOS_SMP_set_core_mask(picoRTOS_priority_t prio,
                                picoRTOS_mask_t core_mask)
{
    picoRTOS_assert_fatal(core_mask != 0);
    picoRTOS_assert_fatal(core_mask < (picoRTOS_mask_t)(1 << CONFIG_SMP_CORES));
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT);
    picoRTOS_assert_fatal(TASK_BY_PRIO(prio).state == PICORTOS_SMP_TASK_STATE_READY);

    TASK_BY_PRIO(prio).core = core_mask;
}

void picoRTOS_start(void)
{
    arch_smp_init();
    picoRTOS.is_running = true;

    /* start auxiliary cores first */
    picoRTOS_core_t i;

    for (i = (picoRTOS_core_t)1;
         i < (picoRTOS_core_t)CONFIG_SMP_CORES; i++) {
        /* allocate a master stack & idle */
        arch_core_init(i, picoRTOS_SMP_stack[i].core, (size_t)ARCH_MIN_STACK_COUNT,
                       TASK_BY_PRIO(TASK_IDLE_PRIO + i).sp);
    }

    /* start scheduler on core #0 */
    arch_start_first_task(TASK_BY_PRIO(TASK_IDLE_PRIO).sp);
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
        *ref = tick;
        picoRTOS_break();
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
    size_t index = picoRTOS.index[core];
    picoRTOS_mask_t mask = (picoRTOS_mask_t)(1u << core);

    struct picoRTOS_SMP_task_core *task = &TASK_BY_PRIO(index);

    picoRTOS_assert_fatal(sp >= task->stack);
    picoRTOS_assert_fatal(sp < task->stack + task->stack_count);

    /* stats */
    task_core_stat_switch(task);

    /* store current sp */
    task->sp = sp;

    do {
        index++;
        /* ignore tasks ran by other cores or not ready */
        if ((TASK_BY_PRIO(index).core & mask) != 0 &&
            TASK_BY_PRIO(index).state == PICORTOS_SMP_TASK_STATE_READY)
            break;
        /* jump out on idle anyway */
    } while (index < (TASK_IDLE_PRIO + (size_t)core));

    /* make previous task available for other cores */
    if (task->state == PICORTOS_SMP_TASK_STATE_RUNNING)
        task->state = PICORTOS_SMP_TASK_STATE_READY;

    /* refresh current task pointer */
    picoRTOS.index[core] = index;
    task = &TASK_BY_PRIO(index);

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
    size_t index = picoRTOS.index[core];
    picoRTOS_mask_t mask = (picoRTOS_mask_t)(1u << core);

    struct picoRTOS_SMP_task_core *task = &TASK_BY_PRIO(index);

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
    size_t n = (size_t)TASK_COUNT;

    while (n-- != 0) {
        if (TASK_BY_PRIO(n).state == PICORTOS_SMP_TASK_STATE_SLEEP &&
            TASK_BY_PRIO(n).tick == picoRTOS.tick)
            /* task is ready to rumble */
            TASK_BY_PRIO(n).state = PICORTOS_SMP_TASK_STATE_READY;

        /* select highest priority ready task */
        if ((TASK_BY_PRIO(n).core & mask) != 0 &&
            TASK_BY_PRIO(n).state == PICORTOS_SMP_TASK_STATE_READY)
            index = n;

        /* reset task counter */
        TASK_BY_PRIO(n).stat.counter = (picoRTOS_cycles_t)0;
    }

    /* refresh current task pointer */
    picoRTOS.index[core] = index;
    task = &TASK_BY_PRIO(index);

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

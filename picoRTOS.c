#include "picoRTOS.h"
#include "picoRTOS_port.h"

/* CHECK FOR OBVIOUS ERRORS */

#if CONFIG_DEFAULT_STACK_COUNT < ARCH_MIN_STACK_COUNT
# error Default stack is too small
#endif

#if PICORTOS_CYCLES_PER_TICK < 1
# error Erroneous timing values
#endif

/* SCHEDULER main structures */

struct picoRTOS_task_stat {
    picoRTOS_cycles_t counter;
    picoRTOS_cycles_t watermark_lo;
    picoRTOS_cycles_t watermark_hi;
};

typedef enum {
    PICORTOS_TASK_STATE_EMPTY,
    PICORTOS_TASK_STATE_READY,
    PICORTOS_TASK_STATE_SLEEP
} picoRTOS_task_state_t;

struct picoRTOS_task_core {
    /*@temp@*/ picoRTOS_stack_t *sp;
    picoRTOS_task_state_t state;
    picoRTOS_tick_t tick;
    /*@temp@*/ picoRTOS_stack_t *stack;
    size_t stack_count;
    struct picoRTOS_task_stat stat;
};

/* user-defined tasks + idle */
#define TASK_COUNT      (CONFIG_TASK_COUNT + 1)
#define TASK_IDLE_PRIO  (TASK_COUNT - 1)
/* shortcut for current task */
#define TASK_CURRENT()  (picoRTOS.task[picoRTOS.index])

struct picoRTOS_core {
    bool is_running;
    size_t index;
    picoRTOS_tick_t tick;
    struct picoRTOS_task_core task[TASK_COUNT];
    /* IDLE */
    picoRTOS_stack_t idle_stack[ARCH_MIN_STACK_COUNT];
};

/* main core component */
static struct picoRTOS_core picoRTOS;

static void task_core_zero(/*@out@*/ struct picoRTOS_task_core *task)
{
    task->sp = NULL;
    task->state = PICORTOS_TASK_STATE_EMPTY;
    task->tick = 0;
    task->stack = NULL;
    task->stack_count = 0;
    /* stats */
    task->stat.counter = (picoRTOS_cycles_t)0;
    task->stat.watermark_lo = (picoRTOS_cycles_t)PICORTOS_CYCLES_PER_TICK;
    task->stat.watermark_hi = (picoRTOS_cycles_t)0;
}

static inline void task_core_stat_begin(struct picoRTOS_task_core *task)
{
    task->stat.counter = arch_counter();
}

static void task_core_stat_watermark(struct picoRTOS_task_core *task)
{
    if (task->stat.counter < task->stat.watermark_lo)
        task->stat.watermark_lo = task->stat.counter;

    if (task->stat.counter > task->stat.watermark_hi)
        task->stat.watermark_hi = task->stat.counter;
}

static void task_core_stat_switch(struct picoRTOS_task_core *task)
{
    task->stat.counter = arch_counter() - task->stat.counter;
    task_core_stat_watermark(task);
}

static void task_core_stat_preempt(struct picoRTOS_task_core *task)
{
    task->stat.counter = (picoRTOS_cycles_t)PICORTOS_CYCLES_PER_TICK -
                         task->stat.counter;

    task_core_stat_watermark(task);
}

/* Group: picoRTOS scheduler API */

/* Function: picoRTOS_init
 * Initialises picoRTOS (mandatory)
 */
void picoRTOS_init(void)
{
    /* zero all tasks */
    size_t n = (size_t)TASK_COUNT;

    while (n-- != 0)
        task_core_zero(&picoRTOS.task[n]);

    /* IDLE */
    struct picoRTOS_task idle;

    picoRTOS_task_init(&idle, arch_idle, NULL, picoRTOS.idle_stack,
                       (size_t)ARCH_MIN_STACK_COUNT);

    picoRTOS_add_task(&idle, (picoRTOS_priority_t)TASK_IDLE_PRIO);
    picoRTOS.index = (size_t)TASK_IDLE_PRIO; /* starts at idle */
    picoRTOS.tick = 0;

    /* RTOS status */
    picoRTOS.is_running = false;
}

/* Function: picoRTOS_task_init
 * Initialises a task structure
 *
 * Parameters:
 *  task - A pointer to the task stucture to initialize
 *  fn - The entry point of the main task
 *  priv - The parameter that will be passed to the task
 *  stack - A pointer to tasks's stack
 *  stack_count - The size of the stack in number of elements
 *
 * Example:
 * (start code)
 * struct picoRTOS_task task;
 * static picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];
 *
 * picoRTOS_task_init(&task, task_entry_point, &task_context, stack, (size_t)CONFIG_DEFAULT_STACK_COUNT);
 * (end)
 *
 * picoRTOS will throw a debug exception and stall if stack_count < ARCH_MIN_STACK_COUNT (architecture-dependent)
 */
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

/* Function: picoRTOS_add_task
 * Adds a task to picoRTOS
 *
 * Parameters:
 *  task - A pointer to already initialised task structure
 *  prio - The priority/identitifer of the task (MUST be < CONFIG_TASK_COUNT)
 *
 * Example:
 * (start code)
 * picoRTOS_add_task(&task, (picoRTOS_priority_t)TASK_PRIO);
 * (end)
 *
 * Remarks:
 * picoRTOS will throw a debug exception and stall if prio >= CONFIG_TASK_COUNT or
 * if the priority is already in use
 */
void picoRTOS_add_task(struct picoRTOS_task *task, picoRTOS_priority_t prio)
{
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)TASK_COUNT);
    picoRTOS_assert_fatal(picoRTOS.task[prio].state == PICORTOS_TASK_STATE_EMPTY);

    picoRTOS.task[prio].sp = arch_prepare_stack(task);
    picoRTOS.task[prio].state = PICORTOS_TASK_STATE_READY;
    /* stat */
    picoRTOS.task[prio].stack = task->stack;
    picoRTOS.task[prio].stack_count = task->stack_count;
}

/* Function: picoRTOS_get_next_available_priority
 * Gets the first available priority by ascending order (0 -> n)
 *
 * Example:
 * (start code)
 * picoRTOS_priority_t prio = picoRTOS_get_next_available_priority()
 * picoRTOS_add_task(&task, prio);
 * (end)
 * Or:
 * (start code)
 * picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
 * (end)
 *
 * Remarks:
 * If no priority is available, picoRTOS will throw a debug exception and stall
 */
picoRTOS_priority_t picoRTOS_get_next_available_priority(void)
{
    picoRTOS_priority_t prio = (picoRTOS_priority_t)0;

    for (; prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT; prio++)
        if (picoRTOS.task[prio].state == PICORTOS_TASK_STATE_EMPTY)
            return prio;

    /* no slot available */
    picoRTOS_assert_fatal(prio != (picoRTOS_priority_t)CONFIG_TASK_COUNT);
    /*@notreached@*/
    return (picoRTOS_priority_t)-1;
}

/* Function: picoRTOS_get_last_available_priority
 * Gets the first available priority by descending order (n -> 0)
 *
 * This is typically helpful for watchdog refreshing tasks
 *
 * Example:
 * (start code)
 * picoRTOS_priority_t prio = picoRTOS_get_last_available_priority()
 * picoRTOS_add_task(&task, prio);
 * (end)
 * Or:
 * (start code)
 * picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());
 * (end)
 *
 * Remarks:
 * If no priority is available, picoRTOS will throw a debug exception and stall
 */
picoRTOS_priority_t picoRTOS_get_last_available_priority(void)
{
    picoRTOS_priority_t prio = (picoRTOS_priority_t)CONFIG_TASK_COUNT;

    while (prio-- != 0)
        if (picoRTOS.task[prio].state == PICORTOS_TASK_STATE_EMPTY)
            return prio;

    /* no slot available */
    picoRTOS_assert_fatal(prio != (picoRTOS_priority_t)-1);
    /*@notreached@*/
    return (picoRTOS_priority_t)-1;
}

/* Function: picoRTOS_start
 * Starts the scheduling. SHOULD never return
 */
void picoRTOS_start(void)
{
    arch_init();
    picoRTOS.is_running = true;
    arch_start_first_task(picoRTOS.task[TASK_IDLE_PRIO].sp);
}

/* Function: picoRTOS_suspend
 * Suspends the scheduling. Typical use is critical sections
 */
void picoRTOS_suspend()
{
    picoRTOS_assert_fatal(picoRTOS.is_running);
    arch_suspend();
}

/* Function: picoRTOS_resume
 * Resumes the scheduling. Typical use is critical sections
 */
void picoRTOS_resume()
{
    picoRTOS_assert_fatal(picoRTOS.is_running);
    arch_resume();
}

/* Function: picoRTOS_schedule
 * Puts the current task to sleep until next tick
 */
void picoRTOS_schedule(void)
{
    picoRTOS_assert_fatal(picoRTOS.is_running);
    arch_syscall(PICORTOS_SYSCALL_SWITCH_CONTEXT, NULL);
}

/* Function: picoRTOS_sleep
 * Puts the current task to sleep for the specified number of ticks
 *
 * Parameters:
 *  delay - A delay in picoRTOS_tick_t (ticks)
 *
 */
void picoRTOS_sleep(picoRTOS_tick_t delay)
{
    picoRTOS_assert_fatal(picoRTOS.is_running);
    arch_syscall(PICORTOS_SYSCALL_SLEEP, (void*)delay);
}

/* Function: picoRTOS_sleep_until
 * Puts the current task to sleep until *ref + period is elapsed.
 *
 * Parameters:
 *  ref - A pointer to a reference time in ticks (will be overwritten)
 *  period - A period in ticks
 *
 * Example:
 * (start code)
 * picoRTOS_tick_t ref = picoRTOS_get_tick();
 *
 * for(;;){
 *   my_periodic_function();
 *   picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
 * }
 * (end)
 *
 * Remarks:
 * If the period is already elapsed (aka we're late), picoRTOS will throw a debug
 * exception, update *ref to current tick and continue anyway
 */
void picoRTOS_sleep_until(picoRTOS_tick_t *ref, picoRTOS_tick_t period)
{
    picoRTOS_assert_fatal(period > 0);
    picoRTOS_assert_fatal(picoRTOS.is_running);

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

/* Function: picoRTOS_kill
 * Kills the current task (suicide)
 */
void picoRTOS_kill(void)
{
    picoRTOS_assert_fatal(picoRTOS.is_running);
    arch_syscall(PICORTOS_SYSCALL_KILL, NULL);
}

/* Function: picoRTOS_self
 * Returns the current task's priority/identitifer
 */
picoRTOS_priority_t picoRTOS_self(void)
{
    picoRTOS_assert_fatal(picoRTOS.is_running);
    return (picoRTOS_priority_t)picoRTOS.index;
}

/* Function: picoRTOS_get_tick
 * Returns the current system tick/timer
 */
picoRTOS_tick_t picoRTOS_get_tick(void)
{
    picoRTOS_assert_fatal(picoRTOS.is_running);
    return picoRTOS.tick;
}

/* SYSCALLS */

static void syscall_sleep(picoRTOS_tick_t delay)
{
    if (delay > 0) {
        TASK_CURRENT().tick = picoRTOS.tick + delay;
        TASK_CURRENT().state = PICORTOS_TASK_STATE_SLEEP;
    }
}

static void syscall_kill(void)
{
    TASK_CURRENT().state = PICORTOS_TASK_STATE_EMPTY;
}

/*@exposed@*/
static picoRTOS_stack_t *syscall_switch_context(picoRTOS_stack_t *sp)
{
    /* store current sp */
    TASK_CURRENT().sp = sp;

    /* stats */
    task_core_stat_switch(&TASK_CURRENT());

    /* choose next task to run */
    do
        picoRTOS.index++;
    /* ignore sleeping and empty tasks */
    while (picoRTOS.index < (size_t)TASK_IDLE_PRIO &&
           TASK_CURRENT().state != PICORTOS_TASK_STATE_READY);

    /* stats */
    task_core_stat_begin(&TASK_CURRENT());

    return TASK_CURRENT().sp;
}

picoRTOS_stack_t *picoRTOS_syscall(picoRTOS_stack_t *sp, picoRTOS_syscall_t syscall, void *priv)
{
    picoRTOS_assert_fatal(syscall < PICORTOS_SYSCALL_COUNT);
    picoRTOS_assert_fatal(sp >= TASK_CURRENT().stack);
    picoRTOS_assert_fatal(sp < (TASK_CURRENT().stack + TASK_CURRENT().stack_count));

    switch (syscall) {
    case PICORTOS_SYSCALL_SLEEP: syscall_sleep((picoRTOS_tick_t)priv); break;
    case PICORTOS_SYSCALL_KILL: syscall_kill(); break;
    default: /* PICORTOS_SYSCALL_SWITCH_CONTEXT */ break;
    }

    return syscall_switch_context(sp);
}

/* TICK */

picoRTOS_stack_t *picoRTOS_tick(picoRTOS_stack_t *sp)
{
    picoRTOS_assert_fatal(sp >= TASK_CURRENT().stack);
    picoRTOS_assert_fatal(sp < (TASK_CURRENT().stack + TASK_CURRENT().stack_count));

    /* store current sp */
    TASK_CURRENT().sp = sp;

    /* stats */
    task_core_stat_preempt(&TASK_CURRENT());

    /* advance tick */
    picoRTOS.tick++;

    /* quick pass on sleeping tasks + idle */
    size_t n = (size_t)TASK_COUNT;

    while (n-- != 0) {
        if (picoRTOS.task[n].state == PICORTOS_TASK_STATE_SLEEP &&
            picoRTOS.task[n].tick == picoRTOS.tick)
            /* task is ready to rumble */
            picoRTOS.task[n].state = PICORTOS_TASK_STATE_READY;

        /* select highest priority ready task */
        if (picoRTOS.task[n].state == PICORTOS_TASK_STATE_READY)
            picoRTOS.index = n;

        /* reset task counter */
        picoRTOS.task[n].stat.counter = (picoRTOS_cycles_t)0;
    }

    /* stats */
    task_core_stat_begin(&TASK_CURRENT());

    return TASK_CURRENT().sp;
}

/* Group: picoRTOS interrupt API */

/* Function: picoRTOS_register_interrupt
 * Registers an interrupt/irq to the system
 *
 * Parameters:
 *  irq - The irq number to register
 *  fn - The interrupt service routine to run
 *  priv - The parameter that will be passed to the ISR
 */
void picoRTOS_register_interrupt(picoRTOS_irq_t irq,
                                 picoRTOS_isr_fn fn,
                                 void *priv)
{
    picoRTOS_assert_fatal(fn != NULL);
    arch_register_interrupt(irq, fn, priv);
}

/* Function: picoRTOS_enable_interrupt
 * Enables an interrupt on the system
 *
 * Parameters:
 *  irq - The irq number to enable
 */
void picoRTOS_enable_interrupt(picoRTOS_irq_t irq)
{
    arch_enable_interrupt(irq);
}

/* Function: picoRTOS_disable_interrupt
 * Disables an interrupt on the system
 *
 * Parameters:
 *  irq - The irq number to disable
 */
void picoRTOS_disable_interrupt(picoRTOS_irq_t irq)
{
    arch_disable_interrupt(irq);
}

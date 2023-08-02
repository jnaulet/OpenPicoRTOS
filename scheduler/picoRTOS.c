#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <generated/autoconf.h>

/* CHECK FOR OBVIOUS ERRORS */

#if CONFIG_DEFAULT_STACK_COUNT < ARCH_MIN_STACK_COUNT
# error Default stack is too small
#endif

#define PICORTOS_CYCLES_PER_TICK (CONFIG_SYSCLK_HZ / CONFIG_TICK_HZ)
#if PICORTOS_CYCLES_PER_TICK < 1
# error Erroneous timing values
#endif

/* SCHEDULER main structures */

typedef enum {
    PICORTOS_TASK_STATE_EMPTY,
    PICORTOS_TASK_STATE_READY,
    PICORTOS_TASK_STATE_SLEEP
} picoRTOS_task_state_t;

struct picoRTOS_task_core {
    /* state machine */
    /*@temp@*/ picoRTOS_stack_t *sp;
    picoRTOS_task_state_t state;
    picoRTOS_tick_t tick;
    /* checks */
    /*@temp@*/ picoRTOS_stack_t *stack_bottom;
    /*@temp@*/ picoRTOS_stack_t *stack_top;
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
#define TASK_COUNT     (CONFIG_TASK_COUNT + 1)
#define TASK_IDLE_PRIO (TASK_COUNT - 1)
#define TASK_IDLE_PID  (TASK_COUNT - 1)
/* shortcut for current task */
#define TASK_CURRENT() (picoRTOS.task[picoRTOS.index])
#define TASK_BY_PID(x) (picoRTOS.task[(x)])

struct picoRTOS_core {
    bool is_running;
    picoRTOS_pid_t index;
    picoRTOS_tick_t tick;
    picoRTOS_pid_t pid_count;
    struct picoRTOS_task_core task[TASK_COUNT];
    picoRTOS_stack_t idle_stack[ARCH_MIN_STACK_COUNT];
};

/* main core component */
static struct picoRTOS_core picoRTOS;

static void task_core_init(/*@out@*/ struct picoRTOS_task_core *task)
{
    /* state machine */
    task->sp = NULL;
    task->state = PICORTOS_TASK_STATE_EMPTY;
    task->tick = 0;
    /* checks */
    task->stack_bottom = NULL;
    task->stack_top = NULL;
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

static bool task_core_is_available(struct picoRTOS_task_core *task)
{
    /* task is ready and it's its turn */
    return task->state == PICORTOS_TASK_STATE_READY &&
           ((size_t)picoRTOS.tick % task->sub_count) == (size_t)task->sub;
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

static void task_idle_init(void)
{
    /* IDLE */
    /* similar to picoRTOS_add_task, but without count limit */
    TASK_BY_PID(TASK_IDLE_PID).state = PICORTOS_TASK_STATE_READY;
    TASK_BY_PID(TASK_IDLE_PID).sp = arch_prepare_stack(picoRTOS.idle_stack,
                                                       (size_t)ARCH_MIN_STACK_COUNT,
                                                       arch_idle, NULL);
    /* checks */
    TASK_BY_PID(TASK_IDLE_PID).stack_bottom = picoRTOS.idle_stack;
    TASK_BY_PID(TASK_IDLE_PID).stack_top = picoRTOS.idle_stack + ARCH_MIN_STACK_COUNT;
    TASK_BY_PID(TASK_IDLE_PID).stack_count = (size_t)ARCH_MIN_STACK_COUNT;
    /* shared priorities, ignored by sort anyway */
    TASK_BY_PID(TASK_IDLE_PID).prio = (picoRTOS_priority_t)TASK_IDLE_PRIO;
}

/* Function: picoRTOS_init
 * Initialises picoRTOS (mandatory)
 */
void picoRTOS_init(void)
{
    /* reset pids */
    picoRTOS.pid_count = 0;

    /* zero all tasks */
    size_t n = (size_t)TASK_COUNT;

    while (n-- != 0)
        task_core_init(&TASK_BY_PID(n));

    /* IDLE */
    task_idle_init();
    picoRTOS.index = (picoRTOS_pid_t)TASK_IDLE_PID; /* first task */
    picoRTOS.tick = (picoRTOS_tick_t)-1;            /* 1st tick will be 0 */

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
                        picoRTOS_task_fn fn, void *priv,
                        picoRTOS_stack_t *stack,
                        size_t stack_count)
{
    picoRTOS_assert_fatal(stack_count >= (size_t)ARCH_MIN_STACK_COUNT,
                          return );

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
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT,
                          return );

    picoRTOS_pid_t pid = picoRTOS.pid_count;

    picoRTOS_assert_fatal(pid < (picoRTOS_pid_t)CONFIG_TASK_COUNT, return );
    picoRTOS_assert_fatal(TASK_BY_PID(pid).state == PICORTOS_TASK_STATE_EMPTY, return );

    /* state machine */
    TASK_BY_PID(pid).state = PICORTOS_TASK_STATE_READY;
    TASK_BY_PID(pid).sp = arch_prepare_stack(task->stack, task->stack_count,
                                             task->fn, task->priv);
    /* checks */
    TASK_BY_PID(pid).stack_bottom = task->stack;
    TASK_BY_PID(pid).stack_top = task->stack + task->stack_count;
    TASK_BY_PID(pid).stack_count = task->stack_count;
    /* shared priorities */
    TASK_BY_PID(pid).prio = prio;

    /* increment */
    picoRTOS.pid_count++;
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
 *
 * Beware:
 * This function might not be as useful in picoRTOS v1.7.x as it was in picoRTOS v1.6.x
 * and might disappear in the future
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
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)TASK_IDLE_PRIO,
                          return (picoRTOS_priority_t)-1);

    return prio;
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
 *
 * Beware:
 * This function might not be as useful in picoRTOS v1.7.x as it was in picoRTOS v1.6.x
 * and might disappear in the future
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
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)TASK_IDLE_PRIO,
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
    picoRTOS_priority_t sub = (picoRTOS_priority_t)0;

    /* count subs */
    for (pid = (picoRTOS_pid_t)1;
         pid < (picoRTOS_pid_t)CONFIG_TASK_COUNT; pid++) {
        /* increment sub priority & count */
        if (TASK_BY_PID(pid).prio == TASK_BY_PID(pid - 1).prio) {
            TASK_BY_PID(pid).sub = ++sub;
            TASK_BY_PID(pid).sub_count = (size_t)sub + 1;
        }else
            sub = (picoRTOS_priority_t)0;
    }

    /* adjust sub_count */
    for (pid = (picoRTOS_pid_t)CONFIG_TASK_COUNT; pid-- != 0;)
        if (TASK_BY_PID(pid).prio == TASK_BY_PID(pid + 1).prio)
            TASK_BY_PID(pid).sub_count = (size_t)TASK_BY_PID(pid + 1).sub_count;
}

/* Function: picoRTOS_start
 * Starts the scheduling. SHOULD never return
 */
void picoRTOS_start(void)
{
    core_sort_tasks();
    core_arrange_shared_priorities();

    arch_init();
    picoRTOS.is_running = true;
    arch_start_first_task(TASK_BY_PID(TASK_IDLE_PID).sp);
}

/* Function: picoRTOS_suspend
 * Suspends the scheduling. Typical use is critical sections
 */
void picoRTOS_suspend()
{
    picoRTOS_assert_fatal(picoRTOS.is_running, return );
    arch_suspend();
}

/* Function: picoRTOS_resume
 * Resumes the scheduling. Typical use is critical sections
 */
void picoRTOS_resume()
{
    picoRTOS_assert_fatal(picoRTOS.is_running, return );
    arch_resume();
}

/* Function: picoRTOS_schedule
 * Puts the current task to sleep until next tick
 */
void picoRTOS_schedule(void)
{
    picoRTOS_assert_fatal(picoRTOS.is_running, return );
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
    picoRTOS_assert_fatal(picoRTOS.is_running, return );
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
    picoRTOS_assert_fatal(period > 0, return );
    picoRTOS_assert_fatal(picoRTOS.is_running, return );

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

/* Function: picoRTOS_kill
 * Kills the current task (suicide)
 */
void picoRTOS_kill(void)
{
    picoRTOS_assert_fatal(picoRTOS.is_running, return );
    arch_syscall(PICORTOS_SYSCALL_KILL, NULL);
}

/* Function: picoRTOS_self
 * Returns the current task's priority/identitifer
 */
picoRTOS_pid_t picoRTOS_self(void)
{
    picoRTOS_assert_fatal(picoRTOS.is_running,
                          return (picoRTOS_pid_t)-1);

    return (picoRTOS_pid_t)picoRTOS.index;
}

/* Function: picoRTOS_get_tick
 * Returns the current system tick/timer
 */
picoRTOS_tick_t picoRTOS_get_tick(void)
{
    picoRTOS_assert_fatal(picoRTOS.is_running,
                          return (picoRTOS_tick_t)-1);

    return picoRTOS.tick;
}

/* SYSCALLS */

static void syscall_sleep(struct picoRTOS_task_core *task, picoRTOS_tick_t delay)
{
    if (delay > 0) {
        task->tick = picoRTOS.tick + delay;
        task->state = PICORTOS_TASK_STATE_SLEEP;
    }
}

static void syscall_kill(struct picoRTOS_task_core *task)
{
    task->state = PICORTOS_TASK_STATE_EMPTY;
}

/*@exposed@*/
static struct picoRTOS_task_core *
syscall_switch_context(struct picoRTOS_task_core *task)
{
    /* stats */
    task_core_stat_switch(task);

    /* choose next task to run */
    do
        picoRTOS.index++;
    /* ignore sleeping, empty tasks & out-of-round sub-tasks */
    while (!task_core_is_available(&TASK_CURRENT()));

    /* refresh current task pointer */
    task = &TASK_CURRENT();

    /* stats */
    task_core_stat_begin(task);

    return task;
}

picoRTOS_stack_t *picoRTOS_syscall(picoRTOS_stack_t *sp, picoRTOS_syscall_t syscall, void *priv)
{
    picoRTOS_assert_fatal(syscall < PICORTOS_SYSCALL_COUNT,
                          return NULL);

    struct picoRTOS_task_core *task = &TASK_CURRENT();

    picoRTOS_assert_fatal(sp >= task->stack_bottom, return NULL);
    picoRTOS_assert_fatal(sp < task->stack_top, return NULL);

    /* store current sp */
    task->sp = sp;

    switch (syscall) {
    case PICORTOS_SYSCALL_SLEEP: syscall_sleep(task, (picoRTOS_tick_t)priv); break;
    case PICORTOS_SYSCALL_KILL: syscall_kill(task); break;
    default: /* PICORTOS_SYSCALL_SWITCH_CONTEXT */ break;
    }

    task = syscall_switch_context(task);
    return task->sp;
}

/* TICK */

picoRTOS_stack_t *picoRTOS_tick(picoRTOS_stack_t *sp)
{
    struct picoRTOS_task_core *task = &TASK_CURRENT();

    picoRTOS_assert_fatal(sp >= task->stack_bottom, return NULL);
    picoRTOS_assert_fatal(sp < task->stack_top, return NULL);

    /* stats */
    task_core_stat_preempt(task);

    /* store current sp */
    task->sp = sp;

    /* advance tick */
    picoRTOS.tick++;

    /* quick pass on sleeping tasks + idle */
    picoRTOS_pid_t pid = (picoRTOS_pid_t)TASK_COUNT;

    while (pid-- != 0) {

        task = &TASK_BY_PID(pid);

        if (task->state == PICORTOS_TASK_STATE_SLEEP &&
            task->tick == picoRTOS.tick)
            /* task is ready to rumble */
            task->state = PICORTOS_TASK_STATE_READY;

        /* select highest priority ready task */
        if (task_core_is_available(task))
            picoRTOS.index = pid;

        /* reset task counter */
        task->stat.counter = (picoRTOS_cycles_t)0;
    }

    /* refresh current task pointer */
    task = &TASK_CURRENT();

    /* stats */
    task_core_stat_begin(task);

    return task->sp;
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

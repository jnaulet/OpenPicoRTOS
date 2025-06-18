#include "picoRTOS.h"
#include "picoRTOS_port.h"

/* CHECK FOR OBVIOUS ERRORS */

#if CONFIG_DEFAULT_STACK_COUNT < ARCH_MIN_STACK_COUNT
# error Default stack is too small
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
    /* deadline */
    size_t deadline_miss_count;
};

/* user-defined tasks + idle */
#define TASK_COUNT     (CONFIG_TASK_COUNT + 1)
#define TASK_IDLE_PRIO (TASK_COUNT - 1)
#define TASK_IDLE_PID  TASK_IDLE_PRIO
/* shortcut for current task */
#define TASK_CURRENT() (picoRTOS.task[picoRTOS.index])
#define TASK_BY_PID(x) (picoRTOS.task[(x)])

#define F_RUNNING   (1 << 0)
#define F_POSTPONED (1 << 1)

struct picoRTOS_core {
    int flags;
    picoRTOS_pid_t index;
    picoRTOS_tick_t tick;
    struct picoRTOS_task_core task[TASK_COUNT];
    picoRTOS_stack_t idle_stack[ARCH_MIN_STACK_COUNT];
} __attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

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
    /* deadline */
    task->deadline_miss_count = 0;
}

static bool task_core_is_available(const struct picoRTOS_task_core *task)
{
    /* task is ready and it's its turn */
    return task->state == PICORTOS_TASK_STATE_READY;
}

static void task_idle_init(void)
{
    /* IDLE */
    static struct picoRTOS_task idle;

    /* ensure proper stack alignment */
    picoRTOS_task_init(&idle, (picoRTOS_task_fn)arch_idle,
                       NULL, picoRTOS.idle_stack,
                       (size_t)ARCH_MIN_STACK_COUNT);

    /* similar to picoRTOS_add_task, but without count limit */
    TASK_BY_PID(TASK_IDLE_PID).state = PICORTOS_TASK_STATE_READY;
    TASK_BY_PID(TASK_IDLE_PID).sp = arch_prepare_stack(idle.stack, idle.stack_count,
                                                       idle.fn, idle.priv);
    /* checks */
    TASK_BY_PID(TASK_IDLE_PID).stack_bottom = idle.stack;
    TASK_BY_PID(TASK_IDLE_PID).stack_top = idle.stack + idle.stack_count;
    TASK_BY_PID(TASK_IDLE_PID).stack_count = idle.stack_count;
}

void picoRTOS_init(void)
{
    /* zero all tasks */
    size_t n = (size_t)TASK_COUNT;

    while (n-- != 0)
        task_core_init(&TASK_BY_PID(n));

    /* IDLE */
    task_idle_init();
    picoRTOS.index = (picoRTOS_pid_t)TASK_IDLE_PID; /* first task */
    picoRTOS.tick = (picoRTOS_tick_t)-1;            /* 1st tick will be 0 */

    /* RTOS status */
    picoRTOS.flags = 0;
}

void picoRTOS_task_init(struct picoRTOS_task *task,
                        picoRTOS_task_fn fn, void *priv,
                        picoRTOS_stack_t *stack,
                        size_t stack_count)
{
    picoRTOS_assert_fatal(stack_count >= (size_t)ARCH_MIN_STACK_COUNT,
                          return );

    task->fn = fn;
    task->priv = priv;

    task->stack = stack;
    task->stack_count = stack_count;
}

void picoRTOS_add_task(struct picoRTOS_task *task, picoRTOS_priority_t prio)
{
    picoRTOS_assert_fatal(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT, return );
    picoRTOS_assert_fatal(TASK_BY_PID(prio).state == PICORTOS_TASK_STATE_EMPTY, return );

    /* state machine */
    TASK_BY_PID(prio).state = PICORTOS_TASK_STATE_READY;
    TASK_BY_PID(prio).sp = arch_prepare_stack(task->stack, task->stack_count,
                                              task->fn, task->priv);
    /* checks */
    TASK_BY_PID(prio).stack_bottom = task->stack;
    TASK_BY_PID(prio).stack_top = task->stack + task->stack_count;
    TASK_BY_PID(prio).stack_count = task->stack_count;
}

picoRTOS_priority_t picoRTOS_get_next_available_priority(void)
{
    picoRTOS_priority_t prio = (picoRTOS_priority_t)0;

    for (; prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT; prio++)
        if (TASK_BY_PID(prio).state == PICORTOS_TASK_STATE_EMPTY)
            return prio;

    /* no slot available */
    picoRTOS_assert_void_fatal(prio < (picoRTOS_priority_t)TASK_IDLE_PRIO);
    return (picoRTOS_priority_t)-1;
}

picoRTOS_priority_t picoRTOS_get_last_available_priority(void)
{
    picoRTOS_priority_t prio = (picoRTOS_priority_t)(TASK_IDLE_PRIO - 1);

    for (; prio != 0; prio--)
        if (TASK_BY_PID(prio).state == PICORTOS_TASK_STATE_EMPTY)
            return prio;

    /* no slot available: overflow */
    picoRTOS_assert_void_fatal(prio != (picoRTOS_priority_t)-1);
    return (picoRTOS_priority_t)-1;
}

void picoRTOS_start(void)
{
    arch_init();
    picoRTOS.flags |= F_RUNNING;
    arch_start_first_task(TASK_BY_PID(TASK_IDLE_PID).sp);
}

void picoRTOS_suspend(void)
{
    picoRTOS_assert_fatal((picoRTOS.flags & F_RUNNING) != 0, return );
    arch_suspend();
}

void picoRTOS_resume(void)
{
    picoRTOS_assert_fatal((picoRTOS.flags & F_RUNNING) != 0, return );
    arch_resume();
}

void picoRTOS_postpone(void)
{
    picoRTOS_assert_fatal((picoRTOS.flags & F_RUNNING) != 0, return );
    arch_syscall(SYSCALL_SWITCH_CONTEXT, NULL);
}

void picoRTOS_sleep(picoRTOS_tick_t delay)
{
    picoRTOS_assert_fatal((picoRTOS.flags & F_RUNNING) != 0, return );
    arch_syscall(SYSCALL_SLEEP, (struct syscall_sleep*)&delay);
}

void picoRTOS_sleep_until(picoRTOS_tick_t *ref, picoRTOS_tick_t period)
{
    picoRTOS_assert_fatal(period > 0, return );
    picoRTOS_assert_fatal((picoRTOS.flags & F_RUNNING) != 0, return );

    struct syscall_sleep_until sc = { *ref, period };

    arch_syscall(SYSCALL_SLEEP_UNTIL, &sc);
    *ref = sc.ref; /* update ref */
}

void picoRTOS_kill(void)
{
    picoRTOS_assert_fatal((picoRTOS.flags & F_RUNNING) != 0, return );
    arch_syscall(SYSCALL_KILL, NULL);
}

picoRTOS_pid_t picoRTOS_self(void)
{
    picoRTOS_assert_fatal((picoRTOS.flags & F_RUNNING) != 0, return (picoRTOS_pid_t)-1);
    return (picoRTOS_pid_t)picoRTOS.index;
}

picoRTOS_tick_t picoRTOS_get_tick(void)
{
    picoRTOS_assert_fatal((picoRTOS.flags & F_RUNNING) != 0, return (picoRTOS_tick_t)-1);
    return picoRTOS.tick;
}

/* SYSCALLS */

static void syscall_sleep(struct picoRTOS_task_core *task,
                          const struct syscall_sleep *sc)
{
    if (sc->delay > 0) {
        task->tick = picoRTOS.tick + sc->delay;
        task->state = PICORTOS_TASK_STATE_SLEEP;
    }
}

static void syscall_sleep_until(struct picoRTOS_task_core *task,
                                struct syscall_sleep_until *sc)
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
}

static void syscall_kill(struct picoRTOS_task_core *task)
{
    task->state = PICORTOS_TASK_STATE_EMPTY;
}

/*@exposed@*/
static struct picoRTOS_task_core *
syscall_switch_context(struct picoRTOS_task_core *task)
{
    int count = 2;

    while (count-- != 0) {
        /* choose next task to run */
        do {
            picoRTOS.index++;
            picoRTOS_assert_void_fatal(picoRTOS.index < (picoRTOS_pid_t)TASK_COUNT);
            /* ignore sleeping, empty tasks & out-of-round sub-tasks */
        } while (!task_core_is_available(&TASK_CURRENT()));

        /* refresh current task pointer */
        task = &TASK_CURRENT();

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

    picoRTOS_assert_void_fatal(count != -1);    /* check */

    return task;
}

picoRTOS_stack_t *picoRTOS_syscall(picoRTOS_stack_t *sp, syscall_t syscall, void *priv)
{
    picoRTOS_assert_fatal(syscall < SYSCALL_COUNT, return NULL);

    struct picoRTOS_task_core *task = &TASK_CURRENT();

    picoRTOS_assert_fatal(sp >= task->stack_bottom, return NULL);
    picoRTOS_assert_fatal(sp < task->stack_top, return NULL);

    /* store current sp */
    task->sp = sp;

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
        picoRTOS.flags |= F_POSTPONED;
        break;
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
    }

    /* refresh current task pointer */
    task = &TASK_CURRENT();

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

void picoRTOS_invalidate_dcache(void *addr, size_t n)
{
    picoRTOS_assert_void_fatal(n > 0);
    arch_invalidate_dcache(addr, n);
}

void picoRTOS_flush_dcache(void *addr, size_t n)
{
    picoRTOS_assert_void_fatal(n > 0);
    arch_flush_dcache(addr, n);
}

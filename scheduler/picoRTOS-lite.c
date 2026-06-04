#include "picoRTOS.h"
#include "picoRTOS_port.h"

/* CHECK FOR OBVIOUS ERRORS */

#if CONFIG_DEFAULT_STACK_COUNT < ARCH_MIN_STACK_COUNT
# error Default stack is too small
#endif

/* SCHEDULER main structures */

typedef enum {
    TASK_STATE_DISABLED,
    TASK_STATE_READY,
    TASK_STATE_SLEEP,
    TASK_STATE_COUNT
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
    struct picoRTOS_task_core task[TASK_COUNT];
} __attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

/* main core component */
static struct picoRTOS_core picoRTOS;
/* stack is provided by arch startup.S */

static void task_core_init(/*@out@*/ struct picoRTOS_task_core *task)
{
    /* state machine */
    task->sp = NULL;
    task->state = TASK_STATE_DISABLED;
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
    return task->state == TASK_STATE_READY;
}

static void task_append(picoRTOS_pid_t pid, struct picoRTOS_task *task)
{
    picoRTOS_assert(pid < (picoRTOS_pid_t)TASK_COUNT, return );
    picoRTOS_assert(TASK_BY_PID(pid).state == TASK_STATE_DISABLED, return );

    /* state machine */
    TASK_BY_PID(pid).state = TASK_STATE_READY;
    TASK_BY_PID(pid).sp = arch_prepare_stack(task->stack, task->stack_count,
                                             task->fn, task->priv);
    /* checks */
    TASK_BY_PID(pid).stack_bottom = task->stack;
    TASK_BY_PID(pid).stack_top = task->stack + task->stack_count;
    TASK_BY_PID(pid).stack_count = task->stack_count;
}

static void task_idle_init(void)
{
    /* IDLE */
    struct picoRTOS_task idle;
    static picoRTOS_stack_t stack[ARCH_MIN_STACK_COUNT];

    /* ensure proper stack alignment */
    picoRTOS_task_init(&idle, (picoRTOS_task_fn)arch_idle, NULL, stack,
                       (size_t)ARCH_MIN_STACK_COUNT);

    /* similar to picoRTOS_add_task, but without count limit */
    task_append((picoRTOS_pid_t)TASK_IDLE_PID, &idle);
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
    picoRTOS_assert(prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT, return );
    picoRTOS_assert(TASK_BY_PID(prio).state == TASK_STATE_DISABLED, return );
    task_append((picoRTOS_pid_t)prio, task);
}

picoRTOS_priority_t picoRTOS_get_next_available_priority(void)
{
    picoRTOS_priority_t prio = (picoRTOS_priority_t)0;

    for (; prio < (picoRTOS_priority_t)CONFIG_TASK_COUNT; prio++)
        if (TASK_BY_PID(prio).state == TASK_STATE_DISABLED)
            break;

    /* no slot available */
    picoRTOS_assert(prio < (picoRTOS_priority_t)TASK_IDLE_PRIO,
                    return (picoRTOS_priority_t)-1);

    return prio;
}

picoRTOS_priority_t picoRTOS_get_last_available_priority(void)
{
    picoRTOS_priority_t prio = (picoRTOS_priority_t)(TASK_IDLE_PRIO - 1);

    for (; prio != 0; prio--)
        if (TASK_BY_PID(prio).state == TASK_STATE_DISABLED)
            break;

    /* no slot available: overflow */
    picoRTOS_assert(prio != (picoRTOS_priority_t)-1,
                    return (picoRTOS_priority_t)-1);

    return prio;
}

void picoRTOS_start(void)
{
    arch_init();
    picoRTOS.flags |= F_RUNNING;
    arch_start_first_task(TASK_BY_PID(TASK_IDLE_PID).sp);
}

/*@noreturn@*/ static void fatal(void)
{
    for (;;)
        arch_break();
}

void picoRTOS_run(bool run)
{
    picoRTOS_assert((picoRTOS.flags & F_RUNNING) != 0, fatal());
    if (run) arch_resume();
    else arch_suspend();
}

void picoRTOS_sleep(picoRTOS_tick_t delay)
{
    arch_syscall(SYSCALL_SLEEP, &delay);
}

void picoRTOS_sleep_until(picoRTOS_tick_t *ref, picoRTOS_tick_t period)
{
    picoRTOS_assert(period > 0, return );
    struct syscall_sleep_until sc = { *ref, period };

    arch_syscall(SYSCALL_SLEEP_UNTIL, &sc);
    *ref = sc.ref; /* update ref */
}

void picoRTOS_kill(int errnum)
{
    arch_syscall(SYSCALL_KILL, &errnum);
}

picoRTOS_pid_t picoRTOS_self(void)
{
    picoRTOS_pid_t pid = 0;

    arch_syscall(SYSCALL_GETPID, &pid);
    return pid;
}

picoRTOS_tick_t picoRTOS_get_tick(void) /*@modifies nothing@*/
{
    picoRTOS_tick_t tick = 0;

    arch_syscall(SYSCALL_GETTICK, &tick);
    return tick;
}

/* SYSCALLS */

/*@exposed@*/
static struct picoRTOS_task_core *
syscall_switch_context(struct picoRTOS_task_core *task)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while (deadlock-- != 0) {
        /* choose next task to run */
        do {
            picoRTOS.index++;
            picoRTOS_assert(picoRTOS.index < (picoRTOS_pid_t)TASK_COUNT, fatal());
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

    picoRTOS_assert(deadlock != -1, fatal());
    return task;
}

/*@exposed@*/ static struct picoRTOS_task_core *
syscall_kill(struct picoRTOS_task_core *task)
{
    task->state = TASK_STATE_DISABLED;
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
        picoRTOS_assert_void(false);    /* force break in debug */
        return syscall_kill(task);      /* kill out-of-control task */
    }

    return task; /* don't switch */
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

picoRTOS_stack_t *picoRTOS_syscall(picoRTOS_stack_t *sp, syscall_t syscall, void *priv)
{
    struct picoRTOS_task_core *task = &TASK_CURRENT();

    picoRTOS_assert((picoRTOS.flags & F_RUNNING) != 0, fatal());
    picoRTOS_assert(sp >= task->stack_bottom, return syscall_kill(task)->sp);
    picoRTOS_assert(sp < task->stack_top, return syscall_kill(task)->sp);
    picoRTOS_assert(priv != NULL, return syscall_kill(task)->sp);

    /* store current sp */
    task->sp = sp;

    switch (syscall) {
    /* OS-related syscalls */
    /* task-related syscalls */
    case SYSCALL_SLEEP: return syscall_sleep(task, *(picoRTOS_tick_t*)priv)->sp;
    case SYSCALL_GETTICK: return syscall_get_tick(task, (picoRTOS_tick_t*)priv)->sp;
    case SYSCALL_SLEEP_UNTIL: return syscall_sleep_until(task, (struct syscall_sleep_until*)priv)->sp;
    case SYSCALL_GETPID: return syscall_get_pid(task, (picoRTOS_pid_t*)priv)->sp;
    case SYSCALL_KILL: /*@fallthrough@*/
    case SYSCALL_SEGFAULT: return syscall_kill(task)->sp;
    default: break;
    }

    /* unhandled syscall */
    picoRTOS_assert_void(false);
    return syscall_kill(task)->sp;
}

/* TICK */

picoRTOS_stack_t *picoRTOS_tick(picoRTOS_stack_t *sp)
{
    struct picoRTOS_task_core *task = &TASK_CURRENT();

    picoRTOS_assert(sp >= task->stack_bottom, task->state = TASK_STATE_DISABLED);
    picoRTOS_assert(sp < task->stack_top, task->state = TASK_STATE_DISABLED);

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
    }

    /* refresh current task pointer */
    task = &TASK_CURRENT();

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

void picoRTOS_invalidate_dcache(const void *addr, size_t n)
{
    picoRTOS_assert(n > 0, return );
    arch_invalidate_dcache(addr, n);
}

void picoRTOS_flush_dcache(const void *addr, size_t n)
{
    picoRTOS_assert(n > 0, return );
    arch_flush_dcache(addr, n);
}

void picoRTOS_mpu_add_region(const void *addr, size_t n, unsigned mode)
{
    /* fully unsupported */
    /*@i@*/ (void)addr;
    /*@i@*/ (void)n;
    /*@i@*/ (void)mode;
}

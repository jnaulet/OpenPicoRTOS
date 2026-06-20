#ifndef PICORTOS_H
#define PICORTOS_H

#include <stddef.h>
#include <stdbool.h>

#include "picoRTOS_types.h"
#include <generated/autoconf.h>

/* TASKS */
typedef void (*picoRTOS_task_fn)(void*);

struct picoRTOS_task {
    /*@temp@*/ picoRTOS_task_fn fn;
    /*@temp@*/ void *priv;
    /*@temp@*/ picoRTOS_stack_t *stack;
    size_t stack_count;
};

void picoRTOS_task_init(/*@out@*/ struct picoRTOS_task *task,
                        picoRTOS_task_fn fn, /*@null@*/ void *priv,
                        /*@reldef@*/ picoRTOS_stack_t *stack,
                        size_t stack_count);

/**§ p
 * ### TASK MANAGEMENT
 */

/** p
 * **PICORTOS_STACK_COUNT**(<ins>x</ins>);
 * > Computes stack count from array <ins>x</ins>
 */
#define PICORTOS_STACK_COUNT(x) (sizeof(x) / sizeof(picoRTOS_stack_t))

/* PRE-SCHEDULER */

void picoRTOS_init(void);
void picoRTOS_add_task(struct picoRTOS_task *task,
                       picoRTOS_priority_t prio);                       /* register task */
picoRTOS_priority_t picoRTOS_get_next_available_priority(void);         /* get next free priority slot */
picoRTOS_priority_t picoRTOS_get_last_available_priority(void);         /* get last free priority slot */
/*@maynotreturn@*/ void picoRTOS_start(void);                           /* starts picoRTOS */
void picoRTOS_run(bool run);                                            /* suspends/resumes picoRTOS */

/* POST-SCHEDULER */
void picoRTOS_sleep(picoRTOS_tick_t delay);                             /* put current task to sleep */
void picoRTOS_sleep_until(picoRTOS_tick_t *ref,                         /* put current task to sleep until */
                          picoRTOS_tick_t period);

/**§ u
 * ### SCHEDULER MANAGEMENT
 */

/** u
 * **picoRTOS_schedule**();
 * > Puts the current task to sleep until next tick.<br>
 * > Strictly equivalent to `picoRTOS_sleep(1)`
 */
#define picoRTOS_schedule() picoRTOS_sleep((picoRTOS_tick_t)1)

/** u
 * **picoRTOS_postpone**();
 * > Puts the current task back in the scheduler's FIFO [(more info)](SCHEDULERS.md)<br>
 * > Strictly equivalent to `picoRTOS_sleep(0)`
 */
#define picoRTOS_postpone() picoRTOS_sleep((picoRTOS_tick_t)0)

/** u
 * **picoRTOS_suspend**();
 * > Suspends the scheduling. Typical use is critical sections.<br>
 * > Strictly equivalent to `picoRTOS_run(false)`
 */
#define picoRTOS_suspend() picoRTOS_run(false)

/** u
 * **picoRTOS_suspend**();
 * > Resumes the scheduling. Typical use is critical sections.<br>
 * > Strictly equivalent to `picoRTOS_run(true)`
 */
#define picoRTOS_resume() picoRTOS_run(true)

/*@noreturn@*/ void picoRTOS_kill(int errnum);              /* kills the current task */
picoRTOS_pid_t picoRTOS_self(void);                         /* gets the current thread pid */
/*@unused@*/ picoRTOS_tick_t picoRTOS_get_tick(void) /*@*/; /* get current tick */

/* TIME MANAGEMENT */

/**§ u
 * ### TIME MANAGEMENT
 */

/** u
 * **PICORTOS_DELAY_SEC**(<ins>x</ins>);
 * > Converts <ins>x</ins> seconds in picoRTOS_tick_t
 */
#define PICORTOS_DELAY_SEC(x) (picoRTOS_tick_t)((x) * CONFIG_TICK_HZ)

/** u
 * **PICORTOS_DELAY_MSEC**(<ins>x</ins>);
 * > Converts <ins>x</ins> milliseconds in picoRTOS_tick_t
 */
#define PICORTOS_DELAY_MSEC(x) (picoRTOS_tick_t)((((x) * CONFIG_TICK_HZ) + 999) / 1000)

/** u
 * **PICORTOS_DELAY_MSEC**(<ins>x</ins>);
 * > Converts <ins>x</ins> microseconds in picoRTOS_tick_t
 */
#define PICORTOS_DELAY_USEC(x) (picoRTOS_tick_t)((((x) * CONFIG_TICK_HZ) + 999999) / 1000000)

/** u
 * **PICORTOS_DELAY_ELAPSED**(<ins>ref</ins>, <ins>x</ins>);
 * > Checks if delay <ins>ref</ins> + <ins>x</ins> has elapsed
 */
#define PICORTOS_DELAY_ELAPSED(ref, x) !((picoRTOS_get_tick() - (ref)) < (x))

typedef void (*picoRTOS_isr_fn)(/*@null@*/ void*);

void picoRTOS_register_interrupt(picoRTOS_irq_t irq,
                                 picoRTOS_isr_fn fn,
                                 /*@null@*/ void *priv);

void picoRTOS_set_interrupt(picoRTOS_irq_t irq, bool active);

/**§ u
 * ### INTERRUPT MANAGEMENT
 */

/** u
 * **picoRTOS_enable_interrupt**(<ins>irq</ins>);
 * > Enables an interrupt on the system.<br>
 * > Strictly equivalent to `picoRTOS_set_interrupt(irq, true)`
 */
#define picoRTOS_enable_interrupt(irq) picoRTOS_set_interrupt(irq, true)

/** u
 * **picoRTOS_disable_interrupt**(<ins>irq</ins>);
 * > Disables an interrupt on the system.<br>
 * > Strictly equivalent to `picoRTOS_set_interrupt(irq, false)`
 */
#define picoRTOS_disable_interrupt(irq) picoRTOS_set_interrupt(irq, false)

/* CACHE MANAGEMENT */

void picoRTOS_invalidate_dcache(const void *addr, size_t n);
void picoRTOS_flush_dcache(const void *addr, size_t n);

/* MEMORY PROTECTION */

void picoRTOS_mpu_add_region(const void *addr, size_t n, unsigned mode);

/**§ u
 * ### ASSERTS
 */

#ifndef NDEBUG
/** u
 * **picoRTOS_dbgbreak**();
 * > Throws a debug exception, ignored if -DNDEBUG
 */
# define picoRTOS_dbgbreak() arch_break()
#else
# define picoRTOS_dbgbreak()
#endif

/** u
 * **picoRTOS_assert**(<ins>x</ins>, <ins>or_else</ins>);
 * > If predicate <ins>x</ins> is true, do nothing.<br>
 * > If predicate <ins>x</ins> is false, execute <ins>or_else</ins>.
 * 
 * >[!NOTE]
 * > This will throw a debug exception unless NDEBUG is set
 */
# define picoRTOS_assert(x, or_else)            \
    if (!(x)) {                                 \
        picoRTOS_core_sef(x);                   \
        picoRTOS_dbgbreak();                    \
        { or_else; }                            \
    }

/** u
 * **picoRTOS_assert**(<ins>x</ins>, <ins>or_else</ins>);
 * > If predicate <ins>x</ins> is true, do nothing.<br>
 * > If predicate <ins>x</ins> is false, throw a debug exeption, unless
 * > NDEBUG is set.
 */
# define picoRTOS_assert_void(x)                \
    if (!(x)){                                  \
        picoRTOS_core_sef(x);                   \
        picoRTOS_dbgbreak();                    \
    }

#endif

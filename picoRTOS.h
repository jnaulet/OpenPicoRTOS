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

/* Group: picoRTOS scheduler API */

/* Macro: PICORTOS_STACK_COUNT(x)
 * Computes stack count from array
 *
 * Parameters:
 *  x - a picoRTOS_stack_t array
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

/* Macro: picoRTOS_schedule()
 * Puts the current task to sleep until next tick
 */
#define picoRTOS_schedule() picoRTOS_sleep((picoRTOS_tick_t)1)

/* Macro: picoRTOS_postpone()
 * Puts the current task back in the scheduler's FIFO (don't wait for next tick)
 */
#define picoRTOS_postpone() picoRTOS_sleep((picoRTOS_tick_t)0)

/* Macro: picoRTOS_suspend()
 * Suspends the scheduling. Typical use is critical sections
 */
#define picoRTOS_suspend() picoRTOS_run(false)

/* Macro: picoRTOS_resume()
 * Resumes the scheduling. Typical use is critical sections
 */
#define picoRTOS_resume() picoRTOS_run(true)

/*@noreturn@*/ void picoRTOS_kill(int errnum);              /* kills the current task */
picoRTOS_pid_t picoRTOS_self(void);                         /* gets the current thread pid */
/*@unused@*/ picoRTOS_tick_t picoRTOS_get_tick(void) /*@*/; /* get current tick */

/* TIME MANAGEMENT */

/* Macro: PICORTOS_DELAY_SEC(x)
 * Converts seconds to picoRTOS_tick_t
 *
 * Parameters:
 *  x - a value in seconds
 */
#define PICORTOS_DELAY_SEC(x) (picoRTOS_tick_t)((x) * CONFIG_TICK_HZ)

/* Macro: PICORTOS_DELAY_MSEC(x)
 * Converts milliseconds to picoRTOS_tick_t (rounded to closest upper tick)
 *
 * Parameters:
 *  x - a value in milliseconds
 */
#define PICORTOS_DELAY_MSEC(x) (picoRTOS_tick_t)((((x) * CONFIG_TICK_HZ) + 999) / 1000)

/* Macro: PICORTOS_DELAY_USEC(x)
 * Converts microseconds in picoRTOS_tick_t (rounded to closest upper tick)
 *
 * Parameters:
 *  x - a value in microseconds
 */
#define PICORTOS_DELAY_USEC(x) (picoRTOS_tick_t)((((x) * CONFIG_TICK_HZ) + 999999) / 1000000)

/* Macro: PICORTOS_DELAY_ELAPSED(ref, x)
 * Checks if a specific delay has passed since ref
 *
 * Parameters:
 *  ref - the reference time/tick
 *  x - a delay in ticks
 */
#define PICORTOS_DELAY_ELAPSED(ref, x) !((picoRTOS_get_tick() - (ref)) < (x))

/* INTERRUPT MANAGEMENT */

typedef void (*picoRTOS_isr_fn)(/*@null@*/ void*);

void picoRTOS_register_interrupt(picoRTOS_irq_t irq,
                                 picoRTOS_isr_fn fn,
                                 /*@null@*/ void *priv);

void picoRTOS_set_interrupt(picoRTOS_irq_t irq, bool active);

/* Macro: picoRTOS_enable_interrupt
 * Enables an interrupt on the system
 *
 * Parameters:
 *  irq - The irq number to enable
 */
#define picoRTOS_enable_interrupt(irq) picoRTOS_set_interrupt(irq, true);

/* Macro: picoRTOS_disable_interrupt
 * Disables an interrupt on the system
 *
 * Parameters:
 *  irq - The irq number to enable
 */
#define picoRTOS_disable_interrupt(irq) picoRTOS_set_interrupt(irq, false);

/* CACHE MANAGEMENT */

void picoRTOS_invalidate_dcache(const void *addr, size_t n);
void picoRTOS_flush_dcache(const void *addr, size_t n);

/* MEMORY PROTECTION */

void picoRTOS_mpu_add_region(const void *addr, size_t n, unsigned mode);

/* Group: picoRTOS assert API */

#ifndef NDEBUG
/* Macro: picoRTOS_dbgbreak()
 * Throws a debug exception, ignored if -DNDEBUG */
# define picoRTOS_dbgbreak() arch_break()
#else
# define picoRTOS_dbgbreak()
#endif

/* Macro: picoRTOS_assert(x, or_else)
 * Returns x, throws a debug exception & executes or_else if x is false,
 * unless -DNDEBUG */
# define picoRTOS_assert(x, or_else)            \
    if (!(x)) {                                 \
        picoRTOS_core_sef(x);                   \
        picoRTOS_dbgbreak();                    \
        { or_else; }                            \
    }

/* Macro: picoRTOS_assert_void(x)
 * Throws a debug exception if x is false, unless -DNDEBUG */
# define picoRTOS_assert_void(x)                \
    if (!(x)){                                  \
        picoRTOS_core_sef(x);                   \
        picoRTOS_dbgbreak();                    \
    }

#endif

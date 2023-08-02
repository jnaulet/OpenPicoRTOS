#ifndef PICORTOS_H
#define PICORTOS_H

#include <stddef.h>
#include <stdbool.h>

#include "picoRTOS_types.h"

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

/* SCHEDULER */
void picoRTOS_init(void);
void picoRTOS_add_task(struct picoRTOS_task *task,
                       picoRTOS_priority_t prio);                       /* register task */
picoRTOS_priority_t picoRTOS_get_next_available_priority(void);         /* get next free priority slot */
picoRTOS_priority_t picoRTOS_get_last_available_priority(void);         /* get last free priority slot */

/*@maynotreturn@*/ void picoRTOS_start(void);                           /* starts picoRTOS */
void picoRTOS_suspend(void);                                            /* suspends the scheduling */
void picoRTOS_resume(void);                                             /* resumes the scheduling */

void picoRTOS_schedule(void);                                           /* move to next task */
void picoRTOS_sleep(picoRTOS_tick_t delay);                             /* put current task to sleep */
void picoRTOS_sleep_until(picoRTOS_tick_t *ref,                         /* put current task to sleep until */
                          picoRTOS_tick_t period);

/*@noreturn@*/ void picoRTOS_kill(void);                            /* kills the current task */

picoRTOS_pid_t picoRTOS_self(void);                                 /* gets the current thread priority */
/*@unused@*/ picoRTOS_tick_t picoRTOS_get_tick(void);               /* get current tick */

/* TIME MANAGEMENT */

/* Macro: PICORTOS_DELAY_SEC(x)
 * Converts seconds to picoRTOS_tick_t
 *
 * Parameters:
 *  x - a value in seconds
 */
#define PICORTOS_DELAY_SEC(x) (picoRTOS_tick_t)((x) * CONFIG_TICK_HZ)

/* Macro: PICORTOS_DELAY_MSEC(x)
 * Converts milliseconds to picoRTOS_tick_t
 *
 * Parameters:
 *  x - a value in milliseconds
 */
#define PICORTOS_DELAY_MSEC(x) (picoRTOS_tick_t)(((x) * CONFIG_TICK_HZ) / 1000)

/* Macro: PICORTOS_DELAY_USEC(x)
 * Converts microseconds in picoRTOS_tick_t
 *
 * Parameters:
 *  x - a value in microseconds
 */
#define PICORTOS_DELAY_USEC(x) (picoRTOS_tick_t)(((x) * CONFIG_TICK_HZ) / 1000000)

/* INTERRUPT MANAGEMENT */

typedef void (*picoRTOS_isr_fn)(/*@null@*/ void*);

void picoRTOS_register_interrupt(picoRTOS_irq_t irq,
                                 picoRTOS_isr_fn fn,
                                 /*@null@*/ void *priv);

void picoRTOS_enable_interrupt(picoRTOS_irq_t irq);
void picoRTOS_disable_interrupt(picoRTOS_irq_t irq);

/* CACHE MANAGEMENT */

#ifndef DEVICE_HAS_SOFTWARE_CACHE_COHERENCY

/* Macro: picoRTOS_invalidate_dcache(addr, n)
 * If DEVICE_HAS_SOFTWARE_CACHE_COHERENCY, invalidates cache at addr up to addr+n,
 * otherwise, do nothing (hardware cache snooping)
 *
 * Parameters:
 *  addr - The memory base address to invalidate
 *  n - The size (in bytes) to invalidate
 */
# define picoRTOS_invalidate_dcache(addr, n)

/* Macro: picoRTOS_flush_dcache(addr, n)
 * If DEVICE_HAS_SOFTWARE_CACHE_COHERENCY, flushes cache at addr up to addr+n,
 * otherwise, do nothing (hardware cache snooping)
 *
 * Parameters:
 *  addr - The memory base address to flush to
 *  n - The size (in bytes) to flush
 */
# define picoRTOS_flush_dcache(addr, n)
#else
# define picoRTOS_invalidate_dcache(addr, n) arch_invalidate_dcache((addr), (n))
# define picoRTOS_flush_dcache(addr, n)      arch_flush_dcache((addr), (n))
#endif

/* Group: picoRTOS assert API */

#if !defined(NDEBUG)
/* Macro: picoRTOS_break()
 * Throws a debug exception, ignored if -DNDEBUG */
# define picoRTOS_break() arch_break()
/* Macro: picoRTOS_assert(x, or_else)
 * Returns x, throws a debug exception & executes or_else if x is false,
 * unless -DNDEBUG */
# define picoRTOS_assert(x, or_else)            \
  if (!(x)) {                                   \
    picoRTOS_break(); /*@notreached@*/          \
    or_else;                                    \
  }
/* Macro: picoRTOS_assert_void(x)
 * Throws a debug exception if x is false, unless -DNDEBUG */
# define picoRTOS_assert_void(x) if (!(x)) picoRTOS_break()
/* Macro: picoRTOS_assert_fatal(x, or_else)
 * Returns x, throws a debug exception & executes or_else if x is false,
 * stalls the system if -DNDEBUG */
# define picoRTOS_assert_fatal(x, or_else) picoRTOS_assert(x, or_else)
/* Macro: picoRTOS_assert_void_fatal(x)
 * Throws a debug exception if x is false, stalls the system if -DNDEBUG */
# define picoRTOS_assert_void_fatal(x) picoRTOS_assert_void(x)
#else
# define picoRTOS_break() for (;;) {}
# define picoRTOS_assert(x, or_else) if (!(x)) { or_else; }
# define picoRTOS_assert_void(x) {}
# define picoRTOS_assert_fatal(x, or_else)      \
  if (!(x)) {                                   \
    picoRTOS_suspend();                         \
    picoRTOS_break(); /*@notreached@*/          \
    or_else;                                    \
  }
# define picoRTOS_assert_void_fatal(x) \
  if (!(x)) {                          \
    picoRTOS_suspend();                \
    picoRTOS_break();                  \
  }
#endif

#endif

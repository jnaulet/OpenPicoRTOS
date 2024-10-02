#ifndef PICORTOS_PORT_H
#define PICORTOS_PORT_H

#include <stddef.h>
#include <stdbool.h>

#include "picoRTOS_types.h"

/* Enum: picoRTOS syscalls
 *
 * SYSCALL_SLEEP - A task asked to sleep
 * SYSCALL_SLEEP_UNTIL - A task asked to sleep until a ref + period deadline
 * SYSCALL_KILL - A task committed suicide
 * SYSCALL_SWITCH_CONTEXT - A task asked to be postponed to the next tick
 */
typedef enum {
    SYSCALL_SLEEP,
    SYSCALL_SLEEP_UNTIL,
    SYSCALL_KILL,
    SYSCALL_SWITCH_CONTEXT,
    SYSCALL_COUNT
} syscall_t;

struct syscall_sleep {
    picoRTOS_tick_t delay;
};

struct syscall_sleep_until {
    picoRTOS_tick_t ref;
    picoRTOS_tick_t period;
};

struct syscall_kill {
    picoRTOS_pid_t pid;
};

/* Function: picoRTOS_syscall
 * Executes a syscall
 *
 * This MUST be called from arch_syscall
 *
 * Parameters:
 *  sp - The current task's stack pointer
 *  syscall - the syscall number
 *  priv - Internal syscall parameter (can be NULL)
 *
 * Returns:
 *  The task stack you have to switch to (context restoration)
 */
extern /*@exposed@*/ /*@null@*/
picoRTOS_stack_t *picoRTOS_syscall(picoRTOS_stack_t *sp,
                                   syscall_t syscall,
                                   /*@null@*/ void *priv);
/* Function: picoRTOS_tick
 * Executes a tick increment
 *
 * This MUST be called from you main tick timer interrupt
 *
 * Parameters:
 *  sp - The current task stack pointer (after context save)
 *
 * Returns:
 *  The task stack pointer you have to switch to (context restoration)
 */
extern /*@exposed@*/ /*@null@*/
picoRTOS_stack_t *picoRTOS_tick(picoRTOS_stack_t *sp);

typedef void (*arch_entry_point_fn)(void*);     /* entry point */
typedef void (*arch_isr_fn)(void*);             /* interrupt service routine */

#if defined(NDEBUG) || !defined(arch_break)
# undef arch_break
# define arch_break() for (;;) {}
#endif

/* Macro: arch_assert(x, or_else)
 * Throws a debug exception & execute or_else if x is false, stalls if -DNDEBUG
 *
 * Parameters:
 *  x - predicate
 *  or_else - code to execute if predicate is false
 *
 * Returns:
 * The value of the predicate
 */
#define arch_assert(x, or_else) if (!(x)) { arch_break(); /*@notreached@*/ or_else; }

/* Macro: arch_assert_void(x)
 * Throws a debug exception if x is false, stalls if -DNDEBUG
 *
 * Parameters:
 *  x - predicate
 */
#define arch_assert_void(x) if (!(x)) arch_break()

/* Function: arch_init
 * Architecture port initialization function
 *
 * This is where you setup your tick timer, interrupts, etc
 */
extern void arch_init(void);

/* Function: arch_suspend
 * Suspends the scheduling
 *
 * See also:
 *  <picoRTOS_suspend>
 */
extern void arch_suspend(void);

/* Function: arch_resume
 * Resumes the scheduling
 *
 * See also:
 *  <picoRTOS_resume>
 */
extern void arch_resume(void);

/* Function: arch_prepare_stack
 * Prepares a task's stack for context restoration
 *
 * This is where you arrange each tasks's stack to allow its first context restoration
 * This structure must match you context restoration procedure
 *
 * Parameters:
 *  stack - A pointer to the stack to prepare
 *  stack_count - The size (in elements) of the stack
 *  fn - The task/thread enytry point
 *  priv - The task/thread private parameter
 *
 * Returns:
 * A pointer to the first element of the newly prepared stack
 */
extern /*@temp@*/
picoRTOS_stack_t *arch_prepare_stack(/*@returned@*/ picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     /*@null@*/ void *priv);

/* Function: arch_start_first_task
 * Starts the first task on the system (idle) and bootstraps the scheduler
 *
 * Parameters:
 *  sp - The stack pointer of the idle task
 */
extern /*@noreturn@*/ void arch_start_first_task(picoRTOS_stack_t *sp);

/* Function: arch_syscall
 * Syscall port function
 *
 * This function will be called by picoRTOS every time someone calls picoRTOS_schedule,
 * picoRTOS_sleep or picoRTOS_sleep_until and MUST call picoRTOS_syscall
 *
 * As syscalls switch contexts, you want this function to trigger an interrupt, unless
 * you cannot (see AVR ports)
 *
 * Parameters:
 *  syscall - The syscall to execute
 *  priv - The syscall parameter to pass to picoRTOS_syscall (can be NULL)
 *
 * See also:
 *  <picoRTOS_syscall>
 */
extern void arch_syscall(syscall_t syscall, /*@null@*/ void *priv);

/* Function: arch_idle
 * The default idle function/task
 *
 * Ths function must provide an infinite loop that puts the CPU in IDLE mode
 *
 * Parameters:
 *  null - SHOULD always be NULL
 */
extern /*@noreturn@*/ void arch_idle(/*@null@*/ const void *null);

/* ARCH: ATOMIC OPS */

/* Function: arch_test_and_set
 * Atomic test and set operation
 *
 * Parameters:
 *  ptr - A pointer to a picoRTOS_atomic_t object
 *
 * Returns:
 *  0 in case of success, 1 otherwise
 */
extern /*@unused@*/ picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr);

/* Function: arch_compare_and_swap
 * Atomic compare and swap operation,
 * will swap the value of *var to new if it's equal to old
 *
 * Parameters:
 *  var - A pointer to the picoRTOS_atomic_t to swap
 *  old - The current value of *var
 *  new - The value to change *var to
 *
 * Returns:
 *  The previous value of *var (old) if success, anything else otherwise (preferably new)
 */
extern /*@unused@*/ picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                            picoRTOS_atomic_t old,
                                                            picoRTOS_atomic_t val);

/* ARCH: INTERRUPTS (optional) */

/* Function: arch_register_interrupt
 * Registers an interrupt on the system
 *
 * See also:
 *  <picoRTOS_register_interrupt>
 */
extern /*@unused@*/ void arch_register_interrupt(picoRTOS_irq_t irq,
                                                 arch_isr_fn fn,
                                                 /*@null@*/ void *priv);

/* Function: arch_enable_interrupt
 * Enables an irq
 *
 * Parameters:
 *  irq - The irq to enable
 *
 * See also:
 *  <picoRTOS_register_interrupt>
 */
extern /*@unused@*/ void arch_enable_interrupt(picoRTOS_irq_t irq);

/* Function: arch_disable_interrupt
 * Disables an irq
 *
 * Parameters:
 *  irq - The irq to disable
 *
 * See also:
 *  <picoRTOS_disable_interrupt>
 */
extern /*@unused@*/ void arch_disable_interrupt(picoRTOS_irq_t irq);

/* STATS */

typedef enum {
    ARCH_COUNTER_CURRENT,
    ARCH_COUNTER_SINCE,
    ARCH_COUNTER_COUNT
} arch_counter_t;

/* Function: arch_counter
 * Provides the current cpu counter value
 *
 * This value should be between (0-PICORTOS_CYCLES_PER_TICK) and will
 * be used to populate the watermarks in picoRTOS_core::task[n]::stat
 *
 * Parameters:
 *  counter - The type of counter we want
 *  t - The counter value to operate on (only valid if ARCH_COUNTER_SINCE)
 *
 * Returns:
 *  The value of the counter in picoRTOS_cycles_t
 */
extern picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t);

/* CACHES */

/* Function: arch_invalidate_dcache
 * Invalidates one or more cache lines
 *
 * Parameters:
 *  addr - A cacheable address in RAM
 *  n - The number of bytes to invalidate
 */
extern /*@external@*/ void arch_invalidate_dcache(void *addr, size_t n);

/* Function: arch_flush_dcache
 * Flushes one or more cache lines
 *
 * Parameters:
 *  addr - A cacheable address in RAM
 *  n - The number of bytes to flush
 */
extern /*@external@*/ void arch_flush_dcache(void *addr, size_t n);

/* CLOCKS */

/* Function: arch_set_clock_frequency
 * Sets the currect CPU input frequency
 *
 * Parameters:
 *  freq - The CPU input frequency (in hertz)
 */
extern /*@external@*/ void arch_set_clock_frequency(unsigned long freq);

/* Function: arch_delay_us
 * Busy waits for at least n microseconds
 *
 * Parameters:
 *  n - The number of microseconds to wait
 */
extern /*@external@*/ void arch_delay_us(unsigned long n);

#endif

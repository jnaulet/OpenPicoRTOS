#ifndef PICORTOS_PORT_H
#define PICORTOS_PORT_H

#include "picoRTOS_types.h"

/* Enum: picoRTOS syscalls
 *
 * PICORTOS_SYSCALL_SLEEP - A task asked to sleep
 * PICORTOS_SYSCALL_KILL - A task committed suicide
 * PICORTOS_SYSCALL_SWITCH_CONTEXT - A task asked to be postponed to the next tick
 */
typedef enum {
    PICORTOS_SYSCALL_SLEEP,
    PICORTOS_SYSCALL_KILL,
    PICORTOS_SYSCALL_SWITCH_CONTEXT,
    PICORTOS_SYSCALL_COUNT
} picoRTOS_syscall_t;

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
/*@exposed@*/ /*@null@*/
picoRTOS_stack_t *picoRTOS_syscall(picoRTOS_stack_t *sp,
                                   picoRTOS_syscall_t syscall,
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
/*@exposed@*/ /*@null@*/
picoRTOS_stack_t *picoRTOS_tick(picoRTOS_stack_t *sp);  /* update tick */

/* Function: arch_init
 * Architecture port initialization function
 *
 * This is where you setup your tick timer, interrupts, etc
 */
extern void arch_init(void);    /* init architecture */

/* Function: arch_suspend
 * Suspends the scheduling
 *
 * See also:
 *  <picoRTOS_suspend>
 */
extern void arch_suspend(void); /* suspends tick */
/* Function: arch_resume
 * Resumes the scheduling
 *
 * See also:
 *  <picoRTOS_resume>
 */
extern void arch_resume(void);  /* resumes tick */

/* Function: arch_prepare_stack
 * Prepares a task's stack for context restoration
 *
 * This is where you arrange each tasks's stack to allow its first context restoration
 * This structure must match you context restoration procedure
 *
 * Parameters:
 *  task - A pointer to the task to prepare the stack for
 *
 * Returns:
 * A pointer to the first element of the newly prepared stack
 */
/*@temp@*/ extern picoRTOS_stack_t * arch_prepare_stack(struct picoRTOS_task *task);

/* Function: arch_start_first_task
 * Starts the first task on the system (idle) and bootstraps the scheduler
 *
 * Parameters:
 *  sp - The stack pointer of the idle task
 */
/*@noreturn@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);

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
extern void arch_syscall(picoRTOS_syscall_t syscall, /*@null@*/ void *priv);

/* Function: arch_idle
 * The default idle function/task
 *
 * Ths function must provide an infinite loop that puts the CPU in IDLE mode
 *
 * Parameters:
 *  null - SHOULD always be NULL
 */
/*@noreturn@*/ extern void arch_idle(/*@null@*/ void *null);

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
/*@external@*/ extern picoRTOS_atomic_t
arch_test_and_set(picoRTOS_atomic_t *ptr);    /* atomic test and set */
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
/*@external@*/ extern picoRTOS_atomic_t
arch_compare_and_swap(picoRTOS_atomic_t *var,
                      picoRTOS_atomic_t old,
                      picoRTOS_atomic_t val); /* atomic compare and swap */

/* ARCH: INTERRUPTS (optional) */

/* Function: arch_register_interrupt
 * Registers an interrupt on the system
 *
 * See also:
 *  <picoRTOS_register_interrupt>
 */
/*@external@*/ extern void
arch_register_interrupt(picoRTOS_irq_t irq,
                        picoRTOS_isr_fn fn,
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
/*@external@*/ extern void arch_enable_interrupt(picoRTOS_irq_t irq);

/* Function: arch_disable_interrupt
 * Disables an irq
 *
 * Parameters:
 *  irq - The irq to disable
 *
 * See also:
 *  <picoRTOS_disable_interrupt>
 */
/*@external@*/ extern void arch_disable_interrupt(picoRTOS_irq_t irq);

/* STATS */

/* Function: arch_counter
 * Provides the current value of the CPU timer for internal statistics
 * and monitoring
 *
 * This value should be between (0-PICORTOS_CYCLES_PER_TICK) and will
 * be used to populate the watermarks in picoRTOS_core::task[n]::stat
 *
 * Returns:
 *  The value of the counter in picoRTOS_cycles_t
 */
picoRTOS_cycles_t arch_counter(void);

/* CACHES */

/* Function: arch_invalidate_dcache
 * Invalidates one or more cache lines
 *
 * Parameters:
 *  addr - A cacheable address in RAM
 *  n - The number of bytes to invalidate
 */
/*@external@*/ extern void arch_invalidate_dcache(void *addr, size_t n);

/* Function: arch_flush_dcache
 * Flushes one or more cache lines
 *
 * Parameters:
 *  addr - A cacheable address in RAM
 *  n - The number of bytes to flush
 */
/*@external@*/ extern void arch_flush_dcache(void *addr, size_t n);

#endif

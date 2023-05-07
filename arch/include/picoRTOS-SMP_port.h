#ifndef PICORTOS_SMP_PORT_H
#define PICORTOS_SMP_PORT_H

#include "picoRTOS_port.h"
#include "picoRTOS_types.h"

/* Function: arch_smp_init
 * SMP Architecture port initialization function
 *
 * This is where you setup your SMP port, usually calls the single-core arch_init
 * but not always
 *
 * See also:
 *  <arch_init>
 */
extern void arch_smp_init(void);

/* Function: arch_core_init
 * Auxiliary core startup function
 *
 * This is heavily architecture-dependent, there are recommandations, though,
 * like preferably using the auxiliary core stack to pass parameters
 *
 * Parameters:
 *  core - The core number to start (1 -> CONFIG_SMP_CORES-1)
 *  stack - A pointer to the top of the auxiliary core main stack
 *  stack_count - The auxiliary core main stack size in picoRTOS_stack_t elements
 *  sp - The auxiliary core 'idle' task prepared stack (ready for context restoration)
 */
extern void arch_core_init(picoRTOS_core_t core,
                           picoRTOS_stack_t *stack,
                           size_t stack_count,
                           picoRTOS_stack_t *sp);

/* Function: arch_core
 * Provides the current running core identifier/index
 *
 * Returns:
 * The current running core index
 */
extern picoRTOS_core_t arch_core(void);

/* Function: arch_spin_lock
 * Locks the SMP spinlock
 *
 * To ensure core synchronization, picoRTOS uses a spinlock, preferably a hardware one
 * You need to provide it, as this is architecure-dependent once again
 *
 * Remarks:
 * This MUST make CONFIG_DEADLOCK_COUNT attempts to acquire the spinlock and declare deadlock
 * if it fails (debug exception)
 */
extern void arch_spin_lock(void);

/* Function: arch_spin_unlock
 * Unlocks the SMP spinlock
 */
extern void arch_spin_unlock(void);

/* Function: arch_propagate_tick
 * Propagates the system tick interrupt to auxiliary cores
 *
 * This function is called by the main core evey time it increments the tick and is used to
 * trigger the auxiliary cores tick interrupt
 */
extern void arch_propagate_tick(void);

/* Function: arch_acknowledge_tick
 * Acknowledge the system tick interrupt as an auxiliary core
 *
 * This function is called by picoRTOS_tick() when it's run by an auxiliary core
 */
extern void arch_acknowledge_tick(void);

/* INTERRUPT MANAGEMENT (optional) */

/* Function: arch_smp_enable_interrupt
 * Enables an interrupt on a specific core or set of cores
 *
 * Parameters:
 *  irq - The irq to enable
 *  core_mask - The core mask, one bit per core
 *
 * See also:
 *  <picoRTOS_SMP_enable_interrupt>
 */
extern /*@unused@*/ void arch_smp_enable_interrupt(picoRTOS_irq_t irq,
                                                   picoRTOS_mask_t core_mask);

/* Function: arch_smp_disable_interrupt
 * Disables an interrupt on a specific core or set of cores
 *
 * Parameters:
 *  irq - The irq to disable
 *  core_mask - The core mask, one bit per core
 *
 * See also:
 *  <picoRTOS_SMP_disable_interrupt>
 */
extern /*@unused@*/ void arch_smp_disable_interrupt(picoRTOS_irq_t irq,
                                                    picoRTOS_mask_t core_mask);

#endif

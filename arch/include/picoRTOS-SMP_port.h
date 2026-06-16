#ifndef PICORTOS_SMP_PORT_H
#define PICORTOS_SMP_PORT_H

#include "picoRTOS_port.h"
#include "picoRTOS_types.h"

/**
 * void **arch_smp_init**(void);
 * > SMP port initialization function
 * ### NOTES
 * > This function is the SMP version of `arch_init()`.<br>
 * > It is called by <<ins>picoRTOS-SMP.c</ins>> `picoRTOS_start()`.
 * >
 * > <ins>Remark:</ins> `arch_init()` won't be automatically called.
 */
extern void arch_smp_init(void);

/**
 * void **arch_core_init**(**picoRTOS_core_t** <ins>core</ins>,
 * **picoRTOS_stack_t** \*<ins>sp</ins>);
 * > Auxiliary <ins>core</ins> startup function
 * ### NOTES
 * > This function will be called by `picoRTOS_start()` for every auxiliary
 * > core you want ot start & will provide the corresponding idle stack/context
 * > in <ins>sp</ins>.
 */
extern void arch_core_init(picoRTOS_core_t core, picoRTOS_stack_t *sp);

/**
 * picoRTOS_core_t **arch_core**(void);
 * > Provides the current core index
 */
extern picoRTOS_core_t arch_core(void) /*@*/;

/**
 * void **arch_spin_lock**(void);
 * > Acquires the SMP spinlock
 * ### NOTES
 * > On SMP architectures, the cores need a sync mechanism.<br>
 * > picoRTOS uses a simple spinlock, preferably a hardware one.
 * >
 * > Remark: this **MUST** make CONFIG_DEADLOCK_COUNT attempts at acquiring the lock
 * > and declare deadlock if it fails (`fatal()`)
 */
extern void arch_spin_lock(void);

/**
 * void **arch_spin_unlock**(void);
 * > Unlocks the SMP spinlock
 */
extern void arch_spin_unlock(void);

/**§
 * Interrupt management
 */

/**
 * void **arch_smp_register_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>,
 * **arch_isr_fn** <ins>fn</ins>, **void** \*<ins>priv</ins>,
 * **picoRTOS_mask_t** <ins>core_mask</ins>);
 * > Registers an interrupt on the system & attach it to the core(s)
 * > matching the <ins>core_mask</ins>
 * ### NOTES
 * > Make sure that when <ins>irq</ins> is asserted, <ins>fn</ins> is
 * > called with <ins>priv</ins> as a parameter.
 * >
 * > <ins>core_mask</ins> is a bitfield where every core has it own bit,
 * > for example, (1 << 0) for core0, (1 << 1) for core1, etc
 */
extern /*@unused@*/ void arch_smp_register_interrupt(picoRTOS_irq_t irq,
                                                     arch_isr_fn fn,
                                                     /*@null@*/ void *priv,
                                                     picoRTOS_mask_t core_mask);

#endif

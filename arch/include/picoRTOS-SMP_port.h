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

#endif

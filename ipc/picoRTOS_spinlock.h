#ifndef PICORTOS_SPINLOCK_H
#define PICORTOS_SPINLOCK_H

#include "picoRTOS.h"

/* A word of caution:
 * This is not very useful and even dangerous in a non-SMP
 * environment. Prefer futexes if possible
 */

typedef picoRTOS_atomic_t picoRTOS_spinlock_t;

void picoRTOS_spinlock_init(/*@notnull@*/ /*@out@*/ picoRTOS_spinlock_t *lock);

int picoRTOS_spin_trylock(/*@notnull@*/ picoRTOS_spinlock_t *lock);
void picoRTOS_spin_lock(/*@notnull@*/ picoRTOS_spinlock_t *lock);
void picoRTOS_spin_unlock(/*@notnull@*/ picoRTOS_spinlock_t *lock);

#endif

#ifndef PICORTOS_MUTEX_H
#define PICORTOS_MUTEX_H

#include <errno.h>
#include "picoRTOS.h"
#include "picoRTOS_port.h"

struct picoRTOS_mutex {
    picoRTOS_atomic_t owner;
    size_t count;
    /* HOTFIX, see picoRTOS_mutex.c */
    picoRTOS_atomic_t prev_owner;
} __attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

#define PICORTOS_MUTEX_NOOWNER -1

/* Macro: PICORTOS_MUTEX_INITIALIZER
 * Statically initializes a mutex
 */
#define PICORTOS_MUTEX_INITIALIZER                                      \
  { (picoRTOS_atomic_t)PICORTOS_MUTEX_NOOWNER,                          \
      (size_t)0,                                                        \
      (picoRTOS_atomic_t)CONFIG_TASK_COUNT }

void picoRTOS_mutex_init(/*@out@*/ struct picoRTOS_mutex *mutex);           /* init mutex */

/*@unused@*/ int picoRTOS_mutex_trylock(struct picoRTOS_mutex *mutex);      /* attempt to acquire mutex */
void picoRTOS_mutex_lock(struct picoRTOS_mutex *mutex);                     /* acquire mutex */
void picoRTOS_mutex_unlock(struct picoRTOS_mutex *mutex);                   /* release mutex */

#endif

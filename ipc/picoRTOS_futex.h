#ifndef PICORTOS_FUTEX_H
#define PICORTOS_FUTEX_H

#include <errno.h>
#include "picoRTOS.h"

typedef picoRTOS_atomic_t picoRTOS_futex_t;

/* macro for static init */
#define PICORTOS_FUTEX_INITIALIZER (picoRTOS_futex_t)0

void picoRTOS_futex_init(/*@out@*/ picoRTOS_futex_t *futex);        /* init futex */

/*@unused@*/ int picoRTOS_futex_trylock(picoRTOS_futex_t *futex);   /* try ot lock futex */
void picoRTOS_futex_lock(picoRTOS_futex_t *futex);                  /* lock futex */
void picoRTOS_futex_unlock(picoRTOS_futex_t *futex);                /* unlock futex */

#endif

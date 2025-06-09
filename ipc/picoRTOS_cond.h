#ifndef PICORTOS_COND_H
#define PICORTOS_COND_H

#include "picoRTOS.h"
#include "picoRTOS_mutex.h"

enum picoRTOS_cond_act {
    PICORTOS_COND_NONE,
    PICORTOS_COND_SIGNAL,
    PICORTOS_COND_BROADCAST
};

struct picoRTOS_cond {
    enum picoRTOS_cond_act act;
    size_t count;
} __attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

/* macro for static init */
#define PICORTOS_COND_INITIALIZER               \
    { PICORTOS_COND_NONE, (size_t)0 }

void picoRTOS_cond_init(struct picoRTOS_cond *cond);

void picoRTOS_cond_signal(struct picoRTOS_cond * cond);                                     /* signal condition */
void picoRTOS_cond_broadcast(struct picoRTOS_cond * cond);                                  /* broadcast condition */
void picoRTOS_cond_wait(struct picoRTOS_cond *cond,
                        struct picoRTOS_mutex *mutex);                                      /* wait for condition */

#endif

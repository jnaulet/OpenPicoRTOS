#ifndef PICORTOS_SMP_H
#define PICORTOS_SMP_H

/* SMP extensions for picoRTOS */

#include "picoRTOS.h"

/* CORES */

/* TASKS (potentially unused) */

/*@unused@*/ void picoRTOS_SMP_add_task(struct picoRTOS_task *task,
                                        picoRTOS_priority_t prio,
                                        picoRTOS_mask_t core_mask);

/* INTERRUPT MANAGEMENT (optional) */

void picoRTOS_SMP_enable_interrupt(picoRTOS_irq_t irq,
                                   picoRTOS_mask_t core_mask);
void picoRTOS_SMP_disable_interrupt(picoRTOS_irq_t irq,
                                    picoRTOS_mask_t core_mask);

#endif

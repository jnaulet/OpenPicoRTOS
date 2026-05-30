/*
 * IDLE is a task in itself, so we have to separate it from the kernel
 * if we want to be able to run it in unprivileged mode
 */
#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

/* SIO */
#define SIO_CPUID     ((volatile unsigned long*)(ADDR_SIO + 0))
#define SIO_SPINLOCK1 ((volatile unsigned long*)(ADDR_SIO + 0x104))

/* this is debatable */
picoRTOS_core_t arch_core(void)
{
    return (picoRTOS_core_t)*SIO_CPUID;
}

static bool UNPRIVILEGED_DATA core1_is_idling = false;
/* This is a privileged interface for picoRTOS-SMP_port.c */
int PRIVILEGED_TEXT arch_core1_is_idling(void)
{
    if (core1_is_idling) return 0;
    return -1;
}

/*
 * IDLE is a task in itself, so we have to separate it from the kernel
 * if we want to be able to run it in unprivileged mode
 */
void arch_idle(void)
{
    if (arch_core() != 0)
        core1_is_idling = true;

    for (;;)
        ASM("wfe");
}

/* ATOMIC ops */

picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                        picoRTOS_atomic_t old,
                                        picoRTOS_atomic_t val)
{
    picoRTOS_atomic_t res = val;

    if (*SIO_SPINLOCK1 != 0) {
        res = *var;
        if (res == old)
            *var = val;

        *SIO_SPINLOCK1 = 1ul;
    }

    return res;
}

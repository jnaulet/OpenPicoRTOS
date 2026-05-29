/*
 * IDLE is a task in itself, so we have to separate it from the kernel
 * if we want to be able to run it in unprivileged mode
 */
#include "picoRTOS_port.h"

void __attribute__((weak)) arch_idle(void)
{
    for (;;)
        ASM("wfe");
}

/*
 * Atomic ops are unprivileged
 */
picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr)
{
    return arch_compare_and_swap(ptr, 0, (picoRTOS_atomic_t)1);
}

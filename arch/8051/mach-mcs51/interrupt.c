#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

void arch_enable_interrupt_ext(picoRTOS_irq_t irq, picoRTOS_mask_t core_mask)
{
    /*@i@*/ (void)core_mask;
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);
}

void arch_disable_interrupt_ext(picoRTOS_irq_t irq, picoRTOS_mask_t core_mask)
{
    /*@i@*/ (void)core_mask;
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);
}

#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stddef.h>
#include <stdint.h>

/* Hum... */
#define cliccfg        ((volatile uint8_t*)(ADDR_ECLIC + 0x0))
#define clicinfo       ((volatile uint32_t*)(ADDR_ECLIC + 0x4))
#define mth            ((volatile uint32_t*)(ADDR_ECLIC + 0xb))
#define clicintip(x)   ((volatile uint8_t*)(ADDR_ECLIC + 0x1000 + 4 * (x)))
#define clicintie(x)   ((volatile uint8_t*)(ADDR_ECLIC + 0x1001 + 4 * (x)))
#define clicintattr(x) ((volatile uint8_t*)(ADDR_ECLIC + 0x1002 + 4 * (x)))
#define clicintctl(x)  ((volatile uint8_t*)(ADDR_ECLIC + 0x1003 + 4 * (x)))

void arch_enable_interrupt_ext(picoRTOS_irq_t irq, picoRTOS_mask_t core_mask)
{
    /*@i@*/ (void)core_mask;
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );
    *clicintie(irq) = (uint8_t)1;
    *clicintattr(irq) = (uint8_t)1;
}

void arch_disable_interrupt_ext(picoRTOS_irq_t irq, picoRTOS_mask_t core_mask)
{
    /*@i@*/ (void)core_mask;
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );
    *clicintie(irq) = (uint8_t)0;
}

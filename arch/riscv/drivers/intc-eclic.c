#include "picoRTOS.h"
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

/*@external@*/
extern struct {
    picoRTOS_isr_fn fn;
    /*@temp@*/ /*@null@*/ void *priv;
} ISR_TABLE[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_register_interrupt(picoRTOS_irq_t irq, picoRTOS_isr_fn fn, void *priv)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;

    ISR_TABLE[irq].fn = fn;
    ISR_TABLE[irq].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;

    *clicintie(irq) = (uint8_t)1;
    *clicintattr(irq) = (uint8_t)1;
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;

    *clicintie(irq) = (uint8_t)0;
}

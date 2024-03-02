#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stddef.h>
#include <stdint.h>

struct R32_PFIC {
    volatile uint32_t ISR1;
    volatile uint32_t ISR2;
    uint32_t RESERVED0[6];
    volatile uint32_t IPR1;
    volatile uint32_t IPR2;
    uint32_t RESERVED1[6];
    volatile uint32_t ITHRESDR;
    uint32_t RESERVED2;
    volatile uint32_t CFGR;
    volatile uint32_t GISR;
    volatile uint32_t VTFIDR;
    uint32_t RESERVED3[3];
    volatile uint32_t VTFADDRR0;
    volatile uint32_t VTFADDRR1;
    uint32_t RESERVED4[26];
    volatile uint32_t IENR1;
    volatile uint32_t IENR2;
    uint32_t RESERVED5[30];
    volatile uint32_t IRER1;
    volatile uint32_t IRER2;
    uint32_t RESERVED6[30];
    volatile uint32_t IPSR1;
    volatile uint32_t IPSR2;
    uint32_t RESERVED7[30];
    volatile uint32_t IPRR1;
    volatile uint32_t IPRR2;
    uint32_t RESERVED8[30];
    volatile uint32_t IACTR1;
    volatile uint32_t IACTR2;
    uint32_t RESERVED9[62];
    volatile uint32_t IPRIORx;
    uint32_t RESERVED10[579];
    volatile uint32_t SCTLR;
};

static struct R32_PFIC *PFIC = (struct R32_PFIC*)0xe000e000;

/*@external@*/
extern struct {
    arch_isr_fn fn;
    /*@temp@*/ /*@null@*/ void *priv;
} ISR_TABLE[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_register_interrupt(picoRTOS_irq_t irq, arch_isr_fn fn, void *priv)
{
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );
    ISR_TABLE[irq].fn = fn;
    ISR_TABLE[irq].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );

    if (irq < (picoRTOS_irq_t)16) {
        unsigned shift = (unsigned)irq + 16u;
        PFIC->IENR1 |= (1 << shift);
    }else{
        unsigned shift = (unsigned)irq - 16u;
        PFIC->IENR2 |= (1 << shift);
    }
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );

    if (irq < (picoRTOS_irq_t)16) {
        unsigned shift = (unsigned)irq + 16u;
        PFIC->IENR1 &= ~(1 << shift);
    }else{
        unsigned shift = (unsigned)irq - 16u;
        PFIC->IENR2 &= ~(1 << shift);
    }
}

#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_IE)   IE;
__sfr __at(ADDR_IE2)  IE2;
#else
static unsigned char IE;
static unsigned char IE2;
#endif

#define IE_EA   (1 << 7)
#define IE_ELVD (1 << 6)
#define IE_EADC (1 << 5)
#define IE_ES   (1 << 4)
#define IE_ET1  (1 << 3)
#define IE_EX1  (1 << 2)
#define IE_ET0  (1 << 1)
#define IE_EX0  (1 << 0)

#define IE2_ESPI (1 << 1)
#define IE2_ES2  (1 << 0)

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    switch (irq) {
    case IRQ_IE0: IE |= IE_EX0; break;
    case IRQ_TF0: IE |= IE_ET0; break;
    case IRQ_IE1: IE |= IE_EX1; break;
    case IRQ_TF1: IE |= IE_ET1; break;
    case IRQ_RI_TI: IE |= IE_ES; break;
    case IRQ_ADC: IE |= IE_EADC; break;
    case IRQ_LVD: IE |= IE_ELVD; break;
    case IRQ_CF_CCF0_CCF1: break;
    case IRQ_S2TI_S2RI: IE2 |= IE2_ES2; break;
    case IRQ_SPIF: IE2 |= IE2_ESPI; break;
    default:
        arch_assert_void(false);
        break;
    }
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    switch (irq) {
    case IRQ_IE0: IE &= ~IE_EX0; break;
    case IRQ_TF0: IE &= ~IE_ET0; break;
    case IRQ_IE1: IE &= ~IE_EX1; break;
    case IRQ_TF1: IE &= ~IE_ET1; break;
    case IRQ_RI_TI: IE &= ~IE_ES; break;
    case IRQ_ADC: IE &= ~IE_EADC; break;
    case IRQ_LVD: IE &= ~IE_ELVD; break;
    case IRQ_CF_CCF0_CCF1: break;
    case IRQ_S2TI_S2RI: IE2 &= ~IE2_ES2; break;
    case IRQ_SPIF: IE2 &= ~IE2_ESPI; break;
    default:
        arch_assert_void(false);
        break;
    }
}

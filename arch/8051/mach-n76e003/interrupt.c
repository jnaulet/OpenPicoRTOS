#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_IE)   IE;
__sfr __at(ADDR_EIE)  C99_EIE;
__sfr __at(ADDR_EIE1) C99_EIE1;
#else
static unsigned char IE;
static unsigned char C99_EIE;
static unsigned char C99_EIE1;
#endif

#define IE_EA   (1 << 7)
#define IE_EADC (1 << 6)
#define IE_EBOD (1 << 5)
#define IE_ES   (1 << 4)
#define IE_ET1  (1 << 3)
#define IE_EX1  (1 << 2)
#define IE_ET0  (1 << 1)
#define IE_EX0  (1 << 0)

#define C99_EIE_ET2  (1 << 7)
#define C99_EIE_ESPI (1 << 6)
#define C99_EIE_EFB  (1 << 5)
#define C99_EIE_EWDT (1 << 4)
#define C99_EIE_EPWM (1 << 3)
#define C99_EIE_ECAP (1 << 2)
#define C99_EIE_EPI  (1 << 1)
#define C99_EIE_EI2C (1 << 0)

#define C99_EIE1_EWKT (1 << 2)
#define C99_EIE1_ET3  (1 << 1)
#define C99_EIE1_ES1  (1 << 0)

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    switch (irq) {
    case IRQ_IE_0: IE |= IE_EX0; break;
    case IRQ_TF_0: IE |= IE_ET0; break;
    case IRQ_IE_1: IE |= IE_EX1; break;
    case IRQ_TF_1: IE |= IE_ET1; break;
    case IRQ_RI_TI_0: IE |= IE_ES; break;
    case IRQ_TF_2: C99_EIE |= C99_EIE_ET2; break;
    case IRQ_SI: C99_EIE |= C99_EIE_EI2C; break;
    case IRQ_PIF: C99_EIE |= C99_EIE_EPI; break;
    case IRQ_BOD: IE |= IE_EBOD; break;
    case IRQ_SPI: C99_EIE |= C99_EIE_ESPI; break;
    case IRQ_WDTF: C99_EIE |= C99_EIE_EWDT; break;
    case IRQ_ADCF: IE |= IE_EADC; break;
    case IRQ_CAPF: C99_EIE |= C99_EIE_ECAP; break;
    case IRQ_PWMF: C99_EIE |= C99_EIE_EPWM; break;
    case IRQ_FBF: C99_EIE |= C99_EIE_EFB; break;
    case IRQ_RI_TI_1: C99_EIE1 |= C99_EIE1_ES1; break;
    case IRQ_TF_3: C99_EIE1 |= C99_EIE1_ET3; break;
    case IRQ_WKTF: C99_EIE1 |= C99_EIE1_EWKT; break;
    default:
        arch_assert_void(false);
        break;
    }
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    switch (irq) {
    case IRQ_IE_0: IE &= ~IE_EX0; break;
    case IRQ_TF_0: IE &= ~IE_ET0; break;
    case IRQ_IE_1: IE &= ~IE_EX1; break;
    case IRQ_TF_1: IE &= ~IE_ET1; break;
    case IRQ_RI_TI_0: IE &= ~IE_ES; break;
    case IRQ_TF_2: C99_EIE &= ~C99_EIE_ET2; break;
    case IRQ_SI: C99_EIE &= ~C99_EIE_EI2C; break;
    case IRQ_PIF: C99_EIE &= ~C99_EIE_EPI; break;
    case IRQ_BOD: IE &= ~IE_EBOD; break;
    case IRQ_SPI: C99_EIE &= ~C99_EIE_ESPI; break;
    case IRQ_WDTF: C99_EIE &= ~C99_EIE_EWDT; break;
    case IRQ_ADCF: IE &= ~IE_EADC; break;
    case IRQ_CAPF: C99_EIE &= ~C99_EIE_ECAP; break;
    case IRQ_PWMF: C99_EIE &= ~C99_EIE_EPWM; break;
    case IRQ_FBF: C99_EIE &= ~C99_EIE_EFB; break;
    case IRQ_RI_TI_1: C99_EIE1 &= ~C99_EIE1_ES1; break;
    case IRQ_TF_3: C99_EIE1 &= ~C99_EIE1_ET3; break;
    case IRQ_WKTF: C99_EIE1 &= ~C99_EIE1_EWKT; break;
    default:
        arch_assert_void(false);
        break;
    }
}

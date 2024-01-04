#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_IE)  IE;
__sfr __at(ADDR_EIE) C99_EIE;
#else
static unsigned char IE;
static unsigned char C99_EIE;
#endif

#define IE_EA   (1 << 7)
#define IE_ES1  (1 << 6)
#define IE_ET2  (1 << 5)
#define IE_ES0  (1 << 4)
#define IE_ET1  (1 << 3)
#define IE_EX1  (1 << 2)
#define IE_ET0  (1 << 1)
#define IE_EX0  (1 << 0)

#define C99_EIE_EX6  (1 << 4)
#define C99_EIE_EX5  (1 << 3)
#define C99_EIE_EX4  (1 << 2)
#define C99_EIE_EI2C (1 << 1)
#define C99_EIE_EUSB (1 << 0)

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    switch (irq) {
    case IRQ_IE0: IE |= IE_EX0; break;
    case IRQ_TF0: IE |= IE_ET0; break;
    case IRQ_IE1: IE |= IE_EX1; break;
    case IRQ_TF1: IE |= IE_ET1; break;
    case IRQ_RI_TI_0: IE |= IE_ES0; break;
    case IRQ_TF2: IE |= IE_ET2; break;
    case IRQ_RESUME: break;
    case IRQ_RI_TI_1: IE |= IE_ES1; break;
    case IRQ_USB: C99_EIE |= C99_EIE_EUSB; break;
    case IRQ_I2C: C99_EIE |= C99_EIE_EI2C; break;
    case IRQ_IE4: C99_EIE |= C99_EIE_EX4; break;
    case IRQ_IE5: C99_EIE |= C99_EIE_EX5; break;
    case IRQ_IE6: C99_EIE |= C99_EIE_EX6; break;
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
    case IRQ_RI_TI_0: IE &= ~IE_ES0; break;
    case IRQ_TF2: IE &= ~IE_ET2; break;
    case IRQ_RESUME: break;
    case IRQ_RI_TI_1: IE &= ~IE_ES1; break;
    case IRQ_USB: C99_EIE &= ~C99_EIE_EUSB; break;
    case IRQ_I2C: C99_EIE &= ~C99_EIE_EI2C; break;
    case IRQ_IE4: C99_EIE &= ~C99_EIE_EX4; break;
    case IRQ_IE5: C99_EIE &= ~C99_EIE_EX5; break;
    case IRQ_IE6: C99_EIE &= ~C99_EIE_EX6; break;
    default:
        arch_assert_void(false);
        break;
    }
}

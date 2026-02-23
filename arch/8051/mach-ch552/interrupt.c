#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_IP_EX)   IP_EX;
__sfr __at(ADDR_IE_EX)   IE_EX;
__sfr __at(ADDR_GPIO_IE) GPIO_IE;
__sfr __at(ADDR_IP)      IP;
__sfr __at(ADDR_IE)      IE;
#else
/*@unused@*/ static unsigned char IP_EX;
static unsigned char IE_EX;
/*@unused@*/ static unsigned char GPIO_IE;
/*@unused@*/ static unsigned char IP;
static unsigned char IE;
#endif

#define IE_EA    (1 << 7)
#define IE_E_DIS (1 << 6)
#define IE_ET2   (1 << 5)
#define IE_ES    (1 << 4)
#define IE_ET1   (1 << 3)
#define IE_EX1   (1 << 2)
#define IE_ET0   (1 << 1)
#define IE_EX0   (1 << 0)

#define IE_EX_IE_WDOG  (1 << 7)
#define IE_EX_IE_GPIO  (1 << 6)
#define IE_EX_IE_PWMX  (1 << 5)
#define IE_EX_IE_UART1 (1 << 4)
#define IE_EX_IE_ADC   (1 << 3)
#define IE_EX_IE_USB   (1 << 2)
#define IE_EX_IE_TKEY  (1 << 1)
#define IE_EX_IE_SPI0  (1 << 0)

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    switch (irq) {
    case IRQ_INT0: IE |= IE_EX0; break;
    case IRQ_TMR0: IE |= IE_ET0; break;
    case IRQ_INT1: IE |= IE_EX1; break;
    case IRQ_TMR1: IE |= IE_ET1; break;
    case IRQ_UART0: IE |= IE_ES; break;
    case IRQ_TMR2: IE |= IE_ET2; break;
    case IRQ_SPI0: IE_EX |= IE_EX_IE_SPI0; break;
    case IRQ_TKEY: IE_EX |= IE_EX_IE_TKEY; break;
    case IRQ_USB: IE_EX |= IE_EX_IE_USB; break;
    case IRQ_ADC: IE_EX |= IE_EX_IE_ADC; break;
    case IRQ_UART1: IE_EX |= IE_EX_IE_UART1; break;
    case IRQ_PWMX: IE_EX |= IE_EX_IE_PWMX; break;
    case IRQ_GPIO: IE_EX |= IE_EX_IE_GPIO; break;
    case IRQ_WDOG: IE_EX |= IE_EX_IE_WDOG; break;
    default:
        arch_assert_void(false);
        break;
    }
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    switch (irq) {
    case IRQ_INT0: IE &= ~IE_EX0; break;
    case IRQ_TMR0: IE &= ~IE_ET0; break;
    case IRQ_INT1: IE &= ~IE_EX1; break;
    case IRQ_TMR1: IE &= ~IE_ET1; break;
    case IRQ_UART0: IE &= ~IE_ES; break;
    case IRQ_TMR2: IE &= ~IE_ET2; break;
    case IRQ_SPI0: IE_EX &= ~IE_EX_IE_SPI0; break;
    case IRQ_TKEY: IE_EX &= ~IE_EX_IE_TKEY; break;
    case IRQ_USB: IE_EX &= ~IE_EX_IE_USB; break;
    case IRQ_ADC: IE_EX &= ~IE_EX_IE_ADC; break;
    case IRQ_UART1: IE_EX &= ~IE_EX_IE_UART1; break;
    case IRQ_PWMX: IE_EX &= ~IE_EX_IE_PWMX; break;
    case IRQ_GPIO: IE_EX &= ~IE_EX_IE_GPIO; break;
    case IRQ_WDOG: IE_EX &= ~IE_EX_IE_WDOG; break;
    default:
        arch_assert_void(false);
        break;
    }
}

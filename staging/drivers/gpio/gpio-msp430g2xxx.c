#include "gpio-msp430g2xxx.h"
#include "picoRTOS.h"

struct GPIO_MSP430G2XXX {
    volatile uint8_t P3REN;     /* 0x10 */
    volatile uint8_t P4REN;     /* 0x11 */
    volatile uint8_t P5REN;     /* 0x12 */
    volatile uint8_t P6REN;     /* 0x13 */
    volatile uint8_t P7REN;     /* 0x14 */
    volatile uint8_t P8REN;     /* 0x15 */
    uint8_t RESERVED0[2];
    volatile uint8_t P3IN;      /* 0x18 */
    volatile uint8_t P3OUT;     /* 0x19 */
    volatile uint8_t P3DIR;     /* 0x1a */
    volatile uint8_t P3SEL;     /* 0x1b */
    volatile uint8_t P4IN;      /* 0x1c */
    volatile uint8_t P4OUT;     /* 0x1d */
    volatile uint8_t P4DIR;     /* 0x1e */
    volatile uint8_t P4SEL;     /* 0x1f */
    volatile uint8_t P1IN;      /* 0x20 */
    volatile uint8_t P1OUT;     /* 0x21 */
    volatile uint8_t P1DIR;     /* 0x22 */
    volatile uint8_t P1IFG;     /* 0x23 */
    volatile uint8_t P1IES;     /* 0x24 */
    volatile uint8_t P1IE;      /* 0x25 */
    volatile uint8_t P1SEL;     /* 0x26 */
    volatile uint8_t P1REN;     /* 0x27 */
    volatile uint8_t P2IN;      /* 0x28 */
    volatile uint8_t P2OUT;     /* 0x29 */
    volatile uint8_t P2DIR;     /* 0x2a */
    volatile uint8_t P2IFG;     /* 0x2b */
    volatile uint8_t P2IES;     /* 0x2c */
    volatile uint8_t P2IE;      /* 0x2d */
    volatile uint8_t P2SEL;     /* 0x2e */
    volatile uint8_t P2REN;     /* 0x2f */
    volatile uint8_t P5IN;      /* 0x30 */
    volatile uint8_t P5OUT;     /* 0x31 */
    volatile uint8_t P5DIR;     /* 0x32 */
    volatile uint8_t P5SEL;     /* 0x33 */
    volatile uint8_t P6IN;      /* 0x34 */
    volatile uint8_t P6OUT;     /* 0x35 */
    volatile uint8_t P6DIR;     /* 0x36 */
    volatile uint8_t P6SEL;     /* 0x37 */
    volatile uint8_t P7IN;      /* 0x38 */
    volatile uint8_t P8IN;      /* 0x39 */
    volatile uint8_t P7OUT;     /* 0x3a */
    volatile uint8_t P8OUT;     /* 0x3b */
    volatile uint8_t P7DIR;     /* 0x3c */
    volatile uint8_t P8DIR;     /* 0x3d */
    volatile uint8_t P7SEL;     /* 0x3e */
    volatile uint8_t P8SEL;     /* 0x3f */
    uint8_t RESERVED1;
    volatile uint8_t P1SEL2;    /* 0x41 */
    volatile uint8_t P2SEL2;    /* 0x42 */
    volatile uint8_t P3SEL2;    /* 0x43 */
    volatile uint8_t P4SEL2;    /* 0x44 */
    volatile uint8_t P5SEL2;    /* 0x45 */
    volatile uint8_t P6SEL2;    /* 0x46 */
    volatile uint8_t P7SEL2;    /* 0x47 */
    volatile uint8_t P8SEL2;    /* 0x47 */
};

int gpio_msp430g2xxx_init(struct gpio *ctx, int base, size_t port, size_t pin)
{
    picoRTOS_assert(port < (size_t)GPIO_MSP430G2XXX_PORT_COUNT, return -EINVAL);
    picoRTOS_assert(pin < (size_t)GPIO_MSP430G2XXX_PIN_COUNT, return -EINVAL);

    struct GPIO_MSP430G2XXX *parent = (struct GPIO_MSP430G2XXX*)base; // NOLINT

    switch (port) {
    case 0:
        ctx->PxIN = &parent->P1IN;
        ctx->PxOUT = &parent->P1OUT;
        break;

    case 1:
        ctx->PxIN = &parent->P2IN;
        ctx->PxOUT = &parent->P2OUT;
        break;

    case 2:
        ctx->PxIN = &parent->P3IN;
        ctx->PxOUT = &parent->P3OUT;
        break;

    case 3:
        ctx->PxIN = &parent->P4IN;
        ctx->PxOUT = &parent->P4OUT;
        break;

    case 4:
        ctx->PxIN = &parent->P5IN;
        ctx->PxOUT = &parent->P5OUT;
        break;

    case 5:
        ctx->PxIN = &parent->P6IN;
        ctx->PxOUT = &parent->P6OUT;
        break;

    case 6:
        ctx->PxIN = &parent->P7IN;
        ctx->PxOUT = &parent->P7OUT;
        break;

    case 7:
        ctx->PxIN = &parent->P8IN;
        ctx->PxOUT = &parent->P8OUT;
        break;

    default:
        picoRTOS_assert_void(false);
        return -EINVAL;
    }

    ctx->mask = (uint8_t)(1u << pin);
    return 0;
}

static int set_invert(struct gpio *ctx, gpio_invert_t invert)
{
    picoRTOS_assert(invert != GPIO_INVERT_IGNORE, return -EINVAL);
    picoRTOS_assert(invert < GPIO_INVERT_COUNT, return -EINVAL);

    ctx->invert = (invert == GPIO_INVERT_ENABLE);
    return 0;
}

int gpio_setup(struct gpio *ctx, const struct gpio_settings *settings)
{
    int res;

    if (settings->invert != GPIO_INVERT_IGNORE &&
        (res = set_invert(ctx, settings->invert)) < 0)
        return res;

    return 0;
}

void gpio_write(struct gpio *ctx, bool value)
{
    if (value ^ ctx->invert) *ctx->PxOUT |= ctx->mask;
    else *ctx->PxOUT &= ~ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return ((*ctx->PxIN & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    *ctx->PxOUT ^= ctx->mask;
}

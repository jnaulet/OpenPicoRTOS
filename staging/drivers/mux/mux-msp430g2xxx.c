#include "mux-msp430g2xxx.h"
#include "picoRTOS.h"

struct MUX_MSP430G2XXX {
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

int mux_msp430g2xxx_init(struct mux *ctx, int base, size_t port)
{
    picoRTOS_assert(port < (size_t)MUX_MSP430G2XXX_PORT_COUNT, return -EINVAL);
    struct MUX_MSP430G2XXX *parent = (struct MUX_MSP430G2XXX*)base; // NOLINT

    switch (port) {
    case 0:
        ctx->PxOUT = &parent->P1OUT;
        ctx->PxDIR = &parent->P1DIR;
        ctx->PxSEL = &parent->P1SEL;
        ctx->PxSEL2 = &parent->P1SEL2;
        ctx->PxREN = &parent->P1REN;
        break;

    case 1:
        ctx->PxOUT = &parent->P2OUT;
        ctx->PxDIR = &parent->P2DIR;
        ctx->PxSEL = &parent->P2SEL;
        ctx->PxSEL2 = &parent->P2SEL2;
        ctx->PxREN = &parent->P2REN;
        break;

    case 2:
        ctx->PxOUT = &parent->P3OUT;
        ctx->PxDIR = &parent->P3DIR;
        ctx->PxSEL = &parent->P3SEL;
        ctx->PxSEL2 = &parent->P3SEL2;
        ctx->PxREN = &parent->P3REN;
        break;

    case 3:
        ctx->PxOUT = &parent->P4OUT;
        ctx->PxDIR = &parent->P4DIR;
        ctx->PxSEL = &parent->P4SEL;
        ctx->PxSEL2 = &parent->P4SEL2;
        ctx->PxREN = &parent->P4REN;
        break;

    case 4:
        ctx->PxOUT = &parent->P5OUT;
        ctx->PxDIR = &parent->P5DIR;
        ctx->PxSEL = &parent->P5SEL;
        ctx->PxSEL2 = &parent->P5SEL2;
        ctx->PxREN = &parent->P5REN;
        break;

    case 5:
        ctx->PxOUT = &parent->P6OUT;
        ctx->PxDIR = &parent->P6DIR;
        ctx->PxSEL = &parent->P6SEL;
        ctx->PxSEL2 = &parent->P6SEL2;
        ctx->PxREN = &parent->P6REN;
        break;

    case 6:
        ctx->PxOUT = &parent->P7OUT;
        ctx->PxDIR = &parent->P7DIR;
        ctx->PxSEL = &parent->P7SEL;
        ctx->PxSEL2 = &parent->P7SEL2;
        ctx->PxREN = &parent->P7REN;
        break;

    case 7:
        ctx->PxOUT = &parent->P8OUT;
        ctx->PxDIR = &parent->P8DIR;
        ctx->PxSEL = &parent->P8SEL;
        ctx->PxSEL2 = &parent->P8SEL2;
        ctx->PxREN = &parent->P8REN;
        break;

    default:
        picoRTOS_assert_void(false);
        return -EINVAL;
    }

    return 0;
}

static int pxsel_setup(struct mux *ctx, size_t pin, mux_msp430g2xxx_pxsel_t pxsel)
{
    picoRTOS_assert(pin < (size_t)MUX_MSP430G2XXX_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(pxsel < MUX_MSP430G2XXX_PXSEL_COUNT, return -EINVAL);

    switch (pxsel) {
    case MUX_MSP430G2XXX_PXSEL_GPIO:
        *ctx->PxSEL &= ~(1u << pin);
        *ctx->PxSEL2 &= ~(1u << pin);
        break;

    case MUX_MSP430G2XXX_PXSEL_FUNC1:
        *ctx->PxSEL |= (1u << pin);
        *ctx->PxSEL2 &= ~(1u << pin);
        break;
    case MUX_MSP430G2XXX_PXSEL_FUNC2:
        *ctx->PxSEL &= ~(1u << pin);
        *ctx->PxSEL2 |= (1u << pin);
        break;
    case MUX_MSP430G2XXX_PXSEL_FUNC3:
        *ctx->PxSEL |= (1u << pin);
        *ctx->PxSEL2 |= (1u << pin);
        break;

    default:
        picoRTOS_assert_void(false);
        return -EINVAL;
    }

    return 0;
}

int mux_msp430g2xxx_input(struct mux *ctx, size_t pin, mux_msp430g2xxx_pxsel_t pxsel)
{
    picoRTOS_assert(pin < (size_t)MUX_MSP430G2XXX_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(pxsel < MUX_MSP430G2XXX_PXSEL_COUNT, return -EINVAL);

    *ctx->PxDIR &= ~(1u << pin);
    return pxsel_setup(ctx, pin, pxsel);
}

int mux_msp430g2xxx_output(struct mux *ctx, size_t pin, mux_msp430g2xxx_pxsel_t pxsel)
{
    picoRTOS_assert(pin < (size_t)MUX_MSP430G2XXX_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(pxsel < MUX_MSP430G2XXX_PXSEL_COUNT, return -EINVAL);

    *ctx->PxDIR |= (1u << pin);
    return pxsel_setup(ctx, pin, pxsel);
}

int mux_msp430g2xxx_pull_up(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_MSP430G2XXX_PIN_COUNT, return -EINVAL);

    *ctx->PxOUT |= (1u << pin);
    *ctx->PxREN |= (1u << pin);

    return 0;
}

int mux_msp430g2xxx_pull_down(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_MSP430G2XXX_PIN_COUNT, return -EINVAL);

    *ctx->PxOUT &= ~(1u << pin);
    *ctx->PxREN |= (1u << pin);

    return 0;
}

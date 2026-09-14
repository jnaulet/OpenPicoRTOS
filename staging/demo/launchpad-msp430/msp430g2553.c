#include "msp430g2553.h"
#include "picoRTOS_device.h"

#include "clock-msp430g2xxx.h"
#include "mux-msp430g2xxx.h"
#include "gpio-msp430g2xxx.h"

static int clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_MSP430G2XXX_DCO_16MHZ,
        CLOCK_MSP430G2XXX_LFXT1SX_32768_XTAL,
        CLOCK_MSP430G2XXX_DIVX_1,   /* DIVAx */
        CLOCK_MSP430G2XXX_SELMX_DCOCLK,
        CLOCK_MSP430G2XXX_DIVX_1,   /* DIVMx */
        CLOCK_MSP430G2XXX_SELS_DCO,
        CLOCK_MSP430G2XXX_DIVX_2,   /* DIVSx */
    };

    (void)clock_msp430g2xxx_init(&CLOCK_settings);
    return 0;
}

static int mux_init(void)
{
    struct mux P1;
    struct mux P2;

    (void)mux_msp430g2xxx_init(&P1, ADDR_DIGITAL_IO, (size_t)0);
    (void)mux_msp430g2xxx_init(&P2, ADDR_DIGITAL_IO, (size_t)1);

    (void)mux_msp430g2xxx_output(&P1, (size_t)0, MUX_MSP430G2XXX_PXSEL_GPIO);
    (void)mux_msp430g2xxx_output(&P1, (size_t)6, MUX_MSP430G2XXX_PXSEL_GPIO);
    (void)mux_msp430g2xxx_output(&P2, (size_t)2, MUX_MSP430G2XXX_PXSEL_GPIO);

    return 0;
}

static int gpio_init(/*@partial@*/ struct msp430g2553 *ctx)
{
    (void)gpio_msp430g2xxx_init(&ctx->TICK, ADDR_DIGITAL_IO, (size_t)1, (size_t)2);
    (void)gpio_msp430g2xxx_init(&ctx->LED1, ADDR_DIGITAL_IO, (size_t)0, (size_t)0);
    (void)gpio_msp430g2xxx_init(&ctx->LED2, ADDR_DIGITAL_IO, (size_t)0, (size_t)6);

    /* default: off */
    (void)gpio_write(&ctx->LED1, false);
    (void)gpio_write(&ctx->LED2, false);
    return 0;
}

int msp430g2553_init(struct msp430g2553 *ctx)
{
    (void)clock_init();
    (void)mux_init();

    (void)gpio_init(ctx);

    return 0;
}

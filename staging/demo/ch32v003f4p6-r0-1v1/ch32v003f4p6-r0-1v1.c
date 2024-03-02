#include "ch32v003f4p6-r0-1v1.h"
#include "picoRTOS_device.h"

#include "clock-ch32v003.h"
#include "mux-wch_ch32x.h"

static void clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_CH32V003_SYSCLK_PLLCLK,
        CLOCK_CH32V003_PLLSRC_HSE,
        24000000ul, /* HSE */
        1ul,        /* HCLK_DIV */
    };

    (void)clock_ch32v003_init(&CLOCK_settings);
    (void)clock_ch32v003_enable(CLOCK_CH32V003_CLK_IOPD);
}

static void mux_init(void)
{
    struct mux PD;

    (void)mux_wch_ch32x_init(&PD, ADDR_PORTD);

    (void)mux_wch_ch32x_output(&PD, (size_t)4, MUX_WCH_CH32X_OUTPUT_PUSH_PULL);
    (void)mux_wch_ch32x_output(&PD, (size_t)6, MUX_WCH_CH32X_OUTPUT_PUSH_PULL);
}

static int gpio_init(/*@partial@*/ struct ch32v003f4p6 *ctx)
{
    (void)gpio_wch_ch32x_init(&ctx->LED1, ADDR_PORTD, (size_t)4);
    (void)gpio_wch_ch32x_init(&ctx->TICK, ADDR_PORTD, (size_t)6);
    return 0;
}

int ch32v003f4p6_init(struct ch32v003f4p6 *ctx)
{
    clock_init();
    mux_init();

    (void)gpio_init(ctx);

    return 0;
}

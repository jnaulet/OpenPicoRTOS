#include "stm8s003f3p3.h"
#include "picoRTOS_device.h"

#include "clock-stm8.h"
#include "mux-stm8.h"

static void clock_init(void)
{
    /* HSI, 16Mhz */
    struct clock_settings CLOCK_settings = {
        CLOCK_STM8_CKM_HSI,
        0ul, /* no external clock */
        CLOCK_STM8_HSIDIV_1,
        CLOCK_STM8_CPUDIV_1
    };

    (void)clock_stm8_init(&CLOCK_settings);
}

static void mux_init(void)
{
    struct mux PB;
    struct mux PD;

    (void)mux_stm8_init(&PB, ADDR_PB);
    (void)mux_stm8_init(&PD, ADDR_PD);

    (void)mux_stm8_output(&PB, (size_t)5, MUX_STM8_OUTPUT_PUSH_PULL);
    (void)mux_stm8_output(&PD, (size_t)4, MUX_STM8_OUTPUT_PUSH_PULL);
}

static int gpio_init(/*@partial@*/ struct stm8s003f3p3 *ctx)
{
    (void)gpio_stm8_init(&ctx->TEST, ADDR_PB, (size_t)5);
    (void)gpio_stm8_init(&ctx->TICK, ADDR_PD, (size_t)4);
    return 0;
}

int stm8s003f3p3_init(struct stm8s003f3p3 *ctx)
{
    /* clock + mux */
    clock_init();
    mux_init();

    /* peripherals */
    (void)gpio_init(ctx);

    return 0;
}

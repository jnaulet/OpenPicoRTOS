#include "longan-nano.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "clock-gd32vf103.h"
#include "mux-gd32vf103.h"

static void clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        0ul, /* disable hxtal */
        CLOCK_GD32VF103_CK_SYS_IRC8M,
        CLOCK_GD32VF103_PLLSEL_IRC8M,
        108000000ul,    /* pll */
        1ul,            /* ahb_div */
        1ul,            /* apb1_div */
        1ul,            /* apb2_div */
        2ul,            /* adc_div */
    };

    (void)clock_gd32vf103_init(&CLOCK_settings);
}

static void mux_init(void)
{
    struct mux MUXA;
    struct mux MUXC;

    /* enable clocks */
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_PA);
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_PC);

    (void)mux_gd32vf103_init(&MUXA, ADDR_GPIOA);
    (void)mux_gd32vf103_init(&MUXC, ADDR_GPIOC);

    (void)mux_gd32vf103_output(&MUXC, (size_t)15, MUX_GD32VF103_MODE_OPEN_DRAIN);   /* TICK */
    (void)mux_gd32vf103_output(&MUXA, (size_t)1, MUX_GD32VF103_MODE_OPEN_DRAIN);    /* LED_G */
    (void)mux_gd32vf103_output(&MUXA, (size_t)2, MUX_GD32VF103_MODE_OPEN_DRAIN);    /* LED_B */
    (void)mux_gd32vf103_output(&MUXC, (size_t)13, MUX_GD32VF103_MODE_OPEN_DRAIN);   /* LED_R */
}

static int gpio_init(/*@partial@*/ struct longan_nano *ctx)
{
    (void)gpio_gd32vf103_init(&ctx->TICK, ADDR_GPIOC, (size_t)15);
    (void)gpio_gd32vf103_init(&ctx->LED_R, ADDR_GPIOC, (size_t)13);
    (void)gpio_gd32vf103_init(&ctx->LED_G, ADDR_GPIOA, (size_t)1);
    (void)gpio_gd32vf103_init(&ctx->LED_B, ADDR_GPIOA, (size_t)2);

    return 0;
}

int longan_nano_init(/*@unused@*/ struct longan_nano *ctx)
{
    clock_init();
    mux_init();

    (void)gpio_init(ctx);

    return 0;
}

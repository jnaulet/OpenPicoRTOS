#include "yd-stm32l431rc.h"
#include "picoRTOS_device.h"

#include "clock-stm32l4x.h"
#include "mux-stm32h7xx.h"

static int clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_STM32L4X_SYSCLK_PLL,
        CLOCK_STM32L4X_PLLSRC_HSE,
        8000000ul,  /* HSE OSC */
        80000000ul, /* PLL output */
        1u,         /* AHB prescaler */
        2u,         /* APB1 prescaler */
        1u,         /* APB2 prescaler */
    };

    /* setup clocks */
    (void)clock_stm32l4x_init(&CLOCK_settings);

    /* enable clock for GPIOs */
    (void)clock_stm32l4x_enable(CLOCK_STM32L4X_CLK_GPIOB);

    return 0;
}

static int mux_init(void)
{
    struct mux PORTB;

    (void)mux_stm32h7xx_init(&PORTB, ADDR_GPIOB);

    (void)mux_stm32h7xx_output(&PORTB, (size_t)4);
    (void)mux_stm32h7xx_output(&PORTB, (size_t)15);
    return 0;
}

static int gpio_init(/*@partial@*/ struct yd_stm32l431rc *ctx)
{
    (void)gpio_stm32h7xx_init(&ctx->TICK, ADDR_GPIOB, (size_t)15);
    (void)gpio_stm32h7xx_init(&ctx->LED, ADDR_GPIOB, (size_t)4);
    return 0;
}

int yd_stm32l431rc_init(struct yd_stm32l431rc *ctx)
{
    (void)clock_init();
    (void)mux_init();
    (void)gpio_init(ctx);

    return 0;
}

#include "stm32f401rct6.h"
#include "picoRTOS_device.h"

#include "clock-stm32f401x.h"
#include "mux-stm32h7xx.h"

static int clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_STM32F401X_SYSCLK_PLL,
        CLOCK_STM32F401X_PLLSRC_HSE,
        25000000ul, /* HSE OSC */
        84000000ul, /* PLL output */
        1u,         /* AHB prescaler */
        2u,         /* APB1 prescaler */
        1u,         /* APB2 prescaler */
    };

    /* setup clocks */
    (void)clock_stm32f401x_init(&CLOCK_settings);

    /* enable clock for GPIOs */
    (void)clock_stm32f401x_enable(CLOCK_STM32F401X_CLK_GPIOB);
    (void)clock_stm32f401x_enable(CLOCK_STM32F401X_CLK_GPIOC);

    return 0;
}

static int mux_init(void)
{
    struct mux PORTB;
    struct mux PORTC;

    (void)mux_stm32h7xx_init(&PORTB, ADDR_GPIOB);
    (void)mux_stm32h7xx_init(&PORTC, ADDR_GPIOC);

    (void)mux_stm32h7xx_output(&PORTB, (size_t)12);
    (void)mux_stm32h7xx_output(&PORTC, (size_t)13);
    return 0;
}

static int gpio_init(/*@partial@*/ struct stm32f401rct6 *ctx)
{
    (void)gpio_stm32h7xx_init(&ctx->TICK, ADDR_GPIOB, (size_t)12);
    (void)gpio_stm32h7xx_init(&ctx->LED, ADDR_GPIOC, (size_t)13);
    return 0;
}

int stm32f401rct6_init(struct stm32f401rct6 *ctx)
{
    (void)clock_init();
    (void)mux_init();
    (void)gpio_init(ctx);

    return 0;
}

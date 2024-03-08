#include "stm32f103xx.h"
#include "picoRTOS_device.h"

#include "clock-stm32f10xxx.h"
#include "mux-stm32f10xxx.h"

static int clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_STM32F10XXX_SYSCLK_PLL,
        CLOCK_STM32F10XXX_PLLSRC_HSE,
        8000000ul,  /* HSE OSC */
        72000000ul, /* PLL output */
        1u,         /* AHB prescaler */
        2u,         /* APB1 prescaler */
        1u,         /* APB2 prescaler */
        2u,         /* ADC prescaler */
    };

    /* setup clocks */
    (void)clock_stm32f10xxx_init(&CLOCK_settings);

    /* enable clock for GPIOs */
    (void)clock_stm32f10xxx_enable(CLOCK_STM32F10XXX_CLK_IOPB);
    (void)clock_stm32f10xxx_enable(CLOCK_STM32F10XXX_CLK_IOPC);

    return 0;
}

static int mux_init(void)
{
    struct mux PORTB;
    struct mux PORTC;

    (void)mux_stm32f10xxx_init(&PORTB, ADDR_GPIO_B);
    (void)mux_stm32f10xxx_init(&PORTC, ADDR_GPIO_C);

    (void)mux_stm32f10xxx_output(&PORTB, (size_t)12, MUX_STM32F10XXX_OUTPUT_PUSH_PULL);
    (void)mux_stm32f10xxx_output(&PORTC, (size_t)13, MUX_STM32F10XXX_OUTPUT_OPEN_DRAIN);
    return 0;
}

static int gpio_init(/*@partial@*/ struct stm32f103xx *ctx)
{
    (void)gpio_stm32f10xxx_init(&ctx->TICK, ADDR_GPIO_B, (size_t)12);
    (void)gpio_stm32f10xxx_init(&ctx->LED, ADDR_GPIO_C, (size_t)13);
    return 0;
}

int stm32f103xx_init(struct stm32f103xx *ctx)
{
    (void)clock_init();
    (void)mux_init();
    (void)gpio_init(ctx);

    return 0;
}

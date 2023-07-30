#include "gpio-stm32h7xx.h"
#include "picoRTOS.h"

#include <stdint.h>

struct GPIO_STM32H7XX {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
};

/* Function: gpio_stm32h7xx_init
 * Initializes a GPIO
 *
 * Parameters:
 *  ctx - The GPIO to init
 *  base - The GPIO PORT base address
 *  pin - The GPIO pin number on that PORT
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int gpio_stm32h7xx_init(struct gpio *ctx, int base, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)GPIO_STM32H7XX_PIN_COUNT)) return -EINVAL;

    ctx->base = (struct GPIO_STM32H7XX*)base;
    ctx->mask = (uint32_t)(1 << pin);
    ctx->invert = false;

    return 0;
}

static int set_invert(struct gpio *ctx, gpio_invert_t invert)
{
    if (!picoRTOS_assert(invert != GPIO_INVERT_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(invert < GPIO_INVERT_COUNT)) return -EINVAL;

    ctx->invert = (invert == GPIO_INVERT_ENABLE);
    return 0;
}

int gpio_setup(struct gpio *ctx, struct gpio_settings *settings)
{
    int res;

    if (settings->invert != GPIO_INVERT_IGNORE &&
        (res = set_invert(ctx, settings->invert)) < 0)
        return res;

    return 0;
}

void gpio_write(struct gpio *ctx, bool value)
{
    if (value ^ ctx->invert) ctx->base->BSRR = ctx->mask;
    else ctx->base->BSRR = ctx->mask << 16;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->base->IDR & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->ODR ^= ctx->mask;
}

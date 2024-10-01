#include "gpio-stm8.h"
#include "picoRTOS.h"

#include <stdint.h>

struct GPIO_STM8 {
    volatile uint8_t ODR;
    volatile uint8_t IDR;
};

int gpio_stm8_init(struct gpio *ctx, int base, size_t pin)
{
    picoRTOS_assert(pin < (size_t)GPIO_STM8_PIN_COUNT, return -EINVAL);

    ctx->base = (struct GPIO_STM8*)base;
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
    if (value ^ ctx->invert) ctx->base->ODR = ctx->mask;
    else ctx->base->ODR &= ~ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->base->IDR & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->ODR ^= ctx->mask;
}

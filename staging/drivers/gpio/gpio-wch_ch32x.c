#include "gpio-wch_ch32x.h"
#include "picoRTOS.h"

#include <stdint.h>

struct GPIO_WCH_CH32X {
    volatile uint32_t CFGLR;
    volatile uint32_t CFGHR;
    volatile uint32_t INDR;
    volatile uint32_t OUTDR;
    volatile uint32_t BSHR;
    volatile uint32_t BCR;
    volatile uint32_t LCKR;
};

int gpio_wch_ch32x_init(struct gpio *ctx, int base, size_t pin)
{
    picoRTOS_assert(pin < (size_t)GPIO_WCH_CH32X_PIN_COUNT, return -EINVAL);

    ctx->base = (struct GPIO_WCH_CH32X*)base;
    ctx->mask = (uint32_t)(1 << pin);
    return 0;
}

static int set_invert(struct gpio *ctx, gpio_invert_t invert)
{
    picoRTOS_assert(invert != GPIO_INVERT_IGNORE, return -EINVAL);
    picoRTOS_assert(invert < GPIO_INVERT_COUNT, return -EINVAL);

    ctx->invert = (invert == GPIO_INVERT_ENABLE);
    return 0;
}

int gpio_setup(struct gpio *ctx, const struct gpio_settings *settings)
{
    int res;

    if (settings->invert != GPIO_INVERT_IGNORE &&
        (res = set_invert(ctx, settings->invert)) < 0)
        return res;

    return 0;
}

void gpio_write(struct gpio *ctx, bool value)
{
    if (value ^ ctx->invert) ctx->base->BSHR = ctx->mask;
    else ctx->base->BSHR = ctx->mask << 16;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->base->INDR & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->OUTDR ^= ctx->mask;
}

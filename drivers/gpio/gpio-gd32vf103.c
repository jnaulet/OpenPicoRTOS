#include "gpio-gd32vf103.h"
#include "picoRTOS.h"

#include <stdint.h>

struct GPIO_GD32VF103 {
    volatile uint32_t GPIOx_CTLn[2];
    volatile uint32_t GPIOx_ISTAT;
    volatile uint32_t GPIOx_OCTL;
    volatile uint32_t GPIOx_BOP;
    volatile uint32_t GPIOx_BC;
    volatile uint32_t GPIOx_LOCK;
};

/* Function: gpio_gd32vf103_init
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
int gpio_gd32vf103_init(struct gpio *ctx, int base, size_t pin)
{
    picoRTOS_assert(pin < (size_t)GPIO_GD32VF103_PIN_COUNT,
                    return -EINVAL);

    ctx->base = (struct GPIO_GD32VF103*)base;
    ctx->mask = (uint32_t)(1 << pin);
    ctx->invert = false;

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
    if (value ^ ctx->invert) ctx->base->GPIOx_BOP = ctx->mask;
    else ctx->base->GPIOx_BC = ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->base->GPIOx_ISTAT & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->GPIOx_OCTL ^= ctx->mask;
}

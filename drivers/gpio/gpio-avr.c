#include "gpio-avr.h"
#include "picoRTOS.h"

#include <stdint.h>

struct GPIO_AVR {
    volatile uint8_t PIN;
    volatile uint8_t RESERVED;
    volatile uint8_t PORT;
};

/* Function: gpio_avr_init
 * Initializes a GPIO
 *
 * Parameters:
 *  ctx - The GPIO to init
 *  base - The GPIO block base address
 *  pin - The GPIO pin number on that block
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int gpio_avr_init(struct gpio *ctx, int base, size_t pin)
{
    picoRTOS_assert(pin < (size_t)8, return -EINVAL);

    ctx->base = (struct GPIO_AVR*)base;
    ctx->mask = (uint8_t)(1 << pin);
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
    if (value ^ ctx->invert) ctx->base->PORT |= ctx->mask;
    else ctx->base->PORT &= ~ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->base->PIN & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->PORT ^= ctx->mask;
}

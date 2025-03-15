#include "gpio-pic32mx.h"
#include "picoRTOS.h"

#include <stdint.h>
#include "misc-pic32mx.h"

struct GPIO_PIC32MX {
    struct PIC32MX_CLR_SET_INV RESERVED0[2];
    struct PIC32MX_CLR_SET_INV PORT;
    struct PIC32MX_CLR_SET_INV LAT;
};

/* Function: gpio_pic32mx_init
 * Initializes a GPIO
 *
 * Parameters:
 *  ctx - The GPIO context to init
 *  base - The GPIO port base address
 *  pin - The GPIO port pin number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int gpio_pic32mx_init(struct gpio *ctx, int base, size_t pin)
{
    picoRTOS_assert(pin < (size_t)GPIO_PIC32MX_PIN_COUNT, return -EINVAL);

    ctx->base = (struct GPIO_PIC32MX*)base;
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
    if (value ^ ctx->invert) ctx->base->LAT.SET = ctx->mask;
    else ctx->base->LAT.CLR = ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->base->PORT.REG & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->LAT.INV = ctx->mask;
}

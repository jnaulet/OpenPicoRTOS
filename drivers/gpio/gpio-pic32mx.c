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
    if (!picoRTOS_assert(pin < (size_t)GPIO_PIC32MX_PIN_COUNT)) return -EINVAL;

    ctx->base = (struct GPIO_PIC32MX*)base;
    ctx->mask = (uint32_t)(1 << pin);

    return 0;
}

void gpio_write(struct gpio *ctx, bool value)
{
    if (value) ctx->base->LAT.SET = ctx->mask;
    else ctx->base->LAT.CLR = ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return (ctx->base->PORT.REG & ctx->mask) != 0;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->LAT.INV = ctx->mask;
}

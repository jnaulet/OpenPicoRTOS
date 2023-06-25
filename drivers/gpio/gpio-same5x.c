#include "gpio-same5x.h"
#include "picoRTOS.h"

#include <stdio.h>

struct GPIO_SAME5X {
    volatile uint32_t DIR;
    volatile uint32_t DIRCLR;
    volatile uint32_t DIRSET;
    volatile uint32_t DIRTGL;
    volatile uint32_t OUT;
    volatile uint32_t OUTCLR;
    volatile uint32_t OUTSET;
    volatile uint32_t OUTTGL;
    volatile uint32_t IN;
    /* ignore other fields */
};

/* Function: gpio_same5x_init
 * Initializes a GPIO
 *
 * Parameters:
 *  ctx - The GPIO to init
 *  base - The GPIO PORT group base address
 *  pin - The GPIO pin number on that PORT group
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int gpio_same5x_init(struct gpio *ctx, int base, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)GPIO_SAME5X_PIN_COUNT)) return -EINVAL;

    ctx->base = (struct GPIO_SAME5X*)base;
    ctx->mask = (uint32_t)1 << pin;
    ctx->invert = false;

    return 0;
}

int gpio_setup(struct gpio *ctx, struct gpio_settings *settings)
{
    ctx->invert = settings->invert;
    return 0;
}

void gpio_write(struct gpio *ctx, bool value)
{
    if (value ^ ctx->invert) ctx->base->OUTSET = ctx->mask;
    else ctx->base->OUTCLR = ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->base->IN & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->OUTTGL = ctx->mask;
}

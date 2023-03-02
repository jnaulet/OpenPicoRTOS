#include "gpio-tinyavr.h"
#include "picoRTOS.h"

#include <stdint.h>

struct GPIO_TINYAVR {
    volatile uint8_t DIR;
    volatile uint8_t DIRSET;
    volatile uint8_t DIRCLR;
    volatile uint8_t DIRTGL;
    volatile uint8_t OUT;
    volatile uint8_t OUTSET;
    volatile uint8_t OUTCLR;
    volatile uint8_t OUTTGL;
    volatile uint8_t IN;
    volatile uint8_t INTFLAGS;
    uint8_t RESERVED0[6];
    volatile uint8_t PINnCTRL[GPIO_TINYAVR_PIN_COUNT];
};

/* Function: gpio_tinyavr_init
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
int gpio_tinyavr_init(struct gpio *ctx,
                      struct GPIO_TINYAVR *base,
                      size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)GPIO_TINYAVR_PIN_COUNT)) return -EINVAL;

    ctx->base = base;
    ctx->pin = pin;
    ctx->mask = (uint8_t)(1 << pin);

    return 0;
}

void gpio_write(struct gpio *ctx, bool value)
{
    if (value) ctx->base->OUTSET = ctx->mask;
    else ctx->base->OUTCLR = ctx->mask;

    ctx->base->DIRSET = ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    ctx->base->DIRCLR = ctx->mask;
    return (ctx->base->IN & ctx->mask) != 0;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->OUTTGL = ctx->mask;
    ctx->base->DIRSET = ctx->mask;
}

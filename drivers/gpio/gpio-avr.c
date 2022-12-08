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
int gpio_avr_init(struct gpio *ctx, struct GPIO_AVR *base, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)8)) return -EINVAL;

    ctx->base = base;
    ctx->pin = pin;

    return 0;
}

/* HAL */

void gpio_write(struct gpio *ctx, bool value)
{
    if (value) ctx->base->PORT |= (1 << ctx->pin);
    else ctx->base->PORT &= ~(1 << ctx->pin);
}

bool gpio_read(struct gpio *ctx)
{
    uint8_t mask = (uint8_t)(1 << ctx->pin);

    return (ctx->base->PIN & mask) != 0;
}

void gpio_toggle(struct gpio *ctx)
{
    uint8_t mask = (uint8_t)(1 << ctx->pin);

    if ((ctx->base->PIN & mask) == 0) ctx->base->PORT |= mask;
    else ctx->base->PORT &= ~mask;
}

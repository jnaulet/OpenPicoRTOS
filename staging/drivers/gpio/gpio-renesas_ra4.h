#ifndef GPIO_RENESAS_RA4_H
#define GPIO_RENESAS_RA4_H

#include "gpio.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define GPIO_RENESAS_RA4_PIN_COUNT 16

struct GPIO_RENESAS_RA4;

struct gpio {
    /*@temp@*/ struct GPIO_RENESAS_RA4 *base;
    uint32_t mask;
    bool invert;
};

int gpio_renesas_ra4_init(/*@out@*/ struct gpio *ctx, int base, size_t pin);

/* Runtime calls:
 * int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

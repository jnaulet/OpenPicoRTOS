#ifndef GPIO_NXP_SIUL2_H
#define GPIO_NXP_SIUL2_H

#include "gpio.h"
#include <stdint.h>
#include <stddef.h>

struct GPIO_NXP_SIUL2;

struct gpio {
    /*@temp@*/ struct GPIO_NXP_SIUL2 *base;
    uint16_t mask;
    bool invert;
};

#define GPIO_NXP_SIUL2_PIN_COUNT 16

int gpio_nxp_siul2_init(/*@out@*/ struct gpio *ctx, int base, size_t pin);

/* Runtime calls:
 * int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

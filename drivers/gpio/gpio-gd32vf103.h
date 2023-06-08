#ifndef GPIO_GD32VF103_H
#define GPIO_GD32VF103_H

#include "gpio.h"
#include <stddef.h>
#include <stdint.h>

#define GPIO_GD32VF103_PIN_COUNT 16

struct GPIO_GD32VF103;

struct gpio {
    /*@temp@*/ struct GPIO_GD32VF103 *base;
    uint32_t mask;
};

int gpio_gd32vf103_init(/*@out@*/ struct gpio *ctx, int base, size_t pin);

/* Implements:
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

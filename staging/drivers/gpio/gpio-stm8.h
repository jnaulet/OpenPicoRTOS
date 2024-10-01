#ifndef GPIO_STM8_H
#define GPIO_STM8_H

#include "gpio.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define GPIO_STM8_PIN_COUNT 8

struct GPIO_STM8;

struct gpio {
    /*@temp@*/ struct GPIO_STM8 *base;
    uint8_t mask;
    bool invert;
};

int gpio_stm8_init(/*@out@*/ struct gpio *ctx, int base, size_t pin);

/* Runtime calls:
 * int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

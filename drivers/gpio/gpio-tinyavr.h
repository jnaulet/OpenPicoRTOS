#ifndef GPIO_TINYAVR_H
#define GPIO_TINYAVR_H

/* GPIO driver for tinyAVR 1-series */

#include "gpio.h"
#include <stdint.h>
#include <stddef.h>

#define GPIO_TINYAVR_PIN_COUNT 8

struct GPIO_TINYAVR;

struct gpio {
    /*@temp@*/ struct GPIO_TINYAVR *base;
    size_t pin;
    uint8_t mask;
};

int gpio_tinyavr_init(/*@out@*/ struct gpio *ctx, int base, size_t pin);

/* Runtime calls:
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

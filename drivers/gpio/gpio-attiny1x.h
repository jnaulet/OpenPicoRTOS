#ifndef GPIO_ATTINY1X_H
#define GPIO_ATTINY1X_H

/* GPIO driver for tinyAVR 1-series */

#include "gpio.h"
#include <stdint.h>
#include <stddef.h>

#define GPIO_ATTINY1X_PIN_COUNT 8

struct GPIO_ATTINY1X;

struct gpio {
    /*@temp@*/ struct GPIO_ATTINY1X *base;
    size_t pin;
    uint8_t mask;
};

int gpio_attiny1x_init(/*@out@*/ struct gpio *ctx,
                       struct GPIO_ATTINY1X *base,
                       size_t pin);

/* Runtime calls:
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

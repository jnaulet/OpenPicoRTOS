#ifndef GPIO_AVR_H
#define GPIO_AVR_H

#include "gpio.h"
#include <stddef.h>

struct GPIO_AVR;

struct gpio {
    /*@temp@*/ struct GPIO_AVR *base;
    size_t pin;
};

int gpio_avr_init(/*@out@*/ struct gpio *ctx, int base, size_t pin);

/* Runtime calls:
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

#ifndef GPIO_ATMEL_PIO_H
#define GPIO_ATMEL_PIO_H

#include "gpio.h"
#include <stdint.h>

struct GPIO_ATMEL_PIO;

#define GPIO_ATMEL_PIO_PIN_COUNT 32

struct gpio {
    /*@temp@*/ struct GPIO_ATMEL_PIO *base;
    uint32_t mask;
    bool invert;
};

int gpio_atmel_pio_init(/*@out@*/ struct gpio *ctx, int base, size_t pin);

/* Runtime calls:
 * int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

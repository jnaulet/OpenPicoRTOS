#ifndef GPIO_SAM_PIO_H
#define GPIO_SAM_PIO_H

#include "gpio.h"
#include <stdint.h>

struct GPIO_SAM_PIO;

#define GPIO_SAM_PIO_PIN_COUNT 32

struct gpio {
    /*@temp@*/ struct GPIO_SAM_PIO *base;
    uint32_t mask;
};

int gpio_sam_pio_init(/*@out@*/ struct gpio *ctx, struct GPIO_SAM_PIO *base, size_t pin);

/* Runtime calls:
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

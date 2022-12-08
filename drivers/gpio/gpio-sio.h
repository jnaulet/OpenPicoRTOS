#ifndef GPIO_SIO_H
#define GPIO_SIO_H

#include "gpio.h"
#include <stddef.h>
#include <stdint.h>

struct GPIO_SIO;

#define GPIO_SIO_PIN_COUNT 30

struct gpio {
    /*@temp@*/ struct GPIO_SIO *base;
    uint32_t mask;
};

typedef enum {
    GPIO_SIO_DIR_INPUT,
    GPIO_SIO_DIR_OUTPUT,
    GPIO_SIO_DIR_COUNT
} gpio_sio_dir_t;

int gpio_sio_init(/*@out@*/ struct gpio *ctx, struct GPIO_SIO *base, size_t pin, gpio_sio_dir_t dir);

/* Runtime calls:
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

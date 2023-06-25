#ifndef GPIO_RP2040_SIO_H
#define GPIO_RP2040_SIO_H

#include "gpio.h"
#include <stddef.h>
#include <stdint.h>

struct GPIO_RP2040_SIO;

#define GPIO_RP2040_SIO_PIN_COUNT 30

struct gpio {
    /*@temp@*/ struct GPIO_RP2040_SIO *base;
    uint32_t mask;
    bool invert;
};

typedef enum {
    GPIO_RP2040_SIO_DIR_INPUT,
    GPIO_RP2040_SIO_DIR_OUTPUT,
    GPIO_RP2040_SIO_DIR_COUNT
} gpio_rp2040_sio_dir_t;

int gpio_rp2040_sio_init(/*@out@*/ struct gpio *ctx, int base,
                         size_t pin, gpio_rp2040_sio_dir_t dir);

/* Runtime calls:
 * int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

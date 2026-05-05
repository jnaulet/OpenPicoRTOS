#ifndef GPIO_NUMICRO_H
#define GPIO_NUMICRO_H

#include "gpio.h"

#include <stdint.h>
#include <stddef.h>

struct gpio {
    size_t port;
    uint8_t mask;
    bool invert;
};

#define GPIO_NUMICRO_PORT_COUNT 4
#define GPIO_NUMICRO_PIN_COUNT  8

int gpio_numicro_init(/*@out@*/ struct gpio *ctx, size_t port, size_t pin);

/* Runtime calls:
 * int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

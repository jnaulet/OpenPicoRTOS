#ifndef GPIO_MC9S08PTXX_H
#define GPIO_MC9S08PTXX_H

#include "gpio.h"

#include <stdint.h>
#include <stddef.h>

struct gpio {
    size_t port;
    uint8_t mask;
    bool invert;
};

#define GPIO_MC9S08PTXX_PORT_COUNT 8
#define GPIO_MC9S08PTXX_PIN_COUNT  8

#define GPIO_MC9S08PTXX_PORT_A 0
#define GPIO_MC9S08PTXX_PORT_B 1
#define GPIO_MC9S08PTXX_PORT_C 2
#define GPIO_MC9S08PTXX_PORT_D 3
#define GPIO_MC9S08PTXX_PORT_E 4
#define GPIO_MC9S08PTXX_PORT_F 5
#define GPIO_MC9S08PTXX_PORT_G 6
#define GPIO_MC9S08PTXX_PORT_H 7

int gpio_mc9s08ptxx_init(/*@out@*/ struct gpio *ctx, size_t port, size_t pin);

/* Runtime calls:
 * int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

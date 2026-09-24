#ifndef GPIO_MSP430G2XXX_H
#define GPIO_MSP430G2XXX_H

#include "gpio.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define GPIO_MSP430G2XXX_PORT_COUNT 8
#define GPIO_MSP430G2XXX_PIN_COUNT 8

struct GPIO_MSP430G2XXX;

struct gpio {
    /*@temp@*/ volatile uint8_t *PxIN;
    /*@temp@*/ volatile uint8_t *PxOUT;
    uint8_t mask;
    bool invert;
};

int gpio_msp430g2xxx_init(/*@out@*/ struct gpio *ctx, int base, size_t port, size_t pin);

/* Runtime calls:
 * int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

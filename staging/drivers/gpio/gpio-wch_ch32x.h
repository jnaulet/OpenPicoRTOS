#ifndef GPIO_WCH_CH32X_H
#define GPIO_WCH_CH32X_H

#include "gpio.h"
#include <stddef.h>
#include <stdint.h>

#define GPIO_WCH_CH32X_PIN_COUNT 16

struct GPIO_WCH_CH32X;

struct gpio {
    /*@temp@*/ struct GPIO_WCH_CH32X *base;
    uint32_t mask;
    bool invert;
};

int gpio_wch_ch32x_init(/*@out@*/ struct gpio *ctx, int base, size_t pin);

/* Runtime calls:
 * int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

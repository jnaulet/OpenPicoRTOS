#ifndef GPIO_NXP_SIU_H
#define GPIO_NXP_SIU_H

#include "gpio.h"
#include <stdint.h>
#include <stddef.h>

#define GPIO_NXP_SIU_PIN_COUNT 32

struct GPIO_NXP_SIU;

struct gpio {
    /*@temp@*/ struct GPIO_NXP_SIU *base;
    uint32_t mask;
    bool invert;
};

int gpio_nxp_siu_init(/*@out@*/ struct gpio *ctx, int base, size_t pin);

/* Runtime calls:
 * int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

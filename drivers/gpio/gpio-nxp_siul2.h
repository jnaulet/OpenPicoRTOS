#ifndef GPIO_NXP_SIUL2_H
#define GPIO_NXP_SIUL2_H

#include "gpio+safety.h"
#include <stdint.h>
#include <stddef.h>

struct GPIO_NXP_SIUL2;

struct gpio_nxp_siul2 {
    /*@temp@*/ struct GPIO_NXP_SIUL2 *base;
};

#define GPIO_NXP_SIUL2_PIN_COUNT 16

int gpio_nxp_siul2_init(/*@out@*/ struct gpio_nxp_siul2 *ctx, int base);

struct gpio {
    /*@temp@*/ struct gpio_nxp_siul2 *parent;
    uint16_t mask;
    bool invert;
};

int gpio_nxp_siul2_gpio_init(/*@out@*/ struct gpio *ctx, struct gpio_nxp_siul2 *parent, size_t pin);

/* Runtime calls:
 * int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 * struct gpio *gpio_claim(struct gpio *ctx);
 */

#endif

#ifndef GPIO_SAME5X_H
#define GPIO_SAME5X_H

#include "gpio.h"
#include <stdint.h>

struct GPIO_SAME5X; /* PORT */

#define GPIO_SAME5X_PIN_COUNT 32

struct gpio {
    /*@temp@*/ struct GPIO_SAME5X *base;
    uint32_t mask;
};

int gpio_same5x_init(/*@out@*/ struct gpio *ctx, struct GPIO_SAME5X *base, size_t pin);

/* Runtime calls:
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

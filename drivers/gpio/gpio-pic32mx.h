#ifndef GPIO_PIC32MX_H
#define GPIO_PIC32MX_H

#include "gpio.h"
#include <stdint.h>
#include <stddef.h>

struct GPIO_PIC32MX; /* PORT */

#define GPIO_PIC32MX_PIN_COUNT 16

struct gpio {
    /*@temp@*/ struct GPIO_PIC32MX *base;
    uint32_t mask;
};

int gpio_pic32mx_init(/*@out@*/ struct gpio *ctx, int base, size_t pin);

/* Runtime calls:
 * void gpio_write(struct gpio *ctx, bool value);
 * bool gpio_read(struct gpio *ctx);
 * void gpio_toggle(struct gpio *ctx);
 */

#endif

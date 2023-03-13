#ifndef GPIO_TI_F28X_H
#define GPIO_TI_F28X_H

/* GPIO driver for TI Ti_F28x family */

#include "gpio.h"
#include <stdint.h>
#include <stddef.h>

#define GPIO_TI_F28X_PIN_COUNT 32

struct GPIO_DATA_REGS;

struct gpio {
    uint32_t mask;
    /*@temp@*/ struct GPIO_DATA_REGS *base;
};

int gpio_ti_f28x_init(/*@out@*/ struct gpio *ctx, struct GPIO_DATA_REGS *base, size_t pin);

#endif

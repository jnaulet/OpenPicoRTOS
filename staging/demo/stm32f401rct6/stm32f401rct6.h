#ifndef STM32F401RCT6_H
#define STM32F401RCT6_H

#include "gpio-stm32h7xx.h"

struct stm32f401rct6 {
    struct gpio TICK;
    struct gpio LED;
};

int stm32f401rct6_init(/*@out@*/ struct stm32f401rct6 *ctx);

#endif

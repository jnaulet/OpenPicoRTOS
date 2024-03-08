#ifndef STM32F103XX_H
#define STM32F103XX_H

#include "gpio-stm32f10xxx.h"

struct stm32f103xx {
    struct gpio TICK;
    struct gpio LED;
};

int stm32f103xx_init(/*@out@*/ struct stm32f103xx *ctx);

#endif

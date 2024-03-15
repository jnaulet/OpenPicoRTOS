#ifndef YD_STM32L431RC_H
#define YD_STM32L431RC_H

#include "gpio-stm32h7xx.h"

struct yd_stm32l431rc {
    struct gpio TICK;
    struct gpio LED;
};

int yd_stm32l431rc_init(/*@out@*/ struct yd_stm32l431rc *ctx);

#endif

#ifndef LONGAN_NANO_H
#define LONGAN_NANO_H

#include "gpio-gd32vf103.h"

struct longan_nano {
    struct gpio TICK;
    struct gpio LED_R;
    struct gpio LED_G;
    struct gpio LED_B;
};

int longan_nano_init(/*@out@*/ struct longan_nano *ctx);

#endif

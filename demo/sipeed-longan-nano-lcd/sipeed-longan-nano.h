#ifndef SIPEED_LONGAN_NANO_H
#define SIPEED_LONGAN_NANO_H

#include "gpio-gd32vf103.h"
#include "spi-gd32vf103.h"

#include "lcd.h"

struct sipeed_longan_nano {
    struct led {
        struct gpio R;
        struct gpio G;
        struct gpio B;
    } LED;
    /* for lcd */
    struct lcd_phys lcd_phys;
};

int sipeed_longan_nano_init(/*@out@*/ struct sipeed_longan_nano *ctx);

#endif

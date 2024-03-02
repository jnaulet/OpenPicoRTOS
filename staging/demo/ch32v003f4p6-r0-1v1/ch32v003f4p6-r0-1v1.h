#ifndef CH32V00F4P6_R0_1V1_H
#define CH32V00F4P6_R0_1V1_H

#include "gpio-wch_ch32x.h"

struct ch32v003f4p6 {
    struct gpio TICK;
    struct gpio LED1;
};

int ch32v003f4p6_init(/*@out@*/ struct ch32v003f4p6 *ctx);

#endif

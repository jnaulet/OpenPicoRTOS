#ifndef MSP430G2553_H
#define MSP430G2553_H

#include "gpio-msp430g2xxx.h"

struct msp430g2553 {
    struct gpio TICK;
    struct gpio LED1;
    struct gpio LED2;
};

int msp430g2553_init(/*@out@*/ struct msp430g2553 *ctx);

#endif

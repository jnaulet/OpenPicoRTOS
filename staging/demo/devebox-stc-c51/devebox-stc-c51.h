#ifndef DEVEBOX_STC_C51_H
#define DEVEBOX_STC_C51_H

#include "gpio-8051.h"
#include "uart-8051_irqdriven.h"

struct devebox_stc_c51 {
    struct gpio TICK;
    struct gpio D2;
    struct gpio R;
    struct gpio T;
    /* serial */
    struct uart UART;
};

int devebox_stc_c51_init(/*@out@*/ struct devebox_stc_c51 *ctx);

#endif

#ifndef MH_TINY_H
#define MH_TINY_H

#include "gpio-avr.h"
#include "mux-avr.h"
#include "spi-avr_irqdriven.h"
#include "wd-avr.h"

struct mh_tiny {
    struct gpio TICK;
    struct gpio LED;
    // struct uart UART;
    struct spi SPI;
    struct wd WD;
};

int mh_tiny_init(/*@out@*/ struct mh_tiny *ctx);

#endif

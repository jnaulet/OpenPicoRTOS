#ifndef MS51PC0AE_H
#define MS51PC0AE_H

#include "gpio-n76e003.h"
#include "uart-n76e003.h"
#include "pwm-n76e003.h"

struct ms51pc0ae {
    struct gpio TICK;
    struct gpio D2;
    /* uart */
    struct uart UART0;
    /* Relays */
    struct gpio OUT0;
    struct gpio OUT1;
    /* PWMs */
    struct pwm PWM;
};

int ms51pc0ae_init(/*@out@*/ struct ms51pc0ae *ctx);

#endif

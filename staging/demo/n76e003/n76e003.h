#ifndef N76E003_H
#define N76E003_H

#include "gpio-n76e003.h"
#include "uart-n76e003.h"
#include "pwm-n76e003.h"

struct n76e003 {
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

int n76e003_init(/*@out@*/ struct n76e003 *ctx);

#endif

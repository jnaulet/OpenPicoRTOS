#ifndef ATTINY817_XPLAINED_H
#define ATTINY817_XPLAINED_H

#include "adc-attiny1x.h"
#include "gpio-attiny1x.h"
#include "spi-attiny1x.h"
#include "uart-attiny1x_usart.h"
#include "wd-attiny1x.h"
#include "pwm-attiny1x_tca.h"

struct attiny817_xplained {
    struct gpio USER_BUTTON;
    struct gpio USER_LED;
    struct uart UART;
    struct spi SPI;
    struct adc AIN04;
    struct wd WDT;
    /* special case for PWM */
    struct attiny817_xplained_PWM {
        struct pwm WO0;
        struct pwm WO1;
    } PWM;
};

int attiny817_xplained_init(/*@out@*/ struct attiny817_xplained *ctx);

#endif

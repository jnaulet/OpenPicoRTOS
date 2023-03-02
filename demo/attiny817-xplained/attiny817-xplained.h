#ifndef ATTINY817_XPLAINED_H
#define ATTINY817_XPLAINED_H

#include "adc-tinyavr.h"
#include "gpio-tinyavr.h"
#include "spi-tinyavr.h"
#include "uart-tinyavr_usart.h"
#include "wd-tinyavr.h"
#include "pwm-tinyavr_tca.h"

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

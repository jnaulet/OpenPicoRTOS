#ifndef ATTINY1607_CNANO_H
#define ATTINY1607_CNANO_H

#include "adc-tinyavr.h"
#include "gpio-tinyavr.h"
#include "spi-tinyavr.h"
#include "uart-tinyavr_usart.h"
#include "wd-tinyavr.h"
#include "pwm-tinyavr_tca.h"

struct attiny1607_cnano {
    struct gpio SW0;
    struct pwm LED0;
    struct uart UART;
    struct spi SPI;
    struct adc AIN04;
    struct wd WDT;
};

int attiny1607_cnano_init(/*@out@*/ struct attiny1607_cnano *ctx);

#endif

#ifndef ARDUINO_UNO_R4_H
#define ARDUINO_UNO_R4_H

#include "adc-renesas_ra4.h"
#include "gpio-renesas_ra4.h"
#include "pwm-renesas_ra4_gpt.h"
#include "spi-renesas_ra4.h"
#include "twi-renesas_ra4.h"
#include "uart-renesas_ra4_sci.h"

struct arduino_uno_r4 {
    struct gpio TICK;
    struct spi SPI;
    struct pwm L;
    struct uart UART;
    struct adc A0;
    struct twi I2C;
};

int arduino_uno_r4_init(/*@out@*/ struct arduino_uno_r4 *ctx);

#endif

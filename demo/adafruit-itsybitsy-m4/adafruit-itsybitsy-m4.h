#ifndef ADAFRUIT_ITSYBITSY_M4_H
#define ADAFRUIT_ITSYBITSY_M4_H

#include "adc-same5x.h"
#include "flash-same5x.h"
#include "gpio-same5x.h"
#include "spi-atmel_sercom.h"
#include "uart-atmel_sercom.h"
#include "pwm-same5x_tc.h"
#include "twi-atmel_sercom.h"
#include "wd-same5x.h"

struct adafruit_itsybitsy_m4 {
    struct gpio RED;
    struct spi SPI;
    struct uart UART;
    struct pwm D5;
    struct adc ADC;
    struct wd WDT;
    struct flash FLASH;
    struct twi I2C;
};

int adafruit_itsybitsy_m4_init(/*@out@*/ struct adafruit_itsybitsy_m4 *ctx);

#endif

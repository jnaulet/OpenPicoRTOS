#ifndef ARDUINO_UNO_H
#define ARDUINO_UNO_H

#include "picoRTOS_device.h"

/* Typical Arduino Uno hw layout
 *
 * Provided as an example, here are a few key features:
 * - A0-A3 read values in mV
 * - A4-A5 are used as I2C
 * - DIGITALs alternate as input and outputs, DIGITAL2 is input, DIGITAL4 is output, etc
 * - DIGITALs 10-13 are used as SPI
 * - cannot use PWM5 & 6 as their are wired on TC0, which is used by picoRTOS
 * - SPI is master, 1x speed and mode 0
 * - Watchdog is set to the shortest possible, 2k cycles, roughly 16ms
 */

#include "adc-avr.h"
#include "clock-atmega328p.h"
#include "eeprom-avr.h"
#include "gpio-avr.h"
#include "mux-avr.h"
#include "pwm-avr.h"
#include "spi-avr.h"
#include "twi-avr.h"
#include "uart-avr.h"
#include "wd-avr.h"

struct arduino_uno {
    /* ANALOG IN */
    struct adc A0;
    struct adc A1;
    struct adc A2;
    struct adc A3;
    /* DIGITAL: RX/TX */
    struct uart SERIAL;
    /* DIGITAL */
    struct gpio DIGITAL2;
    struct gpio DIGITAL4;
    struct gpio DIGITAL6;
    struct gpio DIGITAL7;
    struct gpio DIGITAL8;
    /* DIGITAL: SPI */
    struct spi SPI;
    /* PWM~ */
    struct pwm PWM3;
    struct pwm PWM9;
    /* MISC */
    struct twi I2C;
    /* INTERNALS */
    struct eeprom C99_EEPROM;
    struct wd WDT;
};

int arduino_uno_init(/*@out@*/ struct arduino_uno *ctx);

#endif

#ifndef ARDUINO_MEGA2560_H
#define ARDUINO_MEGA2560_H

#include "picoRTOS_device.h"

/* Typical Arduino Mega2560 hw layout
 *
 * Provided as an example, here are a few key features:
 * - ADCs 0-3 are calibrated to read mV (roughly), others are raw
 * - ADCs 4-7 are unavailable (JTAG signals)
 * - DIGITAL I/Os are set to input for even numbers and output for odd numbers
 * - PWM4 is unusable as it runs on TC0, which is picoRTOS' main timer
 * - SPI is master, 1x speed and mode 0
 * - Watchdog is set to 2nd shortest possible, 4k cycles,
 *   roughly 32ms (32x over what's needed)
 */

#include "adc-avr.h"
#include "clock-atmega2560.h"
#include "eeprom-avr.h"
#include "gpio-avr.h"
#include "mux-avr.h"
#include "pwm-avr.h"
#include "spi-avr.h"
#include "twi-avr.h"
#include "uart-avr.h"
#include "wd-avr.h"

struct arduino_mega2560 {
    /* ANALOG IN */
    struct adc A0;
    struct adc A1;
    struct adc A2;
    struct adc A3;
    /* struct adc A4; */
    /* struct adc A5; */
    /* struct adc A6; */
    /* struct adc A7; */
    struct adc A8;
    struct adc A9;
    struct adc A10;
    struct adc A11;
    struct adc A12;
    struct adc A13;
    struct adc A14;
    struct adc A15;
    /* DIGITAL */
    struct gpio DIGITAL22;
    struct gpio DIGITAL23;
    struct gpio DIGITAL24;
    struct gpio DIGITAL25;
    struct gpio DIGITAL26;
    struct gpio DIGITAL27;
    struct gpio DIGITAL28;
    struct gpio DIGITAL29;
    struct gpio DIGITAL30;
    struct gpio DIGITAL31;
    struct gpio DIGITAL32;
    struct gpio DIGITAL33;
    struct gpio DIGITAL34;
    struct gpio DIGITAL35;
    struct gpio DIGITAL36;
    struct gpio DIGITAL37;
    struct gpio DIGITAL38;
    struct gpio DIGITAL39;
    struct gpio DIGITAL40;
    struct gpio DIGITAL41;
    struct gpio DIGITAL42;
    struct gpio DIGITAL43;
    struct gpio DIGITAL44;
    struct gpio DIGITAL45;
    struct gpio DIGITAL46;
    struct gpio DIGITAL47;
    struct gpio DIGITAL48;
    struct gpio DIGITAL49;
    /* COMMUNICATION */
    struct uart SERIAL0;
    /* struct usart SERIAL3 */
    /* struct usart SERIAL2 */
    /* struct usart SERIAL1 */
    struct twi I2C;
    /* PWM */
    struct pwm PWM2;
    struct pwm PWM3;
    /* struct pwm PWM4; */
    struct pwm PWM5;
    struct pwm PWM6;
    struct pwm PWM7;
    struct pwm PWM8;
    struct pwm PWM9;
    struct pwm PWM10;
    struct pwm PWM11;
    struct pwm PWM12;
    struct pwm PWM13;
    /* ICSP */
    struct spi SPI;
    /* EEPROM */
    struct eeprom C99_EEPROM;
    /* WATCHDOG */
    struct wd WDT;
};

int arduino_mega2560_init(/*@out@*/ struct arduino_mega2560 *ctx);

#endif

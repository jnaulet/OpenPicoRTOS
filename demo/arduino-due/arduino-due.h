#ifndef ARDUINO_DUE_H
#define ARDUINO_DUE_H

#include "picoRTOS_device.h"

/* Typical arduino DUE hw layout
 *
 * Provided as an example, here are a few key features:
 * - ADCs are calibrated to read mV
 * - DIGITAL I/Os are set to input for even numbers and output for odd numbers
 * - PWMs 6 to 10 are not available due to limitations in the static HAL,
 *   you can have PWMs or TCs as PWMs but not both at the same time.
 * - PWM3 is set as IPWM
 * - SPI is master, 1Mbit, 8bit frame size and mode 0
 * - I2C is master, I2C1 is slave
 * - Watchdog is 4ms (shortest possible on sam3x8e)
 */

#include "adc.h"
#include "can.h"
#include "gpio.h"
#include "pwm.h"
#include "ipwm.h"
#include "spi.h"
#include "twi.h"
#include "uart.h"
#include "wd.h"

/* These drivers could be instanciated directly, but this is a demo of picoRTOS' HAL
 * capabilities, so this is fully abstracted using pointers */
struct arduino_due {
    /* ANALOG IN */
    /*@shared@*/ struct adc *A0;
    /*@shared@*/ struct adc *A1;
    /*@shared@*/ struct adc *A2;
    /*@shared@*/ struct adc *A3;
    /*@shared@*/ struct adc *A4;
    /*@shared@*/ struct adc *A5;
    /*@shared@*/ struct adc *A6;
    /*@shared@*/ struct adc *A7;
    /*@shared@*/ struct adc *A8;
    /*@shared@*/ struct adc *A9;
    /*@shared@*/ struct adc *A10;
    /*@shared@*/ struct adc *A11;
    /* DAC */
    /*@shared@*/ /* struct dac *DAC1 */
    /*@shared@*/ /* struct dac *DAC2 */
    /* CAN */
    /*@shared@*/ struct can *CAN0;
    /*@shared@*/ struct can *CAN1;
    /* DIGITAL */
    /*@shared@*/ struct gpio *DIGITAL22;
    /*@shared@*/ struct gpio *DIGITAL23;
    /*@shared@*/ struct gpio *DIGITAL24;
    /*@shared@*/ struct gpio *DIGITAL25;
    /*@shared@*/ struct gpio *DIGITAL26;
    /*@shared@*/ struct gpio *DIGITAL27;
    /*@shared@*/ struct gpio *DIGITAL28;
    /*@shared@*/ struct gpio *DIGITAL29;
    /*@shared@*/ struct gpio *DIGITAL30;
    /*@shared@*/ struct gpio *DIGITAL31;
    /*@shared@*/ struct gpio *DIGITAL32;
    /*@shared@*/ struct gpio *DIGITAL33;
    /*@shared@*/ struct gpio *DIGITAL34;
    /*@shared@*/ struct gpio *DIGITAL35;
    /*@shared@*/ struct gpio *DIGITAL36;
    /*@shared@*/ struct gpio *DIGITAL37;
    /*@shared@*/ struct gpio *DIGITAL38;
    /*@shared@*/ struct gpio *DIGITAL39;
    /*@shared@*/ struct gpio *DIGITAL40;
    /*@shared@*/ struct gpio *DIGITAL41;
    /*@shared@*/ struct gpio *DIGITAL42;
    /*@shared@*/ struct gpio *DIGITAL43;
    /*@shared@*/ struct gpio *DIGITAL44;
    /*@shared@*/ struct gpio *DIGITAL45;
    /*@shared@*/ struct gpio *DIGITAL46;
    /*@shared@*/ struct gpio *DIGITAL47;
    /*@shared@*/ struct gpio *DIGITAL48;
    /*@shared@*/ struct gpio *DIGITAL49;
    /*@shared@*/ struct gpio *DIGITAL50;
    /*@shared@*/ struct gpio *DIGITAL51;
    /*@shared@*/ struct gpio *DIGITAL52;
    /*@shared@*/ struct gpio *DIGITAL53;
    /* COMMUNICATION */
    /*@shared@*/ struct uart *SERIAL0;
    /*@shared@*/ /* struct usart *SERIAL3 */
    /*@shared@*/ /* struct usart *SERIAL2 */
    /*@shared@*/ /* struct usart *SERIAL1 */
    /*@shared@*/ struct twi *I2C;
    /* PWM */
    /*@shared@*/ struct pwm *PWM2;
    /*@shared@*/ struct ipwm *PWM3;
    /*@shared@*/ struct pwm *PWM4;
    /*@shared@*/ struct pwm *PWM5;
    /*@shared@*/ /* struct pwm *PWM6; */
    /*@shared@*/ /* struct pwm *PWM7; */
    /*@shared@*/ /* struct pwm *PWM8; */
    /*@shared@*/ /* struct pwm *PWM9; */
    /*@shared@*/ /* struct pwm *PWM10; */
    /*@shared@*/ struct pwm *PWM11;
    /*@shared@*/ struct pwm *PWM12;
    /*@shared@*/ struct pwm *PWM13;
    /* MISC */
    /*@shared@*/ struct spi *SPI;
    /*@shared@*/ struct twi *I2C1;
    /* WATCHDOG */
    /*@shared@*/ struct wd *WDT;
};

int arduino_due_init(/*@out@*/ struct arduino_due *ctx);

#endif

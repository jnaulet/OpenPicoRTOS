#ifndef RASPBERRY_PICO_H
#define RASPBERRY_PICO_H

/* Raspberry Pico hw layout
 * This layout is the one used on the pico-LCD kit
 * Notable facts:
 * - Don't use PWM0 in SMP, it is used to synchronize the cores
 * - PWM2B (GP5) is an input PWM
 * - Free gpios are outputs
 */

#include "picoRTOS_device.h"

/* Full abstraction here, for demonstration purposes */
#include "adc.h"
#include "gpio.h"
#include "spi.h"
#include "uart.h"
#include "pwm.h"
#include "twi.h"
#include "wd.h"

struct raspberry_pico {
    /* UART */
    /*@shared@*/ struct uart *UART;     /* GP0, GP1 */
    /* I2C */
    /*@shared@*/ struct twi *I2C;       /* GP6, GP7 */
    /* SPI */
    /*@shared@*/ struct spi *SPI;       /* GP9, GP10, GP11, GP12 */
    /* GPIOs */
    /*@shared@*/ struct gpio *GPIO2;    /* Joystick up */
    /*@shared@*/ struct gpio *GPIO3;    /* Joystick press center */
    /*@shared@*/ struct gpio *GPIO4;
    /*@shared@*/ struct gpio *GPIO8;    /* LCD_DC */
    /*@shared@*/ struct gpio *GPIO13;   /* LCD_BL */
    /*@shared@*/ struct gpio *GPIO14;
    /*@shared@*/ struct gpio *GPIO15;   /* User key A */
    /*@shared@*/ struct gpio *GPIO16;   /* Joystick left */
    /*@shared@*/ struct gpio *GPIO17;   /* User key B */
    /*@shared@*/ struct gpio *GPIO18;   /* Joystick down */
    /*@shared@*/ struct gpio *GPIO19;
    /*@shared@*/ struct gpio *GPIO20;   /* Joystick right */
    /*@shared@*/ struct gpio *GPIO21;
    /* PWM */
    /*@shared@*/ struct ipwm *PWM2B;    /* GP5 */
    /*@shared@*/ struct pwm *PWM3A;     /* GP22 */
    /*@shared@*/ struct pwm *PWM3B;     /* GP23 */
    /*@shared@*/ struct pwm *PWM4A;     /* GP24 */
    /*@shared@*/ struct pwm *PWM4B;     /* GP25 */
    /* ADCs */
    /*@shared@*/ struct adc *ADC0;      /* GP26 */
    /*@shared@*/ struct adc *ADC1;      /* GP27 */
    /*@shared@*/ struct adc *ADC2;      /* GP28 */
    /* TEMP */
    /*@shared@*/ struct adc *TEMP_SENSOR;
    /* WATCHDOG */
    /*@shared@*/ struct wd *WDT;
};

int raspberry_pico_init(/*@out@*/ struct raspberry_pico *ctx);

#endif

#ifndef RASPBERRY_PICO_H
#define RASPBERRY_PICO_H

/* safety might require a little less abstraction */
#include "adc-rp2040.h"
#include "clock-rp2040.h"
#include "gpio-rp2040_sio.h"
#include "misc-rp2040_reset.h"
#include "mux-rp2040.h"
#include "spi-arm_pl022.h"
#include "uart-arm_pl011.h"
#include "pwm-rp2040.h"
#include "twi-dw_apb_i2c.h"
#include "wd-rp2040.h"

struct raspberry_pico {
    /* UART */
    /*@shared@*/ struct uart UART;      /* GP0, GP1 */
    /* I2C */
    /*@shared@*/ struct twi I2C0;       /* GP13, GP14 */
    /*@shared@*/ struct twi I2C1;       /* GP6, GP7 */
    /* SPI */
    /*@shared@*/ struct spi SPI;        /* GP9, GP10, GP11, GP12 */
    /* GPIOs */
    /*@shared@*/ struct gpio GPIO2;
    /*@shared@*/ struct gpio GPIO3;
    /*@shared@*/ struct gpio GPIO4;
    /*@shared@*/ struct gpio GPIO8;
    /*@shared@*/ struct gpio GPIO15;
    /*@shared@*/ struct gpio GPIO16;
    /*@shared@*/ struct gpio GPIO17;
    /*@shared@*/ struct gpio GPIO18;
    /*@shared@*/ struct gpio GPIO19;
    /* PWM */
    /*@shared@*/ struct ipwm PWM2B;     /* GP5 */
    /*@shared@*/ struct pwm PWM4A;      /* GP24 */
    /*@shared@*/ struct pwm PWM4B;      /* GP25 */
    /*@shared@*/ struct pwm PWM5A;      /* GP26 */
    /*@shared@*/ struct pwm PWM5B;      /* GP27 */
    /* ADCs */
    /*@shared@*/ struct adc ADC0;       /* GP26 */
    /*@shared@*/ struct adc ADC1;       /* GP27 */
    /*@shared@*/ struct adc ADC2;       /* GP28 */
    /* TEMP */
    /*@shared@*/ struct adc TEMP_SENSOR;
    /* WATCHDOG */
    /*@shared@*/ struct wd WDT;
};

int raspberry_pico_init(/*@out@*/ struct raspberry_pico *ctx);

#endif

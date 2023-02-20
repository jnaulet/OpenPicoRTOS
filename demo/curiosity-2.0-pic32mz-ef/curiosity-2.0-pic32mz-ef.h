#ifndef CURIOSITY_20_PIC32MZ_EF_H
#define CURIOSITY_20_PIC32MZ_EF_H

#include "gpio-pic32mx.h"
#include "pwm-pic32mx.h"
#include "spi-pic32mx.h"
#include "uart-pic32mx.h"
#include "wd-pic32mx.h"

struct curiosity_20_pic32mz_ef {
    struct gpio LED1;
    struct gpio LED2;
    struct gpio LED3;
    struct rgb_led {
        struct pwm R;
        struct pwm G;
        struct pwm B;
    } LED4;
    struct uart UART1;
    struct uart UART6;  /* STREAM */
    struct spi SPI1;
    struct wd WDT;
    struct pwm PWM1;
};

int curiosity_20_pic32mz_ef_init(/*@out@*/ struct curiosity_20_pic32mz_ef *ctx);

#endif

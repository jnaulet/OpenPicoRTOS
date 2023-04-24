#ifndef CURIOSITY_20_PIC32MZ_EF_H
#define CURIOSITY_20_PIC32MZ_EF_H

#include "adc-pic32mx.h"
#include "can-pic32mx.h"
#include "flash-pic32mx.h"
#include "gpio-pic32mx.h"
#include "pwm-pic32mx.h"
#include "spi-pic32mx.h"
#include "twi-pic32mx.h"
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
    struct twi I2C1;
    struct twi I2C2;
    struct adc A0;
    struct adc IVtemp;
    struct can CAN2;
    struct flash FLASH;
};

int curiosity_20_pic32mz_ef_init(/*@out@*/ struct curiosity_20_pic32mz_ef *ctx);

#endif

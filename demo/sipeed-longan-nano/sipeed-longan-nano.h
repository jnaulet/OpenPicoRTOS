#ifndef SIPEED_LONGAN_NANO_H
#define SIPEED_LONGAN_NANO_H

#include "adc-gd32vf103.h"
#include "can-gd32vf103.h"
#include "flash-gd32vf103.h"
#include "gpio-gd32vf103.h"
#include "pwm-gd32vf103.h"
#include "spi-gd32vf103.h"
#include "twi-gd32vf103.h"
#include "uart-gd32vf103.h"
#include "wd-gd32vf103_fwdgt.h"

struct sipeed_longan_nano {
    struct gpio TICK;
    struct gpio LED_R;
    struct gpio LED_G;
    struct pwm LED_B;
    struct uart USART0;
    struct spi SPI0;
    struct wd WDT;
    struct can CAN0;
    struct flash FLASH;
    struct adc TEMP;
    struct twi I2C0;
    struct twi I2C1;
};

int sipeed_longan_nano_init(/*@out@*/ struct sipeed_longan_nano *ctx);

#endif

#ifndef ARDUINO_NANO_V3_CLONE_H
#define ARDUINO_NANO_V3_CLONE_H

#include "picoRTOS_device.h"

#include "clock-lgt8fx8p.h"
#include "gpio-avr.h"
#include "mux-avr.h"
#include "uart-avr.h"
#include "pwm-avr.h"
#include "spi-avr+irqdriven.h"
#include "wd-avr.h"

struct arduino_nano_v3_clone {
    struct gpio TICK;
    struct gpio L;
    struct uart SERIAL;
    struct spi SPI;
    struct pwm PWM3;
    struct pwm PWM9;
    struct wd WDT;
};

int arduino_nano_v3_clone_init(/*@out@*/ struct arduino_nano_v3_clone *ctx);

#endif

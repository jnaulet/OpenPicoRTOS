#ifndef SAME70_XPLAINED_ULTRA_H
#define SAME70_XPLAINED_ULTRA_H

#include "gpio-atmel_pio.h"
#include "can-bosch_mcan.h"
#include "adc-atmel_afec.h"
#include "pwm-atsamx7x_tc.h"
#include "uart-atsamx7x_usart_irqdriven.h"

struct same70_xplained_ultra {
    struct gpio USER_LED0;
    struct can CAN;
    struct adc POT;
    struct pwm PWM;
    struct uart UART;
};

int same70_xplained_ultra_init(/*@out@*/ struct same70_xplained_ultra *ctx);

#endif

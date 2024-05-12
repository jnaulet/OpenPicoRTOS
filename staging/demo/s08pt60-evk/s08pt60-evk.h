#ifndef S08PT60_EVK_H
#define S08PT60_EVK_H

#include "wd-mc9s08ptxx.h"
#include "gpio-mc9s08ptxx.h"
#include "uart-mc9s08ptxx_sci.h"
#include "spi-mc9s08ptxx.h"
#include "pwm-mc9s08ptxx_ftm.h"
#include "adc-mc9s08ptxx.h"

struct s08pt60_evk {
    struct gpio TICK;
    struct gpio RED;
    struct gpio GREEN;
    /* struct gpio BLUE; */
    /* UART */
    struct uart UART2;
    /* SPI */
    struct spi SPI1;
    /* PWM & ADC */
    struct s08pt60_evk_ana {
        struct pwm FTM2CH5;
        struct adc ADP1;
    } ANA;
    /* watchdog */
    struct wd WD;
};

int s08pt60_evk_init(/*@out@*/ struct s08pt60_evk *ctx);

#endif

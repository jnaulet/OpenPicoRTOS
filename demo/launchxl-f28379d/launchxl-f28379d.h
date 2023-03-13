#ifndef LAUNCHXL_F28379D_H
#define LAUNCHXL_F28379D_H

#include "gpio-ti_f28x.h"
#include "spi-ti_f28x.h"
#include "pwm-ti_epwm.h"
#include "adc-ti_type4.h"
#include "can-ti_dcan.h"
#include "wd-ti_f28x.h"
#include "twi-ti_f28x.h"

struct launchxl_f28379d {
    struct gpio TICK;
    struct f28379x_led {
        struct gpio R;
        struct gpio B;
    } LED;
    struct spi SPI;
    struct pwm PWM;
    struct adc ADCIN13; /* temp sensor */
    struct can CAN;
    struct wd WDT;
    struct twi I2CA;
    struct twi I2CB;
};

int launchxl_f28379d_init(/*@out@*/ struct launchxl_f28379d *ctx);

#endif

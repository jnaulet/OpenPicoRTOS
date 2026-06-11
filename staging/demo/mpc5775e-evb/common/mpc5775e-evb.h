#ifndef MPC5775E_EVB_H
#define MPC5775E_EVB_H

#include "adc-nxp_eqadc.h"
#include "gpio-nxp_siu.h"
#include "pwm-nxp_emios.h"
#include "spi-nxp_dspi.h"
#include "can-nxp_flexcan.h"

#define MPC5775E_LED_COUNT 4

struct mpc5775e_evb {
    struct adc ADC;
    struct gpio TICK;
    struct gpio LED0;
    struct gpio LED1;
    struct pwm PWM;
    struct ipwm IPWM;
    struct spi SPI;
    struct can CAN;
};

int mpc5775e_evb_init(/*@out@*/ struct mpc5775e_evb *ctx);

#endif

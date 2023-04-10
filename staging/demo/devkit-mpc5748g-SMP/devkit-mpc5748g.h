#ifndef DEVKIT_MPC5748G_H
#define DEVKIT_MPC5748G_H

#include "adc-nxp_sar.h"
#include "can-nxp_flexcan.h"
#include "flash-nxp_c55fmc.h"
#include "gpio-nxp_siul2.h"
#include "lin-nxp_linflexd.h"
#include "pwm-nxp_emios.h"
#include "spi-nxp_dspi.h"
#include "twi-nxp_i2c.h"
#include "uart-nxp_linflexd.h"

#define DEVKIT_MPC5748G_LED_COUNT 8

struct devkit_mpc5748g {
    struct adc ADC1_P0;
    struct can CAN0;
    struct flash FLASH;
    struct gpio LED[DEVKIT_MPC5748G_LED_COUNT];
    struct lin LIN0;
    struct pwm PWM;
    struct spi SPI;
    struct twi I2C0;
    struct twi I2C1;
    struct gpio TICK;
    struct uart UART;
};

int devkit_mpc5748g_init(/*@out@*/ struct devkit_mpc5748g *ctx);

#endif

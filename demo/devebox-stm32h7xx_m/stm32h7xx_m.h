#ifndef STM32H7XX_M_H
#define STM32H7XX_M_H

#include "adc-stm32h7xx_sar.h"
#include "gpio-stm32h7xx.h"
#include "pwm-stm32h7xx_tim.h"
#include "spi-stm32h7xx.h"
#include "uart-stm32h7xx.h"
#include "can-stm32h7xx_fdcan.h"
#include "rng-stm32h7xx_trng.h"

struct stm32h7xx_m {
    struct gpio TICK;
    struct pwm LED;
    struct spi SPI1;
    struct uart UART4;
    struct can_test {
        struct can CAN;
        struct rng TRNG;
    } can;
    struct adc TEMP;
};

int stm32h7xx_m_init(/*@out@*/ struct stm32h7xx_m *ctx);

#endif

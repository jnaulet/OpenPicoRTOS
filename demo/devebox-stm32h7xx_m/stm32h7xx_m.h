#ifndef STM32H7XX_M_H
#define STM32H7XX_M_H

#include "gpio-stm32h7xx.h"
#include "pwm-stm32h7xx_tim.h"
#include "spi-stm32h7xx.h"
#include "uart-stm32h7xx.h"

struct stm32h7xx_m {
    struct gpio TICK;
    struct pwm LED;
    struct spi SPI1;
    struct uart UART4;
};

int stm32h7xx_m_init(/*@out@*/ struct stm32h7xx_m *ctx);

#endif

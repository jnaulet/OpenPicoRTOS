#ifndef CLOCK_STM32F401X_H
#define CLOCK_STM32F401X_H

#include "clock.h"

#define CLOCK_STM32F401X_SYSCLK  CLOCK_SYSCLK
#define CLOCK_STM32F401X_USBCLK  (clock_id_t)1
#define CLOCK_STM32F401X_I2SCLK  (clock_id_t)2
#define CLOCK_STM32F401X_HCLK    (clock_id_t)3
#define CLOCK_STM32F401X_APB1    (clock_id_t)4
#define CLOCK_STM32F401X_APB2    (clock_id_t)5
#define CLOCK_STM32F401X_RTCCLK  (clock_id_t)6
#define CLOCK_STM32F401X_IWDGCLK (clock_id_t)7

typedef enum {
    CLOCK_STM32F401X_SYSCLK_HSI = 0,
    CLOCK_STM32F401X_SYSCLK_HSE = 1,
    CLOCK_STM32F401X_SYSCLK_PLL = 2,
    CLOCK_STM32F401X_SYSCLK_COUNT,
} clock_stm32f401x_sysclk_t;

typedef enum {
    CLOCK_STM32F401X_PLLSRC_HSI = 0,
    CLOCK_STM32F401X_PLLSRC_HSE = 1,
    CLOCK_STM32F401X_PLLSRC_COUNT,
} clock_stm32f401x_pllsrc_t;

struct clock_settings {
    /* clock sources */
    clock_stm32f401x_sysclk_t sysclk;
    clock_stm32f401x_pllsrc_t pllsrc;
    /* constants */
    unsigned long hse;      /* 4-26 HSE OSC. 0: disable */
    unsigned long pll_p;    /* request pll P freq. 0: disable */
    /* prescalers */
    unsigned ahb_prescaler;
    unsigned apb1_prescaler;
    unsigned apb2_prescaler;
};

int clock_stm32f401x_init(struct clock_settings *settings);

typedef enum {
    /* AHB1 */
    CLOCK_STM32F401X_CLK_GPIOA  = 0x0,
    CLOCK_STM32F401X_CLK_GPIOB  = 0x1,
    CLOCK_STM32F401X_CLK_GPIOC  = 0x2,
    CLOCK_STM32F401X_CLK_GPIOD  = 0x3,
    CLOCK_STM32F401X_CLK_GPIOE  = 0x4,
    CLOCK_STM32F401X_CLK_GPIOH  = 0x7,
    CLOCK_STM32F401X_CLK_CRC    = 0xc,
    CLOCK_STM32F401X_CLK_DMA1   = 0x15,
    CLOCK_STM32F401X_CLK_DMA2   = 0x16,
    /* AHB2 */
    CLOCK_STM32F401X_CLK_OTGFS  = 0x27,
    /* APB1 */
    CLOCK_STM32F401X_CLK_TIM2   = 0x40,
    CLOCK_STM32F401X_CLK_TIM3   = 0x41,
    CLOCK_STM32F401X_CLK_TIM4   = 0x42,
    CLOCK_STM32F401X_CLK_TIM5   = 0x43,
    CLOCK_STM32F401X_CLK_WWDG   = 0x4b,
    CLOCK_STM32F401X_CLK_SPI2   = 0x4e,
    CLOCK_STM32F401X_CLK_SPI3   = 0x4f,
    CLOCK_STM32F401X_CLK_USART2 = 0x51,
    CLOCK_STM32F401X_CLK_I2C1   = 0x55,
    CLOCK_STM32F401X_CLK_I2C2   = 0x56,
    CLOCK_STM32F401X_CLK_I2C3   = 0x57,
    CLOCK_STM32F401X_CLK_PWR    = 0x5c,
    /* APB2 */
    CLOCK_STM32F401X_CLK_TIM1   = 0x60,
    CLOCK_STM32F401X_CLK_USART1 = 0x64,
    CLOCK_STM32F401X_CLK_USART6 = 0x65,
    CLOCK_STM32F401X_CLK_ADC1   = 0x68,
    CLOCK_STM32F401X_CLK_SDIO   = 0x6b,
    CLOCK_STM32F401X_CLK_SPI1   = 0x6c,
    CLOCK_STM32F401X_CLK_SPI4   = 0x6d,
    CLOCK_STM32F401X_CLK_SYSCFG = 0x6e,
    CLOCK_STM32F401X_CLK_TIM9   = 0x70,
    CLOCK_STM32F401X_CLK_TIM10  = 0x71,
    CLOCK_STM32F401X_CLK_TIM11  = 0x72,
    /* The End */
    CLOCK_STM32F401X_CLK_COUNT,
} clock_stm32f401x_clk_t;

int clock_stm32f401x_enable(clock_stm32f401x_clk_t clk);
int clock_stm32f401x_disable(clock_stm32f401x_clk_t clk);

/* Implements:
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

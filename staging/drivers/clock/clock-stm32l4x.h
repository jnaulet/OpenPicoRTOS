#ifndef CLOCK_STM32L4X_H
#define CLOCK_STM32L4X_H

#include "clock.h"

#define CLOCK_STM32L4X_SYSCLK  CLOCK_SYSCLK
#define CLOCK_STM32L4X_HCLK    (clock_id_t)1
#define CLOCK_STM32L4X_PCLK1   (clock_id_t)2
#define CLOCK_STM32L4X_TIM2    (clock_id_t)3
#define CLOCK_STM32L4X_TIM3    CLOCK_STM32L4X_TIM2
#define CLOCK_STM32L4X_TIM6    CLOCK_STM32L4X_TIM2
#define CLOCK_STM32L4X_TIM7    CLOCK_STM32L4X_TIM2
#define CLOCK_STM32L4X_USARTx  (clock_id_t)4
#define CLOCK_STM32L4X_USART2  CLOCK_STM32L4X_USARTx
#define CLOCK_STM32L4X_USART3  CLOCK_STM32L4X_USARTx
#define CLOCK_STM32L4X_USART4  CLOCK_STM32L4X_USARTx
#define CLOCK_STM32L4X_I2Cx    (clock_id_t)5
#define CLOCK_STM32L4X_I2C1    CLOCK_STM32L4X_I2Cx
#define CLOCK_STM32L4X_I2C2    CLOCK_STM32L4X_I2Cx
#define CLOCK_STM32L4X_I2C3    CLOCK_STM32L4X_I2Cx
#define CLOCK_STM32L4X_I2C4    CLOCK_STM32L4X_I2Cx
#define CLOCK_STM32L4X_TPTIMx  (clock_id_t)6
#define CLOCK_STM32L4X_TPTIM1  CLOCK_STM32L4X_TPTIMx
#define CLOCK_STM32L4X_TPTIM2  CLOCK_STM32L4X_TPTIMx
#define CLOCK_STM32L4X_SWPMI   (clock_id_t)7
#define CLOCK_STM32L4X_PCLK2   (clock_id_t)8
#define CLOCK_STM32L4X_TIM1    (clock_id_t)9
#define CLOCK_STM32L4X_TIM15   CLOCK_STM32L4X_TIM1
#define CLOCK_STM32L4X_TIM16   CLOCK_STM32L4X_TIM1
#define CLOCK_STM32L4X_USART1  (clock_id_t)10
#define CLOCK_STM32L4X_DFSDM1  (clock_id_t)11
#define CLOCK_STM32L4X_USB     (clock_id_t)12
#define CLOCK_STM32L4X_RNG     CLOCK_STM32L4X_USB
#define CLOCK_STM32L4X_SDMMC   CLOCK_STM32L4X_USB
#define CLOCK_STM32L4X_ADCx    (clock_id_t)13
#define CLOCK_STM32L4X_ADC1    CLOCK_STM32L4X_ADCx
#define CLOCK_STM32L4X_ADC2    CLOCK_STM32L4X_ADCx
#define CLOCK_STM32L4X_SAI1    (clock_id_t)14
#define CLOCK_STM32L4X_IWDG    (clock_id_t)15
#define CLOCK_STM32L4X_RTC     (clock_id_t)16
#define CLOCK_STM32L4X_LCD     CLOCK_STM32L4X_RTC
#define CLOCK_STM32L4X_PWR     (clock_id_t)17

typedef enum {
    CLOCK_STM32L4X_SYSCLK_MSI   = 0,
    CLOCK_STM32L4X_SYSCLK_HSI16 = 1,
    CLOCK_STM32L4X_SYSCLK_HSE   = 2,
    CLOCK_STM32L4X_SYSCLK_PLL   = 3,
    CLOCK_STM32L4X_SYSCLK_COUNT,
} clock_stm32l4x_sysclk_t;

typedef enum {
    CLOCK_STM32L4X_PLLSRC_NONE  = 0,
    CLOCK_STM32L4X_PLLSRC_MSI   = 1,
    CLOCK_STM32L4X_PLLSRC_HSI16 = 2,
    CLOCK_STM32L4X_PLLSRC_HSE   = 3,
    CLOCK_STM32L4X_PLLSRC_COUNT,
} clock_stm32l4x_pllsrc_t;

struct clock_settings {
    /* clock sources */
    clock_stm32l4x_sysclk_t sysclk;
    clock_stm32l4x_pllsrc_t pllsrc;
    /* constants */
    unsigned long hse;      /* 4-48 HSE OSC. 0: disable */
    unsigned long pll_r;    /* request pll R freq. 0: disable */
    /* prescalers */
    unsigned ahb_prescaler;
    unsigned apb1_prescaler;
    unsigned apb2_prescaler;
};

int clock_stm32l4x_init(struct clock_settings *settings);

typedef enum {
    /* AHB1 */
    CLOCK_STM32L4X_CLK_DMA1     = 0x0,
    CLOCK_STM32L4X_CLK_DMA2     = 0x1,
    CLOCK_STM32L4X_CLK_FLASH    = 0x8,
    CLOCK_STM32L4X_CLK_CRC      = 0xc,
    CLOCK_STM32L4X_CLK_RSC      = 0x10,
    /* AHB2 */
    CLOCK_STM32L4X_CLK_GPIOA    = 0x20,
    CLOCK_STM32L4X_CLK_GPIOB    = 0x21,
    CLOCK_STM32L4X_CLK_GPIOC    = 0x22,
    CLOCK_STM32L4X_CLK_GPIOD    = 0x23,
    CLOCK_STM32L4X_CLK_GPIOE    = 0x24,
    CLOCK_STM32L4X_CLK_GPIOH    = 0x27,
    CLOCK_STM32L4X_CLK_ADC      = 0x2d,
    CLOCK_STM32L4X_CLK_AES      = 0x30,
    CLOCK_STM32L4X_CLK_RNG      = 0x32,
    /* AHB3 */
    CLOCK_STM32L4X_CLK_         = 0x48,
    /* APB1.1 */
    CLOCK_STM32L4X_CLK_TIM2     = 0x60,
    CLOCK_STM32L4X_CLK_TIM3     = 0x61,
    CLOCK_STM32L4X_CLK_TIM6     = 0x64,
    CLOCK_STM32L4X_CLK_TIM7     = 0x65,
    CLOCK_STM32L4X_CLK_LCD      = 0x69,
    CLOCK_STM32L4X_CLK_RTCAPB   = 0x6a,
    CLOCK_STM32L4X_CLK_WWDG     = 0x6b,
    CLOCK_STM32L4X_CLK_SPI2     = 0x6e,
    CLOCK_STM32L4X_CLK_SPI3     = 0x6f,
    CLOCK_STM32L4X_CLK_USART2   = 0x71,
    CLOCK_STM32L4X_CLK_USART3   = 0x72,
    CLOCK_STM32L4X_CLK_UART4    = 0x73,
    CLOCK_STM32L4X_CLK_I2C1     = 0x75,
    CLOCK_STM32L4X_CLK_I2C2     = 0x76,
    CLOCK_STM32L4X_CLK_I2C3     = 0x77,
    CLOCK_STM32L4X_CLK_CRS      = 0x78,
    CLOCK_STM32L4X_CLK_CAN1     = 0x79,
    CLOCK_STM32L4X_CLK_USBFS    = 0x7a,
    CLOCK_STM32L4X_CLK_PWR      = 0x7c,
    CLOCK_STM32L4X_CLK_DAC1     = 0x7d,
    CLOCK_STM32L4X_CLK_OPAMP    = 0x7e,
    CLOCK_STM32L4X_CLK_LPTIM1   = 0x7f,
    /* APB1.2 */
    CLOCK_STM32L4X_CLK_LPUART1  = 0x80,
    CLOCK_STM32L4X_CLK_I2C4     = 0x81,
    CLOCK_STM32L4X_CLK_SWPMI1   = 0x82,
    CLOCK_STM32L4X_CLK_LPTIM2   = 0x85,
    /* APB2 */
    CLOCK_STM32L4X_CLK_SYSCFG   = 0xa0,
    CLOCK_STM32L4X_CLK_FW       = 0xa7,
    CLOCK_STM32L4X_CLK_SDMMC1   = 0xaa,
    CLOCK_STM32L4X_CLK_TIM1     = 0xab,
    CLOCK_STM32L4X_CLK_SPI1     = 0xac,
    CLOCK_STM32L4X_CLK_USART1   = 0xae,
    CLOCK_STM32L4X_CLK_TIM15    = 0xb0,
    CLOCK_STM32L4X_CLK_TIM16    = 0xb1,
    CLOCK_STM32L4X_CLK_SAI1     = 0xb5,
    CLOCK_STM32L4X_CLK_DFSDM1   = 0xb8,
    /* The End */
    CLOCK_STM32L4X_CLK_COUNT,
} clock_stm32l4x_clk_t;

int clock_stm32l4x_enable(clock_stm32l4x_clk_t clk);
int clock_stm32l4x_disable(clock_stm32l4x_clk_t clk);

/* Implements:
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

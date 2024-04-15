#ifndef CLOCK_CH32V003_H
#define CLOCK_CH32V003_H

#include "clock.h"

#define CLOCK_CH32V003_HCLK    CLOCK_SYSCLK
#define CLOCK_CH32V003_IWDGCLK (clock_id_t)1
#define CLOCK_CH32V003_ADCCLK  (clock_id_t)2
#define CLOCK_CH32V003_WWDGCLK (clock_id_t)3
#define CLOCK_CH32V003_COUNT   4

typedef enum {
    CLOCK_CH32V003_SYSCLK_HSI       = 0,
    CLOCK_CH32V003_SYSCLK_HSE       = 1,
    CLOCK_CH32V003_SYSCLK_PLLCLK    = 2,
    CLOCK_CH32V003_SYSCLK_COUNT
} clock_ch32v003_sysclk_t;

typedef enum {
    CLOCK_CH32V003_PLLSRC_HSI   = 0,
    CLOCK_CH32V003_PLLSRC_HSE   = 1,
    CLOCK_CH32V003_PLLSRC_COUNT
} clock_ch32v003_pllsrc_t;

struct clock_settings {
    clock_ch32v003_sysclk_t sysclk;
    clock_ch32v003_pllsrc_t pllsrc; /* only used if sysclk is PLLSRC */
    unsigned long hse;              /* external HSE in hz */
    unsigned long hclk_div;         /* HCLK divider (1-256) */
};

int clock_ch32v003_init(struct clock_settings *settings);

typedef enum {
    /* AHB */
    CLOCK_CH32V003_CLK_DMA1     = 0x0,
    CLOCK_CH32V003_CLK_SRAM     = 0x2,
    /* APB2 */
    CLOCK_CH32V003_CLK_AFIO     = 0x20,
    CLOCK_CH32V003_CLK_IOPA     = 0x22,
    CLOCK_CH32V003_CLK_IOPC     = 0x24,
    CLOCK_CH32V003_CLK_IOPD     = 0x25,
    CLOCK_CH32V003_CLK_ADC1     = 0x29,
    CLOCK_CH32V003_CLK_TIM1     = 0x2b,
    CLOCK_CH32V003_CLK_SPI1     = 0x2c,
    CLOCK_CH32V003_CLK_USART1   = 0x2e,
    /* APB1 */
    CLOCK_CH32V003_CLK_TIM2     = 0x40,
    CLOCK_CH32V003_CLK_WWDG     = 0x4b,
    CLOCK_CH32V003_CLK_I2C1     = 0x55,
    CLOCK_CH32V003_CLK_PWR      = 0x5c,
    /* The End */
    CLOCK_CH32V003_CLK_COUNT
} clock_ch32v003_clk_t;

int clock_ch32v003_enable(clock_ch32v003_clk_t clk);
int clock_ch32v003_disable(clock_ch32v003_clk_t clk);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

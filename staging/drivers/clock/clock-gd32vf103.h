#ifndef CLOCK_GD31VF103_H
#define CLOCK_GD31VF103_H

#include "clock.h"

#define CLOCK_GD32VF103_CK_SYS CLOCK_SYSCLK
#define CLOCK_GD32VF103_AHB    (clock_id_t)1
#define CLOCK_GD32VF103_APB1   (clock_id_t)2
#define CLOCK_GD32VF103_APB2   (clock_id_t)3
#define CLOCK_GD32VF103_ADC    (clock_id_t)4
#define CLOCK_GD32VF103_COUNT  5

typedef enum {
    CLOCK_GD32VF103_PLLSEL_IRC8M    = 0,
    CLOCK_GD32VF103_PLLSEL_HXTAL    = 1,
    CLOCK_GD32VF103_PLLSEL_COUNT
} clock_gd32vf103_pllsel_t;

typedef enum {
    CLOCK_GD32VF103_CK_SYS_IRC8M    = 0,
    CLOCK_GD32VF103_CK_SYS_HXTAL    = 1,
    CLOCK_GD32VF103_CK_SYS_PLL      = 2,
    CLOCK_GD32VF103_CK_SYS_COUNT
} clock_gd32vf103_ck_sys_t;

struct clock_settings {
    unsigned long hxtal;    /* hxtal provided frequency (0: disable) */
    clock_gd32vf103_ck_sys_t ck_sys;
    clock_gd32vf103_pllsel_t pllsel;
    unsigned long pll;      /* pll requested frequency */
    unsigned long ahb_div;
    unsigned long apb1_div;
    unsigned long apb2_div;
    unsigned long adc_div;
};

int clock_gd32vf103_init(struct clock_settings *settings);

typedef enum {
    /* AHB */
    CLOCK_GD32VF103_CLK_DMA0    = 0x0,
    CLOCK_GD32VF103_CLK_DMA1    = 0x1,
    CLOCK_GD32VF103_CLK_SRAMSP  = 0x2,
    CLOCK_GD32VF103_CLK_FMCSP   = 0x4,
    CLOCK_GD32VF103_CLK_CRC     = 0x6,
    CLOCK_GD32VF103_CLK_EXMC    = 0x8,
    CLOCK_GD32VF103_CLK_USBFS   = 0xc,
    /* APB1 */
    CLOCK_GD32VF103_CLK_USART1  = 0x11,
    CLOCK_GD32VF103_CLK_USART2  = 0x12,
    CLOCK_GD32VF103_CLK_USART3  = 0x13,
    CLOCK_GD32VF103_CLK_USART4  = 0x14,
    CLOCK_GD32VF103_CLK_I2C0    = 0x15,
    CLOCK_GD32VF103_CLK_I2C1    = 0x16,
    CLOCK_GD32VF103_CLK_CAN0    = 0x19,
    CLOCK_GD32VF103_CLK_CAN1    = 0x1a,
    CLOCK_GD32VF103_CLK_BKPI    = 0x1b,
    CLOCK_GD32VF103_CLK_PMU     = 0x1c,
    CLOCK_GD32VF103_CLK_DAC     = 0x1d,
    /* APB2 */
    CLOCK_GD32VF103_CLK_AF      = 0x20,
    CLOCK_GD32VF103_CLK_PA      = 0x22,
    CLOCK_GD32VF103_CLK_PB      = 0x23,
    CLOCK_GD32VF103_CLK_PC      = 0x24,
    CLOCK_GD32VF103_CLK_PD      = 0x25,
    CLOCK_GD32VF103_CLK_PE      = 0x26,
    CLOCK_GD32VF103_CLK_ADC0    = 0x29,
    CLOCK_GD32VF103_CLK_ADC1    = 0x2a,
    CLOCK_GD32VF103_CLK_TIMER0  = 0x2b,
    CLOCK_GD32VF103_CLK_SPI0    = 0x2b,
    CLOCK_GD32VF103_CLK_USART0  = 0x2e,
    CLOCK_GD32VF103_CLK_COUNT
} clock_gd32vf103_clk_t;

int clock_gd32vf103_enable(clock_gd32vf103_clk_t clk);
int clock_gd32vf103_disable(clock_gd32vf103_clk_t clk);

/* Implements:
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

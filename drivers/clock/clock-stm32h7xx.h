#ifndef CLOCK_STM32H7XX_H
#define CLOCK_STM32H7XX_H

#include "clock.h"

/* To CPU, busses and peripherals */
#define CLOCK_STM32H7XX_PLL1_P_CK    (clock_id_t)1
#define CLOCK_STM32H7XX_PLL1_Q_CK    (clock_id_t)2
#define CLOCK_STM32H7XX_PLL1_R_CK    (clock_id_t)3
#define CLOCK_STM32H7XX_PLL2_P_CK    (clock_id_t)4
#define CLOCK_STM32H7XX_PLL2_Q_CK    (clock_id_t)5
#define CLOCK_STM32H7XX_PLL2_R_CK    (clock_id_t)6
#define CLOCK_STM32H7XX_PLL3_P_CK    (clock_id_t)7
#define CLOCK_STM32H7XX_PLL3_Q_CK    (clock_id_t)8
#define CLOCK_STM32H7XX_PLL3_R_CK    (clock_id_t)9
#define CLOCK_STM32H7XX_SYS_CK       CLOCK_SYSCLK
#define CLOCK_STM32H7XX_HSE_CK       (clock_id_t)10
#define CLOCK_STM32H7XX_HSI_KER_CK   (clock_id_t)11
#define CLOCK_STM32H7XX_CSI_KER_CK   (clock_id_t)12
#define CLOCK_STM32H7XX_LSI_CK       (clock_id_t)13
#define CLOCK_STM32H7XX_LSE_CK       (clock_id_t)14
#define CLOCK_STM32H7XX_HSI48_CK     (clock_id_t)15

#define CLOCK_STM32H7XX_PLL_COUNT 3

typedef enum {
    CLOCK_STM32H7XX_HSI_64MHZ   = 0,
    CLOCK_STM32H7XX_HSI_32MHZ   = 1,
    CLOCK_STM32H7XX_HSI_16MHZ   = 2,
    CLOCK_STM32H7XX_HSI_8MHZ    = 3,
    CLOCK_STM32H7XX_HSI_OFF     = 4,
    CLOCK_STM32H7XX_HSI_COUNT
} clock_stm32h7xx_hsi_t;

typedef enum {
    CLOCK_STM32H7XX_CSI_OFF,
    CLOCK_STM32H7XX_CSI_ON,
    CLOCK_STM32H7XX_CSI_COUNT
} clock_stm32h7xx_csi_t;

typedef enum {
    CLOCK_STM32H7XX_HSI48_OFF,
    CLOCK_STM32H7XX_HSI48_ON,
    CLOCK_STM32H7XX_HSI48_COUNT
} clock_stm32h7xx_hsi48_t;

typedef enum {
    CLOCK_STM32H7XX_PLLSRC_HSI_CK   = 0,
    CLOCK_STM32H7XX_PLLSRC_CSI_CK   = 1,
    CLOCK_STM32H7XX_PLLSRC_HSE_CK   = 2,
    CLOCK_STM32H7XX_PLLSRC_NO_CK    = 3,
    CLOCK_STM32H7XX_PLLSRC_COUNT
} clock_stm32h7xx_pllsrc_t;

typedef enum {
    CLOCK_STM32H7XX_SW_HSI_CK       = 0,
    CLOCK_STM32H7XX_SW_CSI_CK       = 1,
    CLOCK_STM32H7XX_SW_HSE_CK       = 2,
    CLOCK_STM32H7XX_SW_PLL1_P_CK    = 3,
    CLOCK_STM32H7XX_SW_COUNT
} clock_stm32h7xx_sw_t;

#define CLOCK_STM32H7XX_PLL_DIVx_MAX 128

struct clock_settings {
    clock_stm32h7xx_hsi_t hsi;
    unsigned long hse_hz; /* 0 = disable */
    clock_stm32h7xx_csi_t csi;
    clock_stm32h7xx_hsi48_t hsi48;
    /* plls */
    clock_stm32h7xx_pllsrc_t pllsrc;
    struct pll_settings {
        unsigned long vco_hz;
        unsigned long divp; /* 0 = disable */
        unsigned long divq; /* 0 = disable */
        unsigned long divr; /* 0 = disable */
    } pll[CLOCK_STM32H7XX_PLL_COUNT];
    /* SW mux & per_ck */
    clock_stm32h7xx_sw_t sw;
};

int clock_stm32h7xx_init(struct clock_settings *settings);

/* TODO:
 * Beware, this list might be different depending on the chip
 * and might have to find its way to picoRTOS_device.h
 */
typedef enum {
    /* AHB3 */
    CLOCK_STM32H7XX_AHB3_MDMA           = 0x0,
    CLOCK_STM32H7XX_AHB3_DMA2D          = 0x4,
    CLOCK_STM32H7XX_AHB3_JPGDEC         = 0x5,
    CLOCK_STM32H7XX_AHB3_FMC            = 0xc,
    CLOCK_STM32H7XX_AHB3_QSPI           = 0xe,
    CLOCK_STM32H7XX_AHB3_SDMMC1         = 0x10,
    /* AHB1 */
    CLOCK_STM32H7XX_AHB1_DMA1           = 0x20,
    CLOCK_STM32H7XX_AHB1_DMA2           = 0x21,
    CLOCK_STM32H7XX_AHB1_ADC12          = 0x25,
    CLOCK_STM32H7XX_AHB1_ETH1MAC        = 0x2f,
    CLOCK_STM32H7XX_AHB1_ETH1TX         = 0x30,
    CLOCK_STM32H7XX_AHB1_ETH1RX         = 0x31,
    CLOCK_STM32H7XX_AHB1_USB1OTGHS      = 0x39,
    CLOCK_STM32H7XX_AHB1_USB1OTGHSULPI  = 0x3a,
    CLOCK_STM32H7XX_AHB1_USB2OTGHS      = 0x3b,
    CLOCK_STM32H7XX_AHB1_USB2OTGHSULPI  = 0x3c,
    /* AHB2 */
    CLOCK_STM32H7XX_AHB2_DCMI           = 0x40,
    CLOCK_STM32H7XX_AHB2_CRYPT          = 0x44,
    CLOCK_STM32H7XX_AHB2_HASH           = 0x45,
    CLOCK_STM32H7XX_AHB2_RNG            = 0x46,
    CLOCK_STM32H7XX_AHB2_SDMMC2         = 0x49,
    CLOCK_STM32H7XX_AHB2_SRAM1          = 0x5d,
    CLOCK_STM32H7XX_AHB2_SRAM2          = 0x5e,
    CLOCK_STM32H7XX_AHB2_SRAM3          = 0x5f,
    /* AHB4 */
    CLOCK_STM32H7XX_AHB4_GPIOA          = 0x60,
    CLOCK_STM32H7XX_AHB4_GPIOB          = 0x61,
    CLOCK_STM32H7XX_AHB4_GPIOC          = 0x62,
    CLOCK_STM32H7XX_AHB4_GPIOD          = 0x63,
    CLOCK_STM32H7XX_AHB4_GPIOE          = 0x64,
    CLOCK_STM32H7XX_AHB4_GPIOF          = 0x65,
    CLOCK_STM32H7XX_AHB4_GPIOG          = 0x66,
    CLOCK_STM32H7XX_AHB4_GPIOH          = 0x67,
    CLOCK_STM32H7XX_AHB4_GPIOI          = 0x68,
    CLOCK_STM32H7XX_AHB4_GPIOJ          = 0x69,
    CLOCK_STM32H7XX_AHB4_GPIOK          = 0x6a,
    CLOCK_STM32H7XX_AHB4_CRC            = 0x73,
    CLOCK_STM32H7XX_AHB4_BDMA           = 0x75,
    CLOCK_STM32H7XX_AHB4_ADC3           = 0x78,
    CLOCK_STM32H7XX_AHB4_HSEM           = 0x79,
    CLOCK_STM32H7XX_AHB4_BKPRAM         = 0x7c,
    /* APB3 */
    CLOCK_STM32H7XX_APB3_LTDC           = 0x83,
    CLOCK_STM32H7XX_APB3_WWDG1          = 0x86,
    /* APB1L */
    CLOCK_STM32H7XX_APB1L_TIM2          = 0xa0,
    CLOCK_STM32H7XX_APB1L_TIM3          = 0xa1,
    CLOCK_STM32H7XX_APB1L_TIM4          = 0xa2,
    CLOCK_STM32H7XX_APB1L_TIM5          = 0xa3,
    CLOCK_STM32H7XX_APB1L_TIM6          = 0xa4,
    CLOCK_STM32H7XX_APB1L_TIM7          = 0xa5,
    CLOCK_STM32H7XX_APB1L_TIM12         = 0xa6,
    CLOCK_STM32H7XX_APB1L_TIM13         = 0xa7,
    CLOCK_STM32H7XX_APB1L_TIM14         = 0xa8,
    CLOCK_STM32H7XX_APB1L_LPTIM1        = 0xa9,
    CLOCK_STM32H7XX_APB1L_SPI2          = 0xae,
    CLOCK_STM32H7XX_APB1L_SPI3          = 0xaf,
    CLOCK_STM32H7XX_APB1L_SPDIFRX       = 0xb0,
    CLOCK_STM32H7XX_APB1L_USART2        = 0xb1,
    CLOCK_STM32H7XX_APB1L_USART3        = 0xb2,
    CLOCK_STM32H7XX_APB1L_UART4         = 0xb3,
    CLOCK_STM32H7XX_APB1L_UART5         = 0xb4,
    CLOCK_STM32H7XX_APB1L_I2C1          = 0xb5,
    CLOCK_STM32H7XX_APB1L_I2C2          = 0xb6,
    CLOCK_STM32H7XX_APB1L_I2C3          = 0xb7,
    CLOCK_STM32H7XX_APB1L_CEC           = 0xbb,
    CLOCK_STM32H7XX_APB1L_DAC12         = 0xbd,
    CLOCK_STM32H7XX_APB1L_UART7         = 0xbe,
    CLOCK_STM32H7XX_APB1L_UART8         = 0xbf,
    /* APB1H */
    CLOCK_STM32H7XX_APB1H_CRS           = 0xc1,
    CLOCK_STM32H7XX_APB1H_SWP           = 0xc2,
    CLOCK_STM32H7XX_APB1H_OPAMP         = 0xc4,
    CLOCK_STM32H7XX_APB1H_MDIOS         = 0xc5,
    CLOCK_STM32H7XX_APB1H_FDCAN         = 0xc8,
    /* APB2 */
    CLOCK_STM32H7XX_APB2_TIM1           = 0xe0,
    CLOCK_STM32H7XX_APB2_TIM8           = 0xe1,
    CLOCK_STM32H7XX_APB2_USART1         = 0xe4,
    CLOCK_STM32H7XX_APB2_USART6         = 0xe5,
    CLOCK_STM32H7XX_APB2_SPI1           = 0xec,
    CLOCK_STM32H7XX_APB2_SPI4           = 0xed,
    CLOCK_STM32H7XX_APB2_TIM15          = 0xf0,
    CLOCK_STM32H7XX_APB2_TIM16          = 0xf1,
    CLOCK_STM32H7XX_APB2_TIM17          = 0xf2,
    CLOCK_STM32H7XX_APB2_SPI5           = 0xf4,
    CLOCK_STM32H7XX_APB2_SAI1           = 0xf6,
    CLOCK_STM32H7XX_APB2_SAI2           = 0xf7,
    CLOCK_STM32H7XX_APB2_SAI3           = 0xf8,
    CLOCK_STM32H7XX_APB2_DFSDM1         = 0xfc,
    CLOCK_STM32H7XX_APB2_HRTIM          = 0xfd,
    /* APB4 */
    CLOCK_STM32H7XX_APB4_SYSCFG         = 0x101,
    CLOCK_STM32H7XX_APB4_LPUART1        = 0x103,
    CLOCK_STM32H7XX_APB4_SPI6           = 0x105,
    CLOCK_STM32H7XX_APB4_I2C4           = 0x107,
    CLOCK_STM32H7XX_APB4_LPTIM2         = 0x109,
    CLOCK_STM32H7XX_APB4_LPTIM3         = 0x10a,
    CLOCK_STM32H7XX_APB4_LPTIM4         = 0x10b,
    CLOCK_STM32H7XX_APB4_LPTIM5         = 0x10c,
    CLOCK_STM32H7XX_APB4_COMP12         = 0x10e,
    CLOCK_STM32H7XX_APB4_VREF           = 0x10f,
    CLOCK_STM32H7XX_APB4_RTCAPB         = 0x110,
    CLOCK_STM32H7XX_APB4_SAI4           = 0x111,
    /* the end */
    CLOCK_STM32H7XX_AXB_COUNT
} clock_stm32h7xx_axb_t;

int clock_stm32h7xx_enable(clock_stm32h7xx_axb_t clk);
int clock_stm32h7xx_disable(clock_stm32h7xx_axb_t clk);

#define DxCCIPxR(index, mask, shift)           \
    ((index) << 8 | (mask) << 5 | shift)

typedef enum {
    /* D1 */
    CLOCK_STM32H7XX_KER_FMCSEL          = DxCCIPxR(0, 0x3, 0),
    CLOCK_STM32H7XX_KER_QSPISEL         = DxCCIPxR(0, 0x3, 4),
    CLOCK_STM32H7XX_KER_SDMMCSEL        = DxCCIPxR(0, 0x1, 16),
    CLOCK_STM32H7XX_KER_CKPERSEL        = DxCCIPxR(0, 0x3, 28),
    /* D2 */
    CLOCK_STM32H7XX_KER_SAI1SEL         = DxCCIPxR(1, 0x7, 0),
    CLOCK_STM32H7XX_KER_SAI23SEL        = DxCCIPxR(1, 0x7, 6),
    CLOCK_STM32H7XX_KER_SPI123SEL       = DxCCIPxR(1, 0x7, 12),
    CLOCK_STM32H7XX_KER_SPI45SEL        = DxCCIPxR(1, 0x7, 16),
    CLOCK_STM32H7XX_KER_SPDIFSEL        = DxCCIPxR(1, 0x3, 20),
    CLOCK_STM32H7XX_KER_DFSDM1SEL       = DxCCIPxR(1, 0x1, 24),
    CLOCK_STM32H7XX_KER_FDCANSEL        = DxCCIPxR(1, 0x3, 28),
    CLOCK_STM32H7XX_KER_SWPSEL          = DxCCIPxR(1, 0x1, 31),
    CLOCK_STM32H7XX_KER_USART234578SEL  = DxCCIPxR(2, 0x7, 0),
    CLOCK_STM32H7XX_KER_USART16SEL      = DxCCIPxR(2, 0x7, 3),
    CLOCK_STM32H7XX_KER_RNGSEL          = DxCCIPxR(2, 0x3, 8),
    CLOCK_STM32H7XX_KER_I2C123SEL       = DxCCIPxR(2, 0x3, 12),
    CLOCK_STM32H7XX_KER_USBSEL          = DxCCIPxR(2, 0x3, 20),
    CLOCK_STM32H7XX_KER_CECSEL          = DxCCIPxR(2, 0x3, 22),
    CLOCK_STM32H7XX_KER_LPTIM1SEL       = DxCCIPxR(2, 0x3, 28),
    /* D3 */
    CLOCK_STM32H7XX_KER_LPUART1SEL      = DxCCIPxR(3, 0x7, 0),
    CLOCK_STM32H7XX_KER_I2C4SEL         = DxCCIPxR(3, 0x3, 8),
    CLOCK_STM32H7XX_KER_LPTIM2SEL       = DxCCIPxR(3, 0x7, 10),
    CLOCK_STM32H7XX_KER_LPTIM345SEL     = DxCCIPxR(3, 0x7, 13),
    CLOCK_STM32H7XX_KER_ADCSEL          = DxCCIPxR(3, 0x3, 16),
    CLOCK_STM32H7XX_KER_SAI4ASEL        = DxCCIPxR(3, 0x7, 21),
    CLOCK_STM32H7XX_KER_SAI4BSEL        = DxCCIPxR(3, 0x7, 24),
    CLOCK_STM32H7XX_KER_SPI6SEL         = DxCCIPxR(3, 0x7, 28),
    /* The End */
    CLOCK_STM32H7XX_KER_COUNT
} clock_stm32h7xx_ker_t;

int clock_stm32h7xx_ker_sel(clock_stm32h7xx_ker_t ker, unsigned int value);

/* Implements:
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

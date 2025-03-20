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
#define AxBxENR(index, bit) (((index) << 5) | bit)

typedef enum {
    /* AHB3 */
    CLOCK_STM32H7XX_AHB3_MDMA           = AxBxENR(0, 0),
    CLOCK_STM32H7XX_AHB3_DMA2D          = AxBxENR(0, 4),
    CLOCK_STM32H7XX_AHB3_JPGDEC         = AxBxENR(0, 5),
    CLOCK_STM32H7XX_AHB3_FMC            = AxBxENR(0, 12),
    CLOCK_STM32H7XX_AHB3_QSPI           = AxBxENR(0, 14),
    CLOCK_STM32H7XX_AHB3_SDMMC1         = AxBxENR(0, 16),
    /* AHB1 */
    CLOCK_STM32H7XX_AHB1_DMA1           = AxBxENR(1, 0),
    CLOCK_STM32H7XX_AHB1_DMA2           = AxBxENR(1, 1),
    CLOCK_STM32H7XX_AHB1_ADC12          = AxBxENR(1, 5),
    CLOCK_STM32H7XX_AHB1_ETH1MAC        = AxBxENR(1, 15),
    CLOCK_STM32H7XX_AHB1_ETH1TX         = AxBxENR(1, 16),
    CLOCK_STM32H7XX_AHB1_ETH1RX         = AxBxENR(1, 17),
    CLOCK_STM32H7XX_AHB1_USB1OTGHS      = AxBxENR(1, 25),
    CLOCK_STM32H7XX_AHB1_USB1OTGHSULPI  = AxBxENR(1, 26),
    CLOCK_STM32H7XX_AHB1_USB2OTGHS      = AxBxENR(1, 27),
    CLOCK_STM32H7XX_AHB1_USB2OTGHSULPI  = AxBxENR(1, 28),
    /* AHB2 */
    CLOCK_STM32H7XX_AHB2_DCMI           = AxBxENR(2, 0),
    CLOCK_STM32H7XX_AHB2_CRYPT          = AxBxENR(2, 4),
    CLOCK_STM32H7XX_AHB2_HASH           = AxBxENR(2, 5),
    CLOCK_STM32H7XX_AHB2_RNG            = AxBxENR(2, 6),
    CLOCK_STM32H7XX_AHB2_SDMMC2         = AxBxENR(2, 9),
    CLOCK_STM32H7XX_AHB2_SRAM1          = AxBxENR(2, 29),
    CLOCK_STM32H7XX_AHB2_SRAM2          = AxBxENR(2, 30),
    CLOCK_STM32H7XX_AHB2_SRAM3          = AxBxENR(2, 31),
    /* AHB4 */
    CLOCK_STM32H7XX_AHB4_GPIOA          = AxBxENR(3, 0),
    CLOCK_STM32H7XX_AHB4_GPIOB          = AxBxENR(3, 1),
    CLOCK_STM32H7XX_AHB4_GPIOC          = AxBxENR(3, 2),
    CLOCK_STM32H7XX_AHB4_GPIOD          = AxBxENR(3, 3),
    CLOCK_STM32H7XX_AHB4_GPIOE          = AxBxENR(3, 4),
    CLOCK_STM32H7XX_AHB4_GPIOF          = AxBxENR(3, 5),
    CLOCK_STM32H7XX_AHB4_GPIOG          = AxBxENR(3, 6),
    CLOCK_STM32H7XX_AHB4_GPIOH          = AxBxENR(3, 7),
    CLOCK_STM32H7XX_AHB4_GPIOI          = AxBxENR(3, 8),
    CLOCK_STM32H7XX_AHB4_GPIOJ          = AxBxENR(3, 9),
    CLOCK_STM32H7XX_AHB4_GPIOK          = AxBxENR(3, 10),
    CLOCK_STM32H7XX_AHB4_CRC            = AxBxENR(3, 19),
    CLOCK_STM32H7XX_AHB4_BDMA           = AxBxENR(3, 21),
    CLOCK_STM32H7XX_AHB4_ADC3           = AxBxENR(3, 24),
    CLOCK_STM32H7XX_AHB4_HSEM           = AxBxENR(3, 25),
    CLOCK_STM32H7XX_AHB4_BKPRAM         = AxBxENR(3, 28),
    /* APB3 */
    CLOCK_STM32H7XX_APB3_LTDC           = AxBxENR(4, 3),
    CLOCK_STM32H7XX_APB3_WWDG1          = AxBxENR(4, 6),
    /* APB1L */
    CLOCK_STM32H7XX_APB1L_TIM2          = AxBxENR(5, 0),
    CLOCK_STM32H7XX_APB1L_TIM3          = AxBxENR(5, 1),
    CLOCK_STM32H7XX_APB1L_TIM4          = AxBxENR(5, 2),
    CLOCK_STM32H7XX_APB1L_TIM5          = AxBxENR(5, 3),
    CLOCK_STM32H7XX_APB1L_TIM6          = AxBxENR(5, 4),
    CLOCK_STM32H7XX_APB1L_TIM7          = AxBxENR(5, 5),
    CLOCK_STM32H7XX_APB1L_TIM12         = AxBxENR(5, 6),
    CLOCK_STM32H7XX_APB1L_TIM13         = AxBxENR(5, 7),
    CLOCK_STM32H7XX_APB1L_TIM14         = AxBxENR(5, 8),
    CLOCK_STM32H7XX_APB1L_LPTIM1        = AxBxENR(5, 9),
    CLOCK_STM32H7XX_APB1L_SPI2          = AxBxENR(5, 14),
    CLOCK_STM32H7XX_APB1L_SPI3          = AxBxENR(5, 15),
    CLOCK_STM32H7XX_APB1L_SPDIFRX       = AxBxENR(5, 16),
    CLOCK_STM32H7XX_APB1L_USART2        = AxBxENR(5, 17),
    CLOCK_STM32H7XX_APB1L_USART3        = AxBxENR(5, 18),
    CLOCK_STM32H7XX_APB1L_UART4         = AxBxENR(5, 19),
    CLOCK_STM32H7XX_APB1L_UART5         = AxBxENR(5, 20),
    CLOCK_STM32H7XX_APB1L_I2C1          = AxBxENR(5, 21),
    CLOCK_STM32H7XX_APB1L_I2C2          = AxBxENR(5, 22),
    CLOCK_STM32H7XX_APB1L_I2C3          = AxBxENR(5, 23),
    CLOCK_STM32H7XX_APB1L_CEC           = AxBxENR(5, 27),
    CLOCK_STM32H7XX_APB1L_DAC12         = AxBxENR(5, 29),
    CLOCK_STM32H7XX_APB1L_UART7         = AxBxENR(5, 30),
    CLOCK_STM32H7XX_APB1L_UART8         = AxBxENR(5, 31),
    /* APB1H */
    CLOCK_STM32H7XX_APB1H_CRS           = AxBxENR(6, 1),
    CLOCK_STM32H7XX_APB1H_SWP           = AxBxENR(6, 2),
    CLOCK_STM32H7XX_APB1H_OPAMP         = AxBxENR(6, 4),
    CLOCK_STM32H7XX_APB1H_MDIOS         = AxBxENR(6, 5),
    CLOCK_STM32H7XX_APB1H_FDCAN         = AxBxENR(6, 8),
    CLOCK_STM32H7XX_APB1H_TIM23         = AxBxENR(6, 24),
    CLOCK_STM32H7XX_APB1H_TIM24         = AxBxENR(6, 25),
    /* APB2 */
    CLOCK_STM32H7XX_APB2_TIM1           = AxBxENR(7, 0),
    CLOCK_STM32H7XX_APB2_TIM8           = AxBxENR(7, 1),
    CLOCK_STM32H7XX_APB2_USART1         = AxBxENR(7, 4),
    CLOCK_STM32H7XX_APB2_USART6         = AxBxENR(7, 5),
    CLOCK_STM32H7XX_APB2_SPI1           = AxBxENR(7, 12),
    CLOCK_STM32H7XX_APB2_SPI4           = AxBxENR(7, 13),
    CLOCK_STM32H7XX_APB2_TIM15          = AxBxENR(7, 16),
    CLOCK_STM32H7XX_APB2_TIM16          = AxBxENR(7, 17),
    CLOCK_STM32H7XX_APB2_TIM17          = AxBxENR(7, 18),
    CLOCK_STM32H7XX_APB2_SPI5           = AxBxENR(7, 20),
    CLOCK_STM32H7XX_APB2_SAI1           = AxBxENR(7, 22),
    CLOCK_STM32H7XX_APB2_SAI2           = AxBxENR(7, 23),
    CLOCK_STM32H7XX_APB2_SAI3           = AxBxENR(7, 24),
    CLOCK_STM32H7XX_APB2_DFSDM1         = AxBxENR(7, 28),
    CLOCK_STM32H7XX_APB2_HRTIM          = AxBxENR(7, 29),
    /* APB4 */
    CLOCK_STM32H7XX_APB4_SYSCFG         = AxBxENR(8, 1),
    CLOCK_STM32H7XX_APB4_LPUART1        = AxBxENR(8, 3),
    CLOCK_STM32H7XX_APB4_SPI6           = AxBxENR(8, 5),
    CLOCK_STM32H7XX_APB4_I2C4           = AxBxENR(8, 7),
    CLOCK_STM32H7XX_APB4_LPTIM2         = AxBxENR(8, 9),
    CLOCK_STM32H7XX_APB4_LPTIM3         = AxBxENR(8, 10),
    CLOCK_STM32H7XX_APB4_LPTIM4         = AxBxENR(8, 11),
    CLOCK_STM32H7XX_APB4_LPTIM5         = AxBxENR(8, 12),
    CLOCK_STM32H7XX_APB4_COMP12         = AxBxENR(8, 14),
    CLOCK_STM32H7XX_APB4_VREF           = AxBxENR(8, 15),
    CLOCK_STM32H7XX_APB4_RTCAPB         = AxBxENR(8, 16),
    CLOCK_STM32H7XX_APB4_SAI4           = AxBxENR(8, 17),
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

#ifndef CLOCK_MPC574XX_H
#define CLOCK_MPC574XX_H

#include "clock.h"
#include <stdint.h>

/* clocks list */

#define CLOCK_MPC574XX_F160  CLOCK_SYSCLK
#define CLOCK_MPC574XX_S160  (clock_id_t)1
#define CLOCK_MPC574XX_S80   (clock_id_t)2
#define CLOCK_MPC574XX_FS80  (clock_id_t)3
#define CLOCK_MPC574XX_S40   (clock_id_t)4
#define CLOCK_MPC574XX_F40   (clock_id_t)5
#define CLOCK_MPC574XX_F80   (clock_id_t)6
#define CLOCK_MPC574XX_F20   (clock_id_t)7
#define CLOCK_MPC574XX_COUNT 8

#define CLOCK_MPC574XX_FIRC_FREQ 16000000ul

typedef enum {
    CLOCK_MPC574XX_SYSCLK_FIRC  = 0,
    CLOCK_MPC574XX_SYSCLK_FXOSC = 1,
    CLOCK_MPC574XX_SYSCLK_PHI0  = 2,
    CLOCK_MPC574XX_SYSCLK_COUNT
} clock_mpc574xx_sysclk_t;

typedef enum {
    CLOCK_MPC574XX_FMPLL_CLKIN_FIRC     = 0,
    CLOCK_MPC574XX_FMPLL_CLKIN_FXOSC    = 1,
    CLOCK_MPC574XX_FMPLL_CLKIN_COUNT
} clock_mpc574xx_fmpll_clkin_t;

struct clock_settings {
    clock_mpc574xx_sysclk_t sysclk;
    clock_mpc574xx_fmpll_clkin_t fmpll_clkin;
    unsigned long fxosc;    /* fxosc current value (if used) */
    unsigned long phi0;     /* pll phi0 requested frequency */
    unsigned long s160_div; /* s160 divider (1-8, 0: disable) */
    unsigned long s80_div;  /* s80 divider (1-8, 0: disable) */
    unsigned long fs80_div; /* fs80 divider (1-8, 0: disable) */
    unsigned long s40_div;  /* s40 divider (1-16, 0: disable) */
};

int clock_mpc574xx_init(const struct clock_settings *settings);

/* Peripheral Clocks */

#define CLOCK_MPC574XX_LP_PC_COUNT 8
#define CLOCK_MPC574XX_RUN_PC_COUNT 8

/* RUN flags */
#define R_RUN3  (1 << 7)
#define R_RUN2  (1 << 6)
#define R_RUN1  (1 << 5)
#define R_RUN0  (1 << 4)
#define R_DRUN  (1 << 3)
#define R_SAFE  (1 << 2)
#define R_RESET (1 << 0)

/* LP flags */
#define L_STANDBY0 (1 << 13)
#define L_STOP0    (1 << 10)

int clock_mpc574xx_set_run_pc(size_t index, int flags);
int clock_mpc574xx_set_lp_pc(size_t index, int flags);

typedef enum {
    CLOCK_MPC574XX_PCTL_BCTU        = 0,
    CLOCK_MPC574XX_PCTL_EMIOS0      = 1,
    CLOCK_MPC574XX_PCTL_EMIOS1      = 2,
    CLOCK_MPC574XX_PCTL_EMIOS2      = 3,
    CLOCK_MPC574XX_PCTL_USB_OTG     = 4,
    CLOCK_MPC574XX_PCTL_USB_SPH     = 5,
    CLOCK_MPC574XX_PCTL_MLB         = 6,
    CLOCK_MPC574XX_PCTL_SDHC        = 7,
    CLOCK_MPC574XX_PCTL_SAI0        = 8,
    CLOCK_MPC574XX_PCTL_SAI1        = 9,
    CLOCK_MPC574XX_PCTL_SAI2        = 10,
    CLOCK_MPC574XX_PCTL_ENET        = 15,
    CLOCK_MPC574XX_PCTL_CMP0        = 20,
    CLOCK_MPC574XX_PCTL_CMP1        = 21,
    CLOCK_MPC574XX_PCTL_CMP2        = 22,
    CLOCK_MPC574XX_PCTL_ADC0        = 24,
    CLOCK_MPC574XX_PCTL_ADC1        = 25,
    CLOCK_MPC574XX_PCTL_FLEXRAY     = 28,
    CLOCK_MPC574XX_PCTL_IIC0        = 30,
    CLOCK_MPC574XX_PCTL_IIC1        = 31,
    CLOCK_MPC574XX_PCTL_IIC2        = 32,
    CLOCK_MPC574XX_PCTL_IIC3        = 33,
    CLOCK_MPC574XX_PCTL_DSPI0       = 40,
    CLOCK_MPC574XX_PCTL_DSPI1       = 41,
    CLOCK_MPC574XX_PCTL_DSPI2       = 42,
    CLOCK_MPC574XX_PCTL_DSPI3       = 43,
    CLOCK_MPC574XX_PCTL_LIN0        = 50,
    CLOCK_MPC574XX_PCTL_LIN1        = 51,
    CLOCK_MPC574XX_PCTL_LIN2        = 52,
    CLOCK_MPC574XX_PCTL_LIN3        = 53,
    CLOCK_MPC574XX_PCTL_LIN4        = 54,
    CLOCK_MPC574XX_PCTL_LIN5        = 55,
    CLOCK_MPC574XX_PCTL_LIN6        = 56,
    CLOCK_MPC574XX_PCTL_LIN7        = 57,
    CLOCK_MPC574XX_PCTL_LIN8        = 58,
    CLOCK_MPC574XX_PCTL_LIN9        = 59,
    CLOCK_MPC574XX_PCTL_LIN10       = 60,
    CLOCK_MPC574XX_PCTL_LIN11       = 61,
    CLOCK_MPC574XX_PCTL_LIN12       = 62,
    CLOCK_MPC574XX_PCTL_LIN13       = 63,
    CLOCK_MPC574XX_PCTL_LIN14       = 64,
    CLOCK_MPC574XX_PCTL_LIN15       = 65,
    CLOCK_MPC574XX_PCTL_LIN16       = 66,
    CLOCK_MPC574XX_PCTL_LIN17       = 67,
    CLOCK_MPC574XX_PCTL_FLEXCAN0    = 70,
    CLOCK_MPC574XX_PCTL_FLEXCAN1    = 71,
    CLOCK_MPC574XX_PCTL_FLEXCAN2    = 72,
    CLOCK_MPC574XX_PCTL_FLEXCAN3    = 73,
    CLOCK_MPC574XX_PCTL_FLEXCAN4    = 74,
    CLOCK_MPC574XX_PCTL_FLEXCAN5    = 75,
    CLOCK_MPC574XX_PCTL_FLEXCAN6    = 76,
    CLOCK_MPC574XX_PCTL_FLEXCAN7    = 77,
    CLOCK_MPC574XX_PCTL_JDC         = 82,
    CLOCK_MPC574XX_PCTL_MEMU0       = 83,
    CLOCK_MPC574XX_PCTL_CRC         = 84,
    CLOCK_MPC574XX_PCTL_DMAMUX      = 90,
    CLOCK_MPC574XX_PCTL_PIT_RTI0    = 91,
    CLOCK_MPC574XX_PCTL_WKPU        = 93,
    CLOCK_MPC574XX_PCTL_SPI0        = 96,
    CLOCK_MPC574XX_PCTL_SPI1        = 97,
    CLOCK_MPC574XX_PCTL_SPI2        = 98,
    CLOCK_MPC574XX_PCTL_SPI3        = 99,
    CLOCK_MPC574XX_PCTL_SPI4        = 100,
    CLOCK_MPC574XX_PCTL_SPI5        = 101,
    CLOCK_MPC574XX_PCTL_RTC_API     = 102,
    CLOCK_MPC574XX_PCTL_MEMU1       = 105,
    CLOCK_MPC574XX_PCTL_COUNT
} clock_mpc574xx_pctl_t;

int clock_mpc574xx_set_pctl_lp_cfg(clock_mpc574xx_pctl_t pctl, size_t lp_pc);
int clock_mpc574xx_set_pctl_run_cfg(clock_mpc574xx_pctl_t pctl, size_t run_pc);

#endif

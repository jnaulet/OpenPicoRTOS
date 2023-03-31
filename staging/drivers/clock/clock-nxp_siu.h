#ifndef CLOCK_NXP_SIU_H
#define CLOCK_NXP_SIU_H

#include "clock.h"

#define CLOCK_NXP_SIU_XOSC CLOCK_SYSCLK     /* XOSC (40Mhz) / SYSCLK (PIT) */
#define CLOCK_NXP_SIU_IRC  (clock_id_t)1    /* IRC (16Mhz) */
#define CLOCK_NXP_SIU_PLL0 (clock_id_t)2    /* PLL0 (192Mhz) */
#define CLOCK_NXP_SIU_PLL1 (clock_id_t)3    /* PLL1 (264Mhz) */

/* clocks list */

#define CLOCK_NXP_SIU_XOSC          CLOCK_SYSCLK        /* XOSC / SYSCLK (PIT) */
#define CLOCK_NXP_SIU_IRC           (clock_id_t)1       /* IRC (16Mhz) */
// #define CLOCK_NXP_SIU_ENG_CLK       (clock_id_t)2
// #define CLOCK_NXP_SIU_CLKOUT        (clock_id_t)3
#define CLOCK_NXP_SIU_CORE_CLK      (clock_id_t)4
#define CLOCK_NXP_SIU_PLAT_CLK      (clock_id_t)5
#define CLOCK_NXP_SIU_FM_PER_CLK    (clock_id_t)6
#define CLOCK_NXP_SIU_ETPU_CLK      (clock_id_t)7
#define CLOCK_NXP_SIU_PER_CLK       (clock_id_t)8
// #define CLOCK_NXP_SIU_SD_CLK        (clock_id_t)9
// #define CLOCK_NXP_SIU_PSI5_RX_CLK   (clock_id_t)10
// #define CLOCK_NXP_SIU_PSI5_1US_CLK  (clock_id_t)11
// #define CLOCK_NXP_SIU_TXCLK_REF_CLK (clock_id_t)12

#define CLOCK_NXP_SIU_COUNT 13

/* public interface */

struct clock_settings {
    unsigned long xosc;             /* xosc actual freq */
    unsigned long pll0;             /* pll0 requested freq (ex: 384000000ul) */
    unsigned long pll0_phi_div;     /* pll0_phi divider (1-63) */
    unsigned long pll0_phi1_div;    /* pll0_phi1 divider (4-15) */
    unsigned long pll1;             /* pll1 requested freq (ex: 264000000ul) */
    /* dividers */
    unsigned long sysclkdiv;
    unsigned long perdiv;
    unsigned long fmperdiv;
    unsigned long etpudiv;
};

int clock_nxp_siu_init(struct clock_settings *settings);

#endif

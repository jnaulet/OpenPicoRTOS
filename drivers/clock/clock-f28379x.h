#ifndef CLOCK_F28379X_H
#define CLOCK_F28379X_H

#include "clock.h"

#define CLOCK_F28379X_SYSCLK    CLOCK_SYSCLK            /* CPUx.SYSCLK (pll out) */
#define CLOCK_F28379X_INTOSC2   (clock_id_t)1           /* INTOSC2 (10Mhz) */
#define CLOCK_F28379X_INTOSC1   (clock_id_t)2           /* INTOSC1 (10Mhz) */
#define CLOCK_F28379X_LSPCLK    (clock_id_t)3           /* LSPCLK (CPUCLK/4 by default) */
#define CLOCK_F28379X_EPWMCLK   (clock_id_t)4           /* EPWMCLK (CPUCLK/2 by default) */

typedef enum {
    CLOCK_F28379X_CLKSRC_INTOSC2    = 0,
    CLOCK_F28379X_CLKSRC_XTAL       = 1,
    CLOCK_F28379X_CLKSRC_COUNT
} clock_f28379x_clksrc_t;

struct clock_settings {
    clock_f28379x_clksrc_t clksrc;  /* clock/pll src */
    unsigned long clksrc_freq;      /* known clksrc frequency */
    unsigned long pll;              /* pll requested freq */
};

int clock_f28379x_init(struct clock_settings *settings);

/* f28379x peripheral clocks */
typedef enum {
    /* PCLKCR0 */
    CLOCK_F28379X_PCLK_CLA1         = 0x0,
    CLOCK_F28379X_PCLK_DMA          = 0x2,
    CLOCK_F28379X_PCLK_CPUTIMER0    = 0x3,
    CLOCK_F28379X_PCLK_CPUTIMER1    = 0x4,
    CLOCK_F28379X_PCLK_CPUTIMER2    = 0x5,
    CLOCK_F28379X_PCLK_CPUBGCRC     = 0xd,
    CLOCK_F28379X_PCLK_CLA1BGCRC    = 0xe,
    CLOCK_F28379X_PCLK_HRCAL        = 0x10,
    CLOCK_F28379X_PCLK_TBCLKSYNC    = 0x12,
    CLOCK_F28379X_PCLK_GTBCLKSYNC   = 0x13,
    CLOCK_F28379X_PCLK_ERAD         = 0x18,
    /* PCLKCR1 */
    CLOCK_F28379X_PCLK_EMIF1        = 0x20,
    CLOCK_F28379X_PCLK_EMIF2        = 0x21,
    /* PCLKCR2 */
    CLOCK_F28379X_PCLK_EPWM1        = 0x40,
    CLOCK_F28379X_PCLK_EPWM2        = 0x41,
    CLOCK_F28379X_PCLK_EPWM3        = 0x42,
    CLOCK_F28379X_PCLK_EPWM4        = 0x43,
    CLOCK_F28379X_PCLK_EPWM5        = 0x44,
    CLOCK_F28379X_PCLK_EPWM6        = 0x45,
    CLOCK_F28379X_PCLK_EPWM7        = 0x46,
    CLOCK_F28379X_PCLK_EPWM8        = 0x47,
    CLOCK_F28379X_PCLK_EPWM9        = 0x48,
    CLOCK_F28379X_PCLK_EPWM10       = 0x49,
    CLOCK_F28379X_PCLK_EPWM11       = 0x4a,
    CLOCK_F28379X_PCLK_EPWM12       = 0x4b,
    CLOCK_F28379X_PCLK_EPWM13       = 0x4c,
    CLOCK_F28379X_PCLK_EPWM14       = 0x4d,
    CLOCK_F28379X_PCLK_EPWM15       = 0x4e,
    CLOCK_F28379X_PCLK_EPWM16       = 0x4f,
    /* PCLKCR3 */
    CLOCK_F28379X_PCLK_ECAP1        = 0x60,
    CLOCK_F28379X_PCLK_ECAP2        = 0x61,
    CLOCK_F28379X_PCLK_ECAP3        = 0x62,
    CLOCK_F28379X_PCLK_ECAP4        = 0x63,
    CLOCK_F28379X_PCLK_ECAP5        = 0x64,
    CLOCK_F28379X_PCLK_ECAP6        = 0x65,
    CLOCK_F28379X_PCLK_ECAP7        = 0x66,
    /* PCLKCR4 */
    CLOCK_F28379X_PCLK_EQEP1        = 0x80,
    CLOCK_F28379X_PCLK_EQEP2        = 0x81,
    CLOCK_F28379X_PCLK_EQEP3        = 0x82,
    /* PCLKCR6 */
    CLOCK_F28379X_PCLK_SD1          = 0xc0,
    CLOCK_F28379X_PCLK_SD2          = 0xc1,
    /* PCLKCR7 */
    CLOCK_F28379X_PCLK_SCIA         = 0xe0,
    CLOCK_F28379X_PCLK_SCIB         = 0xe1,
    CLOCK_F28379X_PCLK_SCIC         = 0xe2,
    CLOCK_F28379X_PCLK_SCID         = 0xe3,
    /* PCLKCR8 */
    CLOCK_F28379X_PCLK_SPIA         = 0x100,
    CLOCK_F28379X_PCLK_SPIB         = 0x101,
    CLOCK_F28379X_PCLK_SPIC         = 0x102,
    CLOCK_F28379X_PCLK_SPID         = 0x103,
    /* PCLKCR9 */
    CLOCK_F28379X_PCLK_I2CA         = 0x120,
    CLOCK_F28379X_PCLK_I2CB         = 0x121,
    /* PCLKCR10 */
    CLOCK_F28379X_PCLK_CANA         = 0x140,
    CLOCK_F28379X_PCLK_CANB         = 0x141,
    CLOCK_F28379X_PCLK_MCANA        = 0x144,
    /* PCLKCR11 */
    CLOCK_F28379X_PCLK_MCBSPA       = 0x160,
    CLOCK_F28379X_PCLK_MCBSPB       = 0x161,
    CLOCK_F28379X_PCLK_USBA         = 0x170,
    /* PCLKCR13 */
    CLOCK_F28379X_PCLK_ADCA         = 0x1a0,
    CLOCK_F28379X_PCLK_ADCB         = 0x1a2,
    CLOCK_F28379X_PCLK_ADCC         = 0x1a3,
    CLOCK_F28379X_PCLK_ADCD         = 0x1a4,
    /* PCLKCR14 */
    CLOCK_F28379X_PCLK_CMPSS1       = 0x1c0,
    CLOCK_F28379X_PCLK_CMPSS2       = 0x1c1,
    CLOCK_F28379X_PCLK_CMPSS3       = 0x1c2,
    CLOCK_F28379X_PCLK_CMPSS4       = 0x1c3,
    CLOCK_F28379X_PCLK_CMPSS5       = 0x1c4,
    CLOCK_F28379X_PCLK_CMPSS6       = 0x1c5,
    CLOCK_F28379X_PCLK_CMPSS7       = 0x1c6,
    CLOCK_F28379X_PCLK_CMPSS8       = 0x1c7,
    /* PCLKCR16 */
    CLOCK_F28379X_PCLK_DACA         = 0x210,
    CLOCK_F28379X_PCLK_DACB         = 0x211,
    CLOCK_F28379X_PCLK_DACC         = 0x212,
    /* PLCKCR17 */
    CLOCK_F28379X_PCLK_CLB1         = 0x220,
    CLOCK_F28379X_PCLK_CLB2         = 0x221,
    CLOCK_F28379X_PCLK_CLB3         = 0x222,
    CLOCK_F28379X_PCLK_CLB4         = 0x223,
    CLOCK_F28379X_PCLK_CLB5         = 0x224,
    CLOCK_F28379X_PCLK_CLB6         = 0x225,
    CLOCK_F28379X_PCLK_CLB7         = 0x226,
    CLOCK_F28379X_PCLK_CLB8         = 0x227,
    /* PCLKCR18 */
    CLOCK_F28379X_PCLK_FSITXA       = 0x240,
    CLOCK_F28379X_PCLK_FSITXB       = 0x241,
    CLOCK_F28379X_PCLK_FSIRXA       = 0x250,
    CLOCK_F28379X_PCLK_FSIRXB       = 0x251,
    CLOCK_F28379X_PCLK_FSIRXC       = 0x252,
    CLOCK_F28379X_PCLK_FSIRXD       = 0x253,
    CLOCK_F28379X_PCLK_FSIRXE       = 0x254,
    CLOCK_F28379X_PCLK_FSIRXF       = 0x255,
    CLOCK_F28379X_PCLK_FSIRXG       = 0x256,
    CLOCK_F28379X_PCLK_FSIRXH       = 0x257,
    /* PCLKCR20 */
    CLOCK_F28379X_PCLK_PMBUSA       = 0x280,
    /* PCLKCR21 */
    CLOCK_F28379X_PCLK_DCC0         = 0x2a0,
    CLOCK_F28379X_PCLK_DCC1         = 0x2a1,
    CLOCK_F28379X_PCLK_DCC2         = 0x2a2,
    /* PCLKCR22 */
    CLOCK_F28379X_PCLK_PBISTCLK     = 0x2c0,
    /* PCLKCR23 */
    CLOCK_F28379X_PCLK_ETHERCAT     = 0x2e0,
    CLOCK_F28379X_PCLK_COUNT
} clock_f28379x_pclk_t;

int clock_f28379x_pclk_enable(clock_f28379x_pclk_t pclk);
int clock_f28379x_pclk_disable(clock_f28379x_pclk_t pclk);

/* Implements:
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

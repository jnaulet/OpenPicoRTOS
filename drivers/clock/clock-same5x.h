#ifndef CLOCK_SAME5X_H
#define CLOCK_SAME5X_H

/* Clock driver for SAMD5X/E5X Family */

#include "clock.h"

/* GCLK */
#define CLOCK_SAME5X_GCLK_COUNT 12
#define CLOCK_SAME5X_PCH_COUNT  48

int clock_same5x_gclk_reset(void);

typedef enum {
    CLOCK_SAME5X_GCLK_SRC_XOSC0     = 0,
    CLOCK_SAME5X_GCLK_SRC_XOSC1     = 1,
    CLOCK_SAME5X_GCLK_SRC_GCLK_IN   = 2,
    CLOCK_SAME5X_GCLK_SRC_GCLK_GEN1 = 3,
    CLOCK_SAME5X_GCLK_SRC_OSCULP32K = 4,
    CLOCK_SAME5X_GCLK_SRC_XOSC32K   = 5,
    CLOCK_SAME5X_GCLK_SRC_DFLL      = 6,
    CLOCK_SAME5X_GCLK_SRC_DPLL0     = 7,
    CLOCK_SAME5X_GCLK_SRC_DPLL1     = 8,
    CLOCK_SAME5X_GCLK_SRC_COUNT
} clock_same5x_gclk_src_t;

struct clock_same5x_gclk_settings {
    clock_same5x_gclk_src_t src;
    unsigned long div;
};

int clock_same5x_gclk_generator_setup(size_t index, struct clock_same5x_gclk_settings *settings);
int clock_same5x_gclk_generator_enable(size_t index);
int clock_same5x_gclk_generator_disable(size_t index);

/* FLL */

struct clock_same5x_dfll_settings {
    unsigned long cstep;
    unsigned long fstep;
    unsigned long mul;
};

int clock_same5x_dfll_setup(struct clock_same5x_dfll_settings *settings);
int clock_same5x_dfll_enable(void);
int clock_same5x_dfll_disable(void);

/* PLL */
#define CLOCK_SAME5X_DPLL_COUNT 2

typedef enum {
    CLOCK_SAME5X_DPLL_REFCLK_GCLK   = 0,
    CLOCK_SAME5X_DPLL_REFCLK_XOSC32 = 1,
    CLOCK_SAME5X_DPLL_REFCLK_XOSC0  = 2,
    CLOCK_SAME5X_DPLL_REFCLK_XOSC1  = 3,
    CLOCK_SAME5X_DPLL_REFCLK_COUNT,
} clock_same5x_dpll_refclk_t;

struct clock_same5x_dpll_settings {
    clock_same5x_dpll_refclk_t refclk;
    unsigned long freq;
};

int clock_same5x_dpll_setup(size_t index, struct clock_same5x_dpll_settings *settings);
int clock_same5x_dpll_enable(size_t index);
int clock_same5x_dpll_disable(size_t index);

/* MCLK */

typedef enum {
    /* AHB */
    CLOCK_SAME5X_MCLK_AHB_HPB0              = 0x0,
    CLOCK_SAME5X_MCLK_AHB_HPB1              = 0x1,
    CLOCK_SAME5X_MCLK_AHB_HPB2              = 0x2,
    CLOCK_SAME5X_MCLK_AHB_HPB3              = 0x3,
    CLOCK_SAME5X_MCLK_AHB_DSU               = 0x4,
    CLOCK_SAME5X_MCLK_AHB_NVMCTRL           = 0x6,
    CLOCK_SAME5X_MCLK_AHB_CMCC              = 0x8,
    CLOCK_SAME5X_MCLK_AHB_DMAC              = 0x9,
    CLOCK_SAME5X_MCLK_AHB_USB               = 0xa,
    CLOCK_SAME5X_MCLK_AHB_PAC               = 0xc,
    CLOCK_SAME5X_MCLK_AHB_QSPI              = 0xd,
    CLOCK_SAME5X_MCLK_AHB_GMAC              = 0xe,
    CLOCK_SAME5X_MCLK_AHB_SDHC0             = 0xf,
    CLOCK_SAME5X_MCLK_AHB_SDHC1             = 0x10,
    CLOCK_SAME5X_MCLK_AHB_CAN0              = 0x11,
    CLOCK_SAME5X_MCLK_AHB_CAN1              = 0x12,
    CLOCK_SAME5X_MCLK_AHB_ICM               = 0x13,
    CLOCK_SAME5X_MCLK_AHB_PUKCC             = 0x14,
    CLOCK_SAME5X_MCLK_AHB_QSPI_2X           = 0x15,
    CLOCK_SAME5X_MCLK_AHB_NVMCTRL_SMEEPROM  = 0x16,
    CLOCK_SAME5X_MCLK_AHB_NVMCTRL_CACHE     = 0x17,
    /* APBA */
    CLOCK_SAME5X_MCLK_APBA_PAC              = 0x20,
    CLOCK_SAME5X_MCLK_APBA_PM               = 0x21,
    CLOCK_SAME5X_MCLK_APBA_MCLK             = 0x22,
    CLOCK_SAME5X_MCLK_APBA_RSTC             = 0x23,
    CLOCK_SAME5X_MCLK_APBA_OSCCTRL          = 0x24,
    CLOCK_SAME5X_MCLK_APBA_OSC32KCTRL       = 0x25,
    CLOCK_SAME5X_MCLK_APBA_SUPC             = 0x26,
    CLOCK_SAME5X_MCLK_APBA_GCLK             = 0x27,
    CLOCK_SAME5X_MCLK_APBA_WDT              = 0x28,
    CLOCK_SAME5X_MCLK_APBA_RTC              = 0x29,
    CLOCK_SAME5X_MCLK_APBA_EIC              = 0x2a,
    CLOCK_SAME5X_MCLK_APBA_FREQM            = 0x2b,
    CLOCK_SAME5X_MCLK_APBA_SERCOM0          = 0x2c,
    CLOCK_SAME5X_MCLK_APBA_SERCOM1          = 0x2d,
    CLOCK_SAME5X_MCLK_APBA_TC0              = 0x2e,
    CLOCK_SAME5X_MCLK_APBA_TC1              = 0x2f,
    /* APBB */
    CLOCK_SAME5X_MCLK_APBB_USB              = 0x40,
    CLOCK_SAME5X_MCLK_APBB_DSU              = 0x41,
    CLOCK_SAME5X_MCLK_APBB_NVMCTRL          = 0x42,
    CLOCK_SAME5X_MCLK_APBB_PORT             = 0x44,
    CLOCK_SAME5X_MCLK_APBB_EVSYS            = 0x47,
    CLOCK_SAME5X_MCLK_APBB_SERCOM2          = 0x49,
    CLOCK_SAME5X_MCLK_APBB_SERCOM3          = 0x4a,
    CLOCK_SAME5X_MCLK_APBB_TCC0             = 0x4b,
    CLOCK_SAME5X_MCLK_APBB_TCC1             = 0x4c,
    CLOCK_SAME5X_MCLK_APBB_TC2              = 0x4d,
    CLOCK_SAME5X_MCLK_APBB_TC3              = 0x4e,
    CLOCK_SAME5X_MCLK_APBB_RAMECC           = 0x50,
    /* APBC */
    CLOCK_SAME5X_MCLK_APBC_GMAC             = 0x62,
    CLOCK_SAME5X_MCLK_APBC_TCC2             = 0x63,
    CLOCK_SAME5X_MCLK_APBC_TCC3             = 0x64,
    CLOCK_SAME5X_MCLK_APBC_TC4              = 0x65,
    CLOCK_SAME5X_MCLK_APBC_TC5              = 0x66,
    CLOCK_SAME5X_MCLK_APBC_PDEC             = 0x67,
    CLOCK_SAME5X_MCLK_APBC_AC               = 0x68,
    CLOCK_SAME5X_MCLK_APBC_AES              = 0x69,
    CLOCK_SAME5X_MCLK_APBC_TRNG             = 0x6a,
    CLOCK_SAME5X_MCLK_APBC_ICM              = 0x6b,
    CLOCK_SAME5X_MCLK_APBC_QSPI             = 0x6d,
    CLOCK_SAME5X_MCLK_APBC_CCL              = 0x6e,
    /* APBD */
    CLOCK_SAME5X_MCLK_APBD_SERCOM4          = 0x80,
    CLOCK_SAME5X_MCLK_APBD_SERCOM5          = 0x81,
    CLOCK_SAME5X_MCLK_APBD_SERCOM6          = 0x82,
    CLOCK_SAME5X_MCLK_APBD_SERCOM7          = 0x83,
    CLOCK_SAME5X_MCLK_APBD_TCC4             = 0x84,
    CLOCK_SAME5X_MCLK_APBD_TC6              = 0x85,
    CLOCK_SAME5X_MCLK_APBD_TC7              = 0x86,
    CLOCK_SAME5X_MCLK_APBD_ADC0             = 0x87,
    CLOCK_SAME5X_MCLK_APBD_ADC1             = 0x88,
    CLOCK_SAME5X_MCLK_APBD_DAC              = 0x89,
    CLOCK_SAME5X_MCLK_APBD_I2S              = 0x8a,
    CLOCK_SAME5X_MCLK_APBD_PCC              = 0x8b,
    /* The End */
    CLOCK_SAME5X_MCLK_COUNT
} clock_same5x_mclk_t;

int clock_same5x_mclk_enable(clock_same5x_mclk_t mclk);
int clock_same5x_mclk_disable(clock_same5x_mclk_t mclk);

/* PCHCTRLm mapping */
#define PCHCTRLm(x) (clock_id_t)(x)

#define CLOCK_SAME5X_DFLL48       PCHCTRLm(0)
#define CLOCK_SAME5X_FDPLL0       PCHCTRLm(1)
#define CLOCK_SAME5X_FDPLL1       PCHCTRLm(2)
#define CLOCK_SAME5X_FDPLLn_32K   PCHCTRLm(3)
#define CLOCK_SAME5X_SDHCn_SLOW   PCHCTRLm(3)
#define CLOCK_SAME5X_SERCOMn_SLOW PCHCTRLm(3)
#define CLOCK_SAME5X_EIC          PCHCTRLm(4)
#define CLOCK_SAME5X_MSR          PCHCTRLm(5)
#define CLOCK_SAME5X_REF          PCHCTRLm(6)
#define CLOCK_SAME5X_SERCOM0_CORE PCHCTRLm(7)
#define CLOCK_SAME5X_SERCOM1_CORE PCHCTRLm(8)
#define CLOCK_SAME5X_TC0          PCHCTRLm(9)
#define CLOCK_SAME5X_TC1          PCHCTRLm(9)
#define CLOCK_SAME5X_USB          PCHCTRLm(10)
#define CLOCK_SAME5X_EVSYS(x)     PCHCTRLm(11 + (x))
#define CLOCK_SAME5X_SERCOM2_CORE PCHCTRLm(23)
#define CLOCK_SAME5X_SERCOM3_CORE PCHCTRLm(24)
#define CLOCK_SAME5X_TCC0         PCHCTRLm(25)
#define CLOCK_SAME5X_TCC1         PCHCTRLm(25)
#define CLOCK_SAME5X_TC2          PCHCTRLm(26)
#define CLOCK_SAME5X_TC3          PCHCTRLm(26)
#define CLOCK_SAME5X_CAN0         PCHCTRLm(27)
#define CLOCK_SAME5X_CAN1         PCHCTRLm(28)
#define CLOCK_SAME5X_TCC2         PCHCTRLm(29)
#define CLOCK_SAME5X_TCC3         PCHCTRLm(29)
#define CLOCK_SAME5X_TC4          PCHCTRLm(30)
#define CLOCK_SAME5X_TC5          PCHCTRLm(30)
#define CLOCK_SAME5X_PDEC         PCHCTRLm(31)
#define CLOCK_SAME5X_AC           PCHCTRLm(32)
#define CLOCK_SAME5X_CCL          PCHCTRLm(33)
#define CLOCK_SAME5X_SERCOM4_CORE PCHCTRLm(34)
#define CLOCK_SAME5X_SERCOM5_CORE PCHCTRLm(35)
#define CLOCK_SAME5X_SERCOM6_CORE PCHCTRLm(36)
#define CLOCK_SAME5X_SERCOM7_CORE PCHCTRLm(37)
#define CLOCK_SAME5X_TCC4         PCHCTRLm(38)
#define CLOCK_SAME5X_TC6          PCHCTRLm(39)
#define CLOCK_SAME5X_TC7          PCHCTRLm(39)
#define CLOCK_SAME5X_ADC0         PCHCTRLm(40)
#define CLOCK_SAME5X_ADC1         PCHCTRLm(41)
#define CLOCK_SAME5X_DAC          PCHCTRLm(42)
#define CLOCK_SAME5X_I2S_A        PCHCTRLm(43)
#define CLOCK_SAME5X_I2S_B        PCHCTRLm(44)
#define CLOCK_SAME5X_SDHC0        PCHCTRLm(45)
#define CLOCK_SAME5X_SDHC1        PCHCTRLm(46)
#define CLOCK_SAME5X_CM4_TRACE    PCHCTRLm(47)

#define CLOCK_SAME5X_COUNT 48

int clock_same5x_setup(clock_id_t clkid, size_t gclk_index);
int clock_same5x_enable(clock_id_t clkid);
int clock_same5x_disable(clock_id_t clkid);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

#ifndef CLOCK_MSP430G2XXX_H
#define CLOCK_MSP430G2XXX_H

#include "clock.h"

#define CLOCK_MSP430G2XXX_MCLK  CLOCK_SYSCLK
#define CLOCK_MSP430G2XXX_ACLK  (clock_id_t)1
#define CLOCK_MSP430G2XXX_SMCLK (clock_id_t)2

typedef enum {
    CLOCK_MSP430G2XXX_DCO_1MHZ  = 0,
    CLOCK_MSP430G2XXX_DCO_8MHZ  = 1,
    CLOCK_MSP430G2XXX_DCO_12MHZ = 2,
    CLOCK_MSP430G2XXX_DCO_16MHZ = 3,
    CLOCK_MSP430G2XXX_DCO_COUNT
} clock_msp430g2xxx_dco_t;

typedef enum {
    CLOCK_MSP430G2XXX_LFXT1SX_32768_XTAL    = 0,
    CLOCK_MSP430G2XXX_LFXT1SX_RESERVED      = 1,
    CLOCK_MSP430G2XXX_LFXT1SX_VLOCLK        = 2,
    CLOCK_MSP430G2XXX_LFXT1SX_DIGITAL_EXT   = 3,
    CLOCK_MSP430G2XXX_LFXT1SX_COUNT
} clock_msp430g2xxx_lfxt1sx_t;

typedef enum {
    CLOCK_MSP430G2XXX_DIVX_1    = 0,
    CLOCK_MSP430G2XXX_DIVX_2    = 1,
    CLOCK_MSP430G2XXX_DIVX_4    = 2,
    CLOCK_MSP430G2XXX_DIVX_8    = 3,
    CLOCK_MSP430G2XXX_DIVX_COUNT
} clock_msp430g2xxx_divx_t;

typedef enum {
    CLOCK_MSP430G2XXX_SELMX_DCOCLK      = 0,
    CLOCK_MSP430G2XXX_SELMX_XT2CLK      = 2,
    CLOCK_MSP430G2XXX_SELMX_LFXT1CLK    = 3,
    CLOCK_MSP430G2XXX_SELMX_COUNT,
} clock_msp430g2xxx_selmx_t;

typedef enum {
    CLOCK_MSP430G2XXX_SELS_DCO  = 0,
    CLOCK_MSP430G2XXX_SELS_XT2  = 1,
    CLOCK_MSP430G2XXX_SELS_COUNT,
} clock_msp430g2xxx_sels_t;

struct clock_settings {
    clock_msp430g2xxx_dco_t dco;
    clock_msp430g2xxx_lfxt1sx_t LFXT1Sx;
    clock_msp430g2xxx_divx_t DIVAx;
    clock_msp430g2xxx_selmx_t SELMx;
    clock_msp430g2xxx_divx_t DIVMx;
    clock_msp430g2xxx_sels_t SELS;
    clock_msp430g2xxx_divx_t DIVSx;
};

int clock_msp430g2xxx_init(const struct clock_settings *settings);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

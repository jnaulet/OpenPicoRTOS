#include "clock-msp430g2xxx.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct CLOCK_MODULE_PLUS {
    volatile uint8_t BSCCTL3;
    uint8_t RESERVED0[2];
    volatile uint8_t DCOCTL;
    volatile uint8_t BSCCTL1;
    volatile uint8_t BSCCTL2;
};

static struct CLOCK_MODULE_PLUS *CLK = (struct CLOCK_MODULE_PLUS*)ADDR_CLOCK_MODULE_PLUS;

#define DCOCTL_DCOx_M  0x7u
#define DCOCTL_DCOx(x) (((x) & DCOCTL_DCOx_M) << 5)
#define DCOCTL_MODx_M  0x1fu
#define DCOCTL_MODx(x) ((x) & DCOCTL_MODx_M)

#define BSCCTL1_XT2OFF   (1 << 7)
#define BSCCTL1_XTS      (1 << 6)
#define BSCCTL1_DIVAx_M  0x3u
#define BSCCTL1_DIVAx(x) (((x) & BSCCTL1_DIVAx_M) << 4)
#define BSCCTL1_RSELx_M  0xfu
#define BSCCTL1_RSELx(x) ((x) & BSCCTL1_RSELx_M)

#define BSCCTL2_SELMx_M  0x3u
#define BSCCTL2_SELMx(x) (((x) & BSCCTL2_SELMx_M) << 6)
#define BSCCTL2_DIVMx_M  0x3u
#define BSCCTL2_DIVMx(x) (((x) & BSCCTL2_DIVMx_M) << 4)
#define BSCCTL2_SELS     (1 << 3)
#define BSCCTL2_DIVSx_M  0x3u
#define BSCCTL2_DIVSx(x) (((x) & BSCCTL2_DIVSx_M) << 1)
#define BSCCTL2_DCOR     (1 << 0)

#define BSCCTL3_XT2Sx_M    0x3u
#define BSCCTL3_XT2Sx(x)   (((x) & BSCCTL3_XT2Sx_M) << 6)
#define BSCCTL3_LFXT1Sx_M  0x3u
#define BSCCTL3_LFXT1Sx(x) (((x) & BSCCTL3_LFXT1Sx_M) << 4)
#define BSCCTL3_XCAPx_M    0x3u
#define BSCCTL3_XCAPx(x)   (((x) & BSCCTL3_XCAPx_M) << 2)
#define BSCCTL3_XT2OF      (1 << 1)
#define BSCCTL3_LFXT1OF    (1 << 0)

static struct {
    clock_freq_t dco;
    clock_freq_t lfxt1sx;
    clock_freq_t selmx;
    /* public */
    clock_freq_t aclk;
    clock_freq_t mclk;
    clock_freq_t smclk;
} clocks;

static int dco_setup(const struct clock_settings *settings)
{
    switch (settings->dco) {
    case CLOCK_MSP430G2XXX_DCO_1MHZ:
        CLK->DCOCTL = *(uint8_t*)ADDR_CALDCO_1MHZ;
        CLK->BSCCTL1 = *(uint8_t*)ADDR_CALBC1_1MHZ;
        clocks.dco = (clock_freq_t)1000000;
        break;

    case CLOCK_MSP430G2XXX_DCO_8MHZ:
        CLK->DCOCTL = *(uint8_t*)ADDR_CALDCO_8MHZ;
        CLK->BSCCTL1 = *(uint8_t*)ADDR_CALBC1_8MHZ;
        clocks.dco = (clock_freq_t)8000000;
        break;

    case CLOCK_MSP430G2XXX_DCO_12MHZ:
        CLK->DCOCTL = *(uint8_t*)ADDR_CALDCO_12MHZ;
        CLK->BSCCTL1 = *(uint8_t*)ADDR_CALBC1_12MHZ;
        clocks.dco = (clock_freq_t)12000000;
        break;

    case CLOCK_MSP430G2XXX_DCO_16MHZ:
        CLK->DCOCTL = *(uint8_t*)ADDR_CALDCO_16MHZ;
        CLK->BSCCTL1 = *(uint8_t*)ADDR_CALBC1_16MHZ;
        clocks.dco = (clock_freq_t)16000000;
        break;

    default:
        picoRTOS_assert_void(false);
        return -EINVAL;
    }

    return 0;
}

static int lfxt1sx_setup(const struct clock_settings *settings)
{
    switch (settings->LFXT1Sx) {
    case CLOCK_MSP430G2XXX_LFXT1SX_32768_XTAL:
        clocks.lfxt1sx = (clock_freq_t)32768;
        break;

    case CLOCK_MSP430G2XXX_LFXT1SX_VLOCLK:
        clocks.lfxt1sx = (clock_freq_t)12000;
        break;

    default:
        picoRTOS_assert_void(false);
        return -EINVAL;
    }

    CLK->BSCCTL3 &= ~BSCCTL3_LFXT1Sx(BSCCTL3_LFXT1Sx_M);
    CLK->BSCCTL3 |= BSCCTL3_LFXT1Sx(settings->LFXT1Sx);
    return 0;
}

static int selmx_setup(const struct clock_settings *settings)
{
    switch (settings->SELMx) {
    case CLOCK_MSP430G2XXX_SELMX_DCOCLK:
        clocks.selmx = clocks.dco;
        break;

    case CLOCK_MSP430G2XXX_SELMX_LFXT1CLK:
        clocks.selmx = clocks.lfxt1sx;
        break;

    default:
        picoRTOS_assert_void(false);
        return -EINVAL;
    }

    CLK->BSCCTL2 &= ~BSCCTL2_SELMx(BSCCTL2_SELMx_M);
    CLK->BSCCTL2 |= BSCCTL2_SELMx(settings->SELMx);
    return 0;
}

int clock_msp430g2xxx_init(const struct clock_settings *settings)
{
    picoRTOS_assert(settings->DIVAx < CLOCK_MSP430G2XXX_DIVX_COUNT, return -EINVAL);
    picoRTOS_assert(settings->SELMx < CLOCK_MSP430G2XXX_SELMX_COUNT, return -EINVAL);
    picoRTOS_assert(settings->DIVMx < CLOCK_MSP430G2XXX_DIVX_COUNT, return -EINVAL);
    picoRTOS_assert(settings->SELS < CLOCK_MSP430G2XXX_SELS_COUNT, return -EINVAL);
    picoRTOS_assert(settings->DIVSx < CLOCK_MSP430G2XXX_DIVX_COUNT, return -EINVAL);

    int res;

    if ((res = dco_setup(settings)) < 0) return res;
    if ((res = lfxt1sx_setup(settings)) < 0) return res;
    if ((res = selmx_setup(settings)) < 0) return res;

    CLK->BSCCTL1 &= ~BSCCTL1_DIVAx(BSCCTL1_DIVAx_M);
    CLK->BSCCTL1 |= BSCCTL1_DIVAx(settings->DIVAx);
    clocks.aclk = (clock_freq_t)((unsigned)clocks.lfxt1sx >> settings->DIVAx);

    CLK->BSCCTL2 &= ~BSCCTL2_DIVMx(BSCCTL2_DIVMx_M);
    CLK->BSCCTL2 |= BSCCTL2_DIVMx(settings->DIVMx);
    clocks.mclk = (clock_freq_t)((unsigned)clocks.selmx >> settings->DIVMx);

    /* no support for XT2 */
    CLK->BSCCTL2 &= ~BSCCTL2_SELS;
    CLK->BSCCTL2 &= ~BSCCTL2_DIVSx(BSCCTL2_DIVSx_M);
    CLK->BSCCTL2 |= BSCCTL2_DIVSx(settings->DIVSx);
    clocks.smclk = (clock_freq_t)((unsigned)clocks.dco >> settings->DIVSx);

    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid <= CLOCK_MSP430G2XXX_SMCLK, return (clock_freq_t)-EINVAL);

    switch (clkid) {
    case CLOCK_MSP430G2XXX_MCLK: return clocks.mclk;
    case CLOCK_MSP430G2XXX_ACLK: return clocks.mclk;
    case CLOCK_MSP430G2XXX_SMCLK: return clocks.smclk;
    default: picoRTOS_assert_void(false); break;
    }

    return (clock_freq_t)-EINVAL;
}

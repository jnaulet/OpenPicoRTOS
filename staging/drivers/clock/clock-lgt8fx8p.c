#include "clock-lgt8fx8p.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define PMCR  ((volatile uint8_t*)ADDR_PMCR)
#define CLKPR ((volatile uint8_t*)ADDR_CLKPR)

#define PMCR_PMCE   (1 << 7)
#define PMCR_CLKFS  (1 << 6)
#define PMCR_CLKSS  (1 << 5)
#define PMCR_CLK_M  0x3u
#define PMCR_CLK(x) (((x) & PMCR_CLK_M) << 5)
#define PMCR_WCLKS  (1 << 4)
#define PMCR_OSCKEN (1 << 3)
#define PMCR_OSCMEN (1 << 2)
#define PMCR_RCKEN  (1 << 1)
#define PMCR_RCMEN  (1 << 0)

#define CLKPR_WCE      (1 << 7)
#define CLKPR_CLKOEN1  (1 << 6)
#define CLKPR_CLKOEN0  (1 << 5)
#define CLKPR_CLKPS_M  0xfu
#define CLKPR_CLKPS(x) ((x) & CLKPR_CLKPS_M)

#define HFRC_FREQ 32000000ul    /* 32mhz */
#define LFRC_FREQ 32000ul       /* 32khz */

static struct {
    clock_freq_t mclk;
    clock_freq_t wdt;
} clocks;

static int set_clkps(clock_lgt8fx8p_ps_t ps)
{
    picoRTOS_assert(ps < CLOCK_LGT8FX8P_PS_COUNT, return -EINVAL);

    register uint8_t clkpr = *CLKPR & (uint8_t) ~CLKPR_CLKPS(CLKPR_CLKPS_M);

    arch_suspend();

    if (*CLKPR != clkpr) {
        *CLKPR = (uint8_t)CLKPR_WCE;
        *CLKPR = clkpr;
    }

    arch_resume();
    return 0;
}

static int set_pmcr(register uint8_t value)
{
    arch_suspend();

    if (*PMCR != value) {
        *PMCR = (uint8_t)PMCR_PMCE;
        *PMCR = value;
    }

    arch_resume();
    return 0;
}

int clock_lgt8fx8p_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->mclk < CLOCK_LGT8FX8P_MCLK_COUNT, return -EINVAL);
    picoRTOS_assert(settings->wdclks < CLOCK_LGT8FX8P_WDCLKS_COUNT, return -EINVAL);

    int res;
    uint8_t pmcr = *PMCR & (uint8_t) ~PMCR_CLK(PMCR_CLK_M);

    if (settings->mclk == CLOCK_LGT8FX8P_MCLK_HERC ||
        settings->mclk == CLOCK_LGT8FX8P_MCLK_LERC)
        picoRTOS_assert(settings->erc != 0, return -EINVAL);

    switch (settings->mclk) {
    case CLOCK_LGT8FX8P_MCLK_HFRC:
        pmcr |= (PMCR_CLK(settings->mclk) | PMCR_RCMEN);
        clocks.mclk = (clock_freq_t)HFRC_FREQ;
        break;

    case CLOCK_LGT8FX8P_MCLK_LFRC:
        pmcr |= (PMCR_CLK(settings->mclk) | PMCR_RCKEN);
        clocks.mclk = (clock_freq_t)LFRC_FREQ;
        break;

    case CLOCK_LGT8FX8P_MCLK_HERC:
        pmcr |= PMCR_OSCMEN;
    /*@fallthrough@*/

    case CLOCK_LGT8FX8P_MCLK_LERC:
        picoRTOS_assert(settings->erc != 0, return -EINVAL);
        pmcr |= (PMCR_CLK(settings->mclk) | PMCR_OSCKEN);
        clocks.mclk = (clock_freq_t)settings->erc;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    if (settings->wdclks != CLOCK_LGT8FX8P_WDCLKS_LFRC) {
        pmcr = (pmcr & ~PMCR_WCLKS) | PMCR_RCMEN;
        clocks.wdt = (clock_freq_t)(HFRC_FREQ / 16);
    }else{
        pmcr |= (PMCR_WCLKS | PMCR_RCKEN);
        clocks.wdt = (clock_freq_t)LFRC_FREQ;
    }

    if ((res = set_clkps(settings->ps)) < 0 ||
        (res = set_pmcr(pmcr)) < 0)
        return res;

    arch_set_clock_frequency((unsigned long)clocks.mclk);
    return 0;
}

/* hooks */

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid < (clock_id_t)CLOCK_LGT8FX8P_COUNT,
                    return (clock_freq_t)-EINVAL);

    switch (clkid) {
    case CLOCK_LGT8FX8P_CPU_CLK:    /*@fallthrough@*/
    case CLOCK_LGT8FX8P_PERI_CLK:   /*@fallthrough@*/
    case CLOCK_LGT8FX8P_E2P_CLK:    /*@fallthrough@*/
    case CLOCK_LGT8FX8P_ASY_CLK: return clocks.mclk;
    case CLOCK_LGT8FX8P_WDT_CLK: return clocks.wdt;
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-EIO;
}

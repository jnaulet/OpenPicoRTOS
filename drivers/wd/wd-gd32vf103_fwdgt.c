#include "wd-gd32vf103_fwdgt.h"
#include "picoRTOS.h"

#include <stdint.h>

#define CMD_DISABLE 0x5555
#define CMD_START   0xcccc
#define CMD_RELOAD  0xaaaa

struct WD_GD32VF103_FWDGT {
    volatile uint32_t FWDGT_CTL;
    volatile uint32_t FWDGT_PSC;
    volatile uint32_t FWDGT_RLD;
    volatile uint32_t FWDGT_STAT;
};

#define FWDGT_PSC_PSC_M  0x7u
#define FWDGT_PSC_PSC(x) ((x) & FWDGT_PSC_PSC_M)

#define FWDGT_RLD_RLD_M  0xfffu
#define FWDGT_RLD_RLD(x) ((x) & FWDGT_RLD_RLD_M)

/* Function: wd_gd32vf103_fwdgt_init
 * Initializes a free watchdog timer
 *
 * Parameters:
 *  ctx - The watchdog context to init
 *  base - The WDT base address
 *  clkid - The WDT input clock id
 *
 * Returns:
 * Always 0
 */
int wd_gd32vf103_fwdgt_init(struct wd *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct WD_GD32VF103_FWDGT*)base;
    ctx->clkid = clkid;

    return 0;
}

/* Function: wd_gd32vf103_fwdgt_setup
 * Configures a watchdog
 *
 * Parameters:
 *  ctx - The watchdog to configure
 *  settings - The parameters to set
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int wd_gd32vf103_fwdgt_setup(struct wd *ctx, struct wd_gd32vf103_fwdgt_settings *settings)
{
    unsigned long psc_p2;
    clock_freq_t freq = clock_get_freq(ctx->clkid);
    unsigned long nclk = (unsigned long)freq * settings->timeout_us / 1000000ul;

    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    for (psc_p2 = 2ul; psc_p2 <= 8ul; psc_p2++) {
        unsigned long rld = nclk / (1ul << psc_p2);
        if (rld <= (unsigned long)FWDGT_RLD_RLD_M) {
            ctx->base->FWDGT_CTL = (uint32_t)CMD_DISABLE;
            ctx->base->FWDGT_PSC = (uint32_t)FWDGT_PSC_PSC(psc_p2 - 2ul);
            ctx->base->FWDGT_RLD = (uint32_t)FWDGT_RLD_RLD(rld);
            return 0;
        }
    }

    picoRTOS_break();
    /*@notreached@*/ return -EINVAL;
}

int wd_start(struct wd *ctx)
{
    ctx->base->FWDGT_CTL = (uint32_t)CMD_START;
    return 0;
}

int wd_stop(struct wd *ctx)
{
    ctx->base->FWDGT_CTL = (uint32_t)CMD_DISABLE;
    return 0;
}

void wd_refresh(struct wd *ctx)
{
    ctx->base->FWDGT_CTL = (uint32_t)CMD_RELOAD;
}

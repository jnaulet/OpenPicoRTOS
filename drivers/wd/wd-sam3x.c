#include "wd-sam3x.h"
#include "picoRTOS.h"

#include <stdint.h>

struct WD_SAM3X {
    volatile uint32_t WDT_CR;
    volatile uint32_t WDT_MR;
    volatile uint32_t WDT_SR;
};

#define WDT_CR_KEY_M  0xffu
#define WDT_CR_KEY(x) (((x) & WDT_CR_KEY_M) << 24)
#define WDT_CR_WDRSTT (1 << 0)

#define WDT_MR_WDIDLEHLT (1 << 29)
#define WDT_MR_WDDBGHLT  (1 << 28)
#define WDT_MR_WDD_M     0xfffu
#define WDT_MR_WDD(x)    (((x) & WDT_MR_WDD_M) << 16)
#define WDT_MR_WDDIS     (1 << 15)
#define WDT_MR_WDRPROC   (1 << 14)
#define WDT_MR_WDRSTEN   (1 << 13)
#define WDT_MR_WDFIEN    (1 << 12)
#define WDT_MR_WDV_M     0xfffu
#define WDT_MR_WDV(x)    ((x) & WDT_MR_WDV_M)

#define WD_SR_WDERR (1 << 1)
#define WD_SR_WDUNF (1 << 0)

/* Function: wd_sam3x_init
 * Initializes a watchdog timer
 *
 * Parameters:
 *  ctx - The WD to init
 *  base - The WD base address
 *  clkid - The WD clock ID
 *
 * Returns:
 * Always 0
 */
int wd_sam3x_init(struct wd *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct WD_SAM3X*)base;
    ctx->clkid = clkid;

    /* disable & setup for external reset */
    ctx->base->WDT_MR |= WDT_MR_WDDIS;
    ctx->base->WDT_MR |= WDT_MR_WDRSTEN;

    return 0;
}

/* Function: wd_sam3x_setup
 * Configures a WD
 *
 * Parameters:
 *  ctx - The WD to configure
 *  settings - The configuration settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int wd_sam3x_setup(struct wd *ctx, const struct wd_sam3x_settings *settings)
{
#define DEFAULT_DIVIDER 128
    uint32_t wdv;
    clock_freq_t freq = clock_get_freq(ctx->clkid) / (clock_freq_t)DEFAULT_DIVIDER;

    picoRTOS_assert(freq > 0, return -EIO);

    wdv = (uint32_t)(settings->timeout_ms / (1000ul / (unsigned long)freq));

    picoRTOS_assert(wdv != 0, return -EINVAL);
    picoRTOS_assert(wdv <= (uint32_t)WDT_MR_WDV_M, return -EINVAL);

    ctx->base->WDT_MR &= ~WDT_MR_WDV(WDT_MR_WDV_M);
    ctx->base->WDT_MR |= ~WDT_MR_WDV(wdv);

    return 0;
}

int wd_start(struct wd *ctx)
{
    ctx->base->WDT_MR &= ~WDT_MR_WDDIS;
    return 0;
}

int wd_stop(struct wd *ctx)
{
    ctx->base->WDT_MR |= WDT_MR_WDDIS;
    return 0;
}

void wd_refresh(struct wd *ctx)
{
    ctx->base->WDT_CR = (uint32_t)(WDT_CR_KEY(0xa5) | WDT_CR_WDRSTT);
}

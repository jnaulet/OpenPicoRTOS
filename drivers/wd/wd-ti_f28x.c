#include "wd-ti_f28x.h"
#include "picoRTOS.h"

#include <stdint.h>

struct WD_REGS {
    uint16_t RESERVED0[34];
    volatile uint16_t SCSR;
    volatile uint16_t WDCNTR;
    uint16_t RESERVED1;
    volatile uint16_t WDKEY;
    uint16_t RESERVED2[3];
    volatile uint16_t WDCR;
    volatile uint16_t WDWCR;
};

#define SCSR_WDINTS     (1 << 2)
#define SCSR_WDENINT    (1 << 1)
#define SCSR_WDOVERRIDE (1 << 0)

#define WDCR_WDDIS    (1 << 6)
#define WDCR_WDCHK_M  0x7u
#define WDCR_WDCHK(x) (((x) & WDCR_WDCHK_M) << 3)
#define WDCR_WDPS_M   0x7u
#define WDCR_WDPS(x)  ((x) & WDCR_WDPS_M)

#define WDWCR_FIRSTKEY (1 << 8)
#define WDWCR_MIN_M    0xffu
#define WDWCR_MIN(x)   ((x) & WDWCR_MIN_M)

#define WDCR_WDCHK_ALWAYS 0x5

/* Function: wd_ti_f28x_init
 * Init a watchdog
 *
 * Parameters:
 *  ctx - The watchdog to init
 *  base - The watchdog base address
 *  clkid - The watchdog inpit clock id
 *
 * Returns:
 * Always 0
 */
int wd_ti_f28x_init(struct wd *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct WD_REGS*)base;
    ctx->clkid = clkid;
    return 0;
}

/* Function: wd_ti_f28x_setup
 * Configures a watchdog
 *
 * Parameters:
 *  ctx - The watchdog to configure
 *  settings - The specific settings for this watchdog
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int wd_ti_f28x_setup(struct wd *ctx, struct wd_ti_f28x_settings *settings)
{
    if (!picoRTOS_assert(settings->wdps < WD_TI_F28X_WDPS_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->wdwcr_min <= WDWCR_MIN_M)) return -EINVAL;

    ASM(" eallow");

    /* Prescaler */
    ctx->base->WDCR = (uint16_t)(WDCR_WDDIS |
                                 WDCR_WDPS(settings->wdps) |
                                 WDCR_WDCHK(WDCR_WDCHK_ALWAYS));
    /* Window */
    ctx->base->WDWCR &= ~WDWCR_MIN(WDWCR_MIN_M);
    ctx->base->WDWCR |= WDWCR_MIN(settings->wdwcr_min);

    ASM(" edis");
    return 0;
}

/* hooks */

int wd_start(struct wd *ctx)
{
    ASM(" eallow");
    ctx->base->WDCR = (uint16_t)WDCR_WDCHK(WDCR_WDCHK_ALWAYS);
    ASM(" edis");

    return 0;
}

int wd_stop(struct wd *ctx)
{
    ASM(" eallow");
    ctx->base->SCSR |= SCSR_WDOVERRIDE;
    ctx->base->WDCR |= (WDCR_WDDIS | WDCR_WDCHK(WDCR_WDCHK_ALWAYS));
    ASM(" edis");
    return 0;
}

void wd_refresh(struct wd *ctx)
{
    ASM(" eallow");
    ctx->base->WDKEY = (uint16_t)0x55;
    ctx->base->WDKEY = (uint16_t)0xaa;
    ASM(" edis");
}

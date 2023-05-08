#include "wd-pic32mx.h"
#include "picoRTOS.h"

#include <stdint.h>
#include "misc-pic32mx.h"

struct WD_PIC32MX {
    struct PIC32MX_CLR_SET_INV WDTCON;
};

#define WDTCON_WDTCLRKEY_M  0xffffu
#define WDTCON_WDTCLRKEY(x) (((x) & WDTCON_WDTCLRKEY_M) << 16)
#define WDTCON_ON           (1 << 15)
#define WDTCON_RUNDIV_M     0xfu
#define WDTCON_RUNDIV(x)    (((x) & WDTCON_RUNDIV_M) << 8)
#define WDTCON_WDTWINEN     (1 << 0)

/* Function: wd_pic32mx_init
 * Initializes a watchdog
 *
 * Parameters:
 *  ctx - The WD context to init
 *  base - The WD base address
 *
 * Returns:
 * Always 0
 */
int wd_pic32mx_init(struct wd *ctx, int base)
{
    ctx->base = (struct WD_PIC32MX*)base;
    return 0;
}

/* Function: wd_pic32mx_setup
 * Configures the watchdog
 *
 * Parameters:
 *  ctx - The watchdog to init
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int wd_pic32mx_setup(struct wd *ctx, struct wd_pic32mx_settings *settings)
{
    if (!picoRTOS_assert(settings->timeout < WD_PIC32MX_PERIOD_COUNT)) return -EINVAL;

    ctx->base->WDTCON.CLR = (uint32_t)WDTCON_RUNDIV(WDTCON_RUNDIV_M);
    ctx->base->WDTCON.SET = (uint32_t)WDTCON_RUNDIV(settings->timeout);

    if (settings->enable_windowed)
        ctx->base->WDTCON.SET = (uint32_t)WDTCON_WDTWINEN;

    return 0;
}

int wd_start(struct wd *ctx)
{
    ctx->base->WDTCON.SET = (uint32_t)WDTCON_ON;
    return 0;
}

int wd_stop(struct wd *ctx)
{
    ctx->base->WDTCON.CLR = (uint32_t)WDTCON_ON;
    return 0;
}

void wd_refresh(struct wd *ctx)
{
    ctx->base->WDTCON.SET = (uint32_t)WDTCON_WDTCLRKEY(0x5743);
}

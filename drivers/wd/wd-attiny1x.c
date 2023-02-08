#include "wd-attiny1x.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define CCP_IOREG 0xd8
static volatile uint8_t *CCP = (volatile uint8_t*)(ADDR_CPU + 0x4);

struct WDT_ATTINY1X {
    volatile uint8_t CTRLA;
    volatile uint8_t STATUS;
};

#define CTRLA_WINDOW_M  0xfu
#define CTRLA_WINDOW(x) (((x) & CTRLA_WINDOW_M) << 4)
#define CTRLA_PERIOD_M  0xfu
#define CTRLA_PERIOD(x) ((x) & CTRLA_PERIOD_M)

#define STATUS_LOCK     (1 << 7)
#define STATUS_SYNCBUSY (1 << 0)

/* Function: wd_attiny1x_init
 * Initializes the watchdog timer
 *
 * Parameters:
 *  ctx - The WD to init
 *  base - The WD base address
 *
 * Returns:
 * Always 0
 */
int wd_attiny1x_init(struct wd *ctx, struct WDT_ATTINY1X *base)
{
    ctx->base = base;
    ctx->ctrla = 0;
    return 0;
}

/* Function: wd_attiny1x_setup
 * Configures a watchdog timer
 *
 * Parameters:
 *  ctx - The WD to configure
 *  settings - The configuration parameters
 *
 * Return:
 * 0 if success, -errno otherwise
 */
int wd_attiny1x_setup(struct wd *ctx, struct wd_attiny1x_settings *settings)
{
    if (!picoRTOS_assert(settings->window < WD_ATTINY1X_PERIOD_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->period < WD_ATTINY1X_PERIOD_COUNT)) return -EINVAL;

    ctx->ctrla = (uint8_t)(CTRLA_WINDOW(settings->window) | CTRLA_PERIOD(settings->period));
    return 0;
}

int wd_start(struct wd *ctx)
{
    register uint8_t ctrla = ctx->ctrla;

    /* unprotect and set */
    *CCP = (uint8_t)CCP_IOREG;
    ctx->base->CTRLA = ctrla;

    ASM("wdr");
    return 0;
}

int wd_stop(struct wd *ctx)
{
    /* unprotect and set */
    *CCP = (uint8_t)CCP_IOREG;
    ctx->base->CTRLA = 0;
    return 0;
}

void wd_refresh(/*@unused@*/ struct wd *ctx __attribute__((unused)))
{
    ASM("wdr");
}

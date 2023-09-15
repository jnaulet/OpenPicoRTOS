#include "wd-same5x.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <stdint.h>

struct WD_SAME5X {
    volatile uint8_t CTRLA;
    volatile uint8_t CONFIG;
    volatile uint8_t EWCTRL;
    uint8_t RESERVED0;
    volatile uint8_t INTENCLR;
    volatile uint8_t INTENSET;
    volatile uint8_t INTFLAG;
    uint8_t RESERVED1;
    volatile uint32_t SYNCBUSY;
    volatile uint8_t CLEAR;
};

#define CTRLA_ALWAYSON (1 << 7)
#define CTRLA_WEN      (1 << 2)
#define CTRLA_ENABLE   (1 << 1)

#define CONFIG_WINDOW_M  0xfu
#define CONFIG_WINDOW(x) (((x) & CONFIG_WINDOW_M) << 4)
#define CONFIG_PER_M     0xfu
#define CONFIG_PER(x)    ((x) & CONFIG_PER_M)

#define C99_EWCTRL_EWOFFSET_M  0xfu
#define C99_EWCTRL_EWOFFSET(x) ((x) & EWCTRL_EWOFFSET_M)

#define SYNCBUSY_CLEAR    (1 << 4)
#define SYNCBUSY_ALWAYSON (1 << 3)
#define SYNCBUSY_WEN      (1 << 2)
#define SYNCBUSY_ENABLE   (1 << 1)

static int sync_busywait(struct wd *ctx, uint32_t mask)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((ctx->base->SYNCBUSY & mask) != 0 && deadlock-- != 0)
        arch_delay_us(10ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

/* Function: wd_same5x_init
 * Initializes a watchdog timer
 *
 * Parameters:
 *  ctx - The watchdog context to init
 *  base - The WDT base address
 *
 * Returns:
 * Always 0
 */
int wd_same5x_init(struct wd *ctx, int base)
{
    ctx->base = (struct WD_SAME5X*)base;

    /* disable wd & write protection */
    ctx->base->CTRLA = (uint8_t)0;
    return sync_busywait(ctx, (uint32_t)SYNCBUSY_ENABLE);
}

/* Function: wd_same5x_setup
 * Configures a watchdog
 *
 * Parameters:
 *  ctx - The watchdog to configure
 *  settings - The parameters to set
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int wd_same5x_setup(struct wd *ctx, struct wd_same5x_settings *settings)
{
    picoRTOS_assert(settings->period < WD_SAME5X_PERIOD_COUNT, return -EINVAL);

    ctx->base->CONFIG = (uint8_t)CONFIG_PER(settings->period);
    if (settings->window_mode) {
        picoRTOS_assert(settings->window < WD_SAME5X_PERIOD_COUNT, return -EINVAL);
        ctx->base->CONFIG |= CONFIG_WINDOW(settings->window);
        ctx->base->CTRLA = (uint8_t)CTRLA_WEN;
    }

    return sync_busywait(ctx, (uint32_t)SYNCBUSY_WEN);
}

int wd_start(struct wd *ctx)
{
    ctx->base->CTRLA |= CTRLA_ENABLE;
    return sync_busywait(ctx, (uint32_t)SYNCBUSY_ENABLE);
}

int wd_stop(struct wd *ctx)
{
    ctx->base->CTRLA &= ~CTRLA_ENABLE;
    return sync_busywait(ctx, (uint32_t)SYNCBUSY_ENABLE);
}

void wd_refresh(struct wd *ctx)
{
    ctx->base->CLEAR = (uint8_t)0xa5;
}

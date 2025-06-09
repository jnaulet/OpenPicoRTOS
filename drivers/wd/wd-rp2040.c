#include "wd-rp2040.h"
#include "picoRTOS.h"

#define WD_RP2040_LOAD_COUNT   0x1000000ul
#define WD_RP2040_CYCLES_COUNT 512u

struct WD_RP2040 {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t REASON;
    volatile uint32_t SCRATCHn[8];
    volatile uint32_t TICK;
};

#define CTRL_ENABLE (1 << 30)

#define TICK_ENABLE    (1 << 9)
#define TICK_CYCLES_M  0x1ffu
#define TICK_CYCLES(x) ((x) & TICK_CYCLES_M)

/* Function: wd_rp2040_init
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
int wd_rp2040_init(struct wd *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct WD_RP2040*)base;
    ctx->load = (uint32_t)(WD_RP2040_LOAD_COUNT * 2 - 1);

    /* prepare cycles for 1us */
    clock_freq_t freq = clock_get_freq(clkid);
    unsigned int cycles = (unsigned)freq / 1000000u;

    picoRTOS_assert(freq > 0, return -EIO);
    picoRTOS_assert(cycles < WD_RP2040_CYCLES_COUNT, return -EINVAL);

    ctx->base->TICK = (uint32_t)(TICK_ENABLE | TICK_CYCLES(cycles));
    return 0;
}

/* Function: wd_rp2040_setup
 * Configures a WD
 *
 * Parameters:
 *  ctx - the WD to configure
 *  settings - The Configuration settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int wd_rp2040_setup(struct wd *ctx, const struct wd_rp2040_settings *settings)
{
    picoRTOS_assert(settings->timeout_us > 0, return -EINVAL);

    unsigned long load = settings->timeout_us * 2ul;

    picoRTOS_assert(load < WD_RP2040_LOAD_COUNT, return -EINVAL);
    ctx->load = (uint32_t)load;

    ctx->base->LOAD = ctx->load;
    return 0;
}

int wd_start(struct wd *ctx)
{
    ctx->base->CTRL |= CTRL_ENABLE;
    return 0;
}

int wd_stop(struct wd *ctx)
{
    ctx->base->CTRL &= ~CTRL_ENABLE;
    return 0;
}

void wd_refresh(struct wd *ctx)
{
    ctx->base->LOAD = ctx->load;
}

#include "wd-rp2040.h"
#include "picoRTOS.h"

#define WD_RP2040_LOAD_COUNT   0x1000000ul
#define WD_RP2040_CYCLES_COUNT 512ul

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
    ctx->clkid = clkid;
    ctx->load = (uint32_t)(WD_RP2040_LOAD_COUNT * 2 - 1);

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
int wd_rp2040_setup(struct wd *ctx, struct wd_rp2040_settings *settings)
{
    if (!picoRTOS_assert(settings->timeout_us > 0)) return -EINVAL;

    unsigned long load;
    unsigned long tick_cycles;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    for (tick_cycles = 1ul; tick_cycles < WD_RP2040_CYCLES_COUNT; tick_cycles++) {
        load = settings->timeout_us * ((unsigned long)freq / 1000000ul) / tick_cycles;
        /* fix error in logic */
        load *= 2ul;

        if (load < WD_RP2040_LOAD_COUNT)
            break;
    }

    if (!picoRTOS_assert(load < WD_RP2040_LOAD_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(tick_cycles < WD_RP2040_CYCLES_COUNT)) return -EINVAL;

    ctx->load = (uint32_t)load;

    ctx->base->LOAD = ctx->load;
    ctx->base->TICK = (uint32_t)(TICK_ENABLE | TICK_CYCLES(tick_cycles));

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

#include "timer-pic32mx.h"
#include "picoRTOS.h"

#include <stdint.h>
#include "misc-pic32mx.h"

struct TIMER_PIC32MX {
    struct PIC32MX_CLR_SET_INV TxCON;
    struct PIC32MX_CLR_SET_INV TMRx;
    struct PIC32MX_CLR_SET_INV PRx;
    uint32_t RESERVED0[120];
    /* 32-bit mode */
    struct PIC32MX_CLR_SET_INV TyCON;
    struct PIC32MX_CLR_SET_INV TMRy;
    struct PIC32MX_CLR_SET_INV PRy;
};

#define TxCON_ON       (1 << 15)
#define TxCON_SIDL     (1 << 13)    /* T1CON */
#define TxCON_TWDIS    (1 << 12)    /* T1CON */
#define TxCON_TWIP     (1 << 11)    /* T1CON */
#define TxCON_TGATE    (1 << 7)
#define TxCON_TCKPS_M  0x7u         /* T2+CON */
#define TxCON_TCKPS(x) (((x) & TxCON_TCKPS_M) << 4)
#define TxCON_T32      (1 << 3)
#define TxCON_TSYNC    (1 << 3) /* T1CON */
#define TxCON_TCS      (1 << 1)

#define PRx_PR_M  0xffffu
#define PRx_PR(x) ((x) & PRx_PR_M)

int timer_pic32mx_init(/*@out@*/ struct timer *ctx, struct TIMER_PIC32MX *base, clock_id_t clkid)
{
    ctx->base = base;
    ctx->clkid = clkid;
    ctx->is_32bit = false;
    ctx->period = 0;

    return 0;
}

static unsigned long prescaler[TxCON_TCKPS_M + 1] = { 1ul, 2ul, 4ul, 8ul, 16ul, 32ul, 64ul, 256ul };

static int set_period16(struct timer *ctx, timer_period_us_t period)
{
    if (!picoRTOS_assert(period > 0)) return -EINVAL;

    size_t p;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return (int)freq;

    for (p = 0; p <= (size_t)TxCON_TCKPS_M; p++) {
        unsigned long pfreq = (unsigned long)freq / prescaler[p];
        unsigned long ncycles = (pfreq / 1000000ul) * (unsigned long)period;

        if (ncycles <= (unsigned long)PRx_PR_M) {
            /* correct value found */
            ctx->base->TxCON.CLR = (uint32_t)TxCON_TCKPS(TxCON_TCKPS_M);
            ctx->base->TxCON.SET = (uint32_t)TxCON_TCKPS(p);
            ctx->base->PRx.REG = PRx_PR(ncycles);
            /* remember */
            ctx->period = period;
            return 0;
        }
    }

    /* out-of-bounds result */
    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int set_period32(struct timer *ctx, timer_period_us_t period)
{
    if (!picoRTOS_assert(period > 0)) return -EINVAL;

    clock_freq_t freq = clock_get_freq(ctx->clkid);
    unsigned long ncycles = ((unsigned long)freq / 1000000ul) * (unsigned long)period;

    if (!picoRTOS_assert(freq > 0))
        return (int)freq;

    /* correct value found */
    ctx->base->TxCON.CLR = (uint32_t)TxCON_TCKPS(TxCON_TCKPS_M);
    ctx->base->PRx.REG = PRx_PR(ncycles >> 16);
    ctx->base->PRy.REG = PRx_PR(ncycles);

    /* remember */
    ctx->period = period;
    return 0;
}

static int set_period(struct timer *ctx, timer_period_us_t period)
{
    if (!picoRTOS_assert(period > 0)) return -EINVAL;

    if (ctx->is_32bit)
        return set_period32(ctx, period);

    return set_period16(ctx, period);
}

int timer_pic32mx_setup(struct timer *ctx, struct timer_pic32mx_settings *settings)
{
    int res;

    if (settings->t32_mode) {
        ctx->base->TxCON.SET = (uint32_t)TxCON_T32;
        ctx->is_32bit = true;
    }

    if ((res = set_period(ctx, settings->period)) < 0)
        return res;

    return 0;
}

void timer_pic32mx_start(struct timer *ctx)
{
    ctx->base->TxCON.SET = (uint32_t)TxCON_ON;
}

void timer_pic32mx_stop(struct timer *ctx)
{
    ctx->base->TxCON.CLR = (uint32_t)TxCON_ON;
}

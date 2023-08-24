#include "wd-stm32h7xx_iwdg.h"
#include "picoRTOS.h"

#include <stdint.h>

#define KEY_AAAA 0xaaaa
#define KEY_CCCC 0xcccc
#define KEY_5555 0x5555

struct WD_STM32H7XX_IWDG {
    volatile uint32_t KR;
    volatile uint32_t PR;
    volatile uint32_t RLR;
    volatile uint32_t SR;
    volatile uint32_t WINR;
};

#define KR_KEY_M  0xffffu
#define KR_KEY(x) ((x) & KR_KEY_M)

#define PR_PR_M  0x7u
#define PR_PR(x) ((x) & PR_PR_M)

#define RLR_RL_M  0xfffu
#define RLR_RL(x) ((x) & RLR_RL_M)

#define SR_WVU (1 << 2)
#define SR_RVU (1 << 1)
#define SR_PVU (1 << 0)

#define WINR_WIN_M  0xfffu
#define WINR_WIN(x) ((x) & WINR_WIN_M)

/* Function: wd_stm32h7xx_iwdg_init
 * Initializes a free-running watchdog timer
 *
 * Parameters:
 *  ctx - The watchdog context to init
 *  base - The WDT base address
 *  clkid - The WDT input clock id
 *
 * Returns:
 * Always 0
 */
int wd_stm32h7xx_iwdg_init(/*@out@*/ struct wd *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct WD_STM32H7XX_IWDG*)base;
    ctx->clkid = clkid;

    return 0;
}

static int reg_update_busywait(struct wd *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT * 1000;

    while (deadlock-- != 0)
        if (ctx->base->SR == 0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

/* Function: wd_stm32h7xx_iwdg_setup
 * Configures a IWDG watchdog
 *
 * Parameters:
 *  ctx - The watchdog to configure
 *  settings - The parameters to set
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int wd_stm32h7xx_iwdg_setup(struct wd *ctx, struct wd_stm32h7xx_iwdg_settings *settings)
{
    picoRTOS_assert(settings->timeout_ms > 0, return -EINVAL);
    picoRTOS_assert(settings->window_ms > 0, return -EINVAL);
    picoRTOS_assert(settings->window_ms <= settings->timeout_ms, return -EINVAL);

    size_t p;
    clock_freq_t freq;
    static const unsigned long pr[] = { 4ul, 8ul, 16ul, 32ul, 64ul, 128ul, 256ul };

    picoRTOS_assert((freq = clock_get_freq(ctx->clkid)) > 0,
                    return -EIO);

    /* determine prescaler + reload + window */
    for (p = 0; p < (size_t)7; p++) {
        unsigned long tick = ((unsigned long)freq / pr[p]) / 1000ul;
        unsigned long rl = settings->timeout_ms * tick;
        unsigned long win = settings->window_ms * tick;

        if (rl <= (unsigned long)RLR_RL_M &&
            win <= (unsigned long)WINR_WIN_M) {
            int res;
            /* set values */
            ctx->base->KR = (uint32_t)KEY_CCCC;
            ctx->base->KR = (uint32_t)KEY_5555;
            /* pr + reload */
            ctx->base->PR = (uint32_t)PR_PR(p);
            ctx->base->RLR = (uint32_t)RLR_RL(rl);
            /* wait for registers to be updated */
            if ((res = reg_update_busywait(ctx)) < 0)
                return res;
            /* set window */
            ctx->base->WINR = (uint32_t)WINR_WIN(win);
            return 0;
        }
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int wd_start(struct wd *ctx)
{
    ctx->base->KR = (uint32_t)KEY_CCCC;
    return 0;
}

int wd_stop(/*@unused@*/ struct wd *ctx __attribute__((unused)))
{
    return 0;
}

void wd_refresh(struct wd *ctx)
{
    ctx->base->KR = (uint32_t)KEY_AAAA;
}

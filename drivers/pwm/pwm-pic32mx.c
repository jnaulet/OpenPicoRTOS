#include "pwm-pic32mx.h"
#include "picoRTOS.h"

#include <stdint.h>
#include "misc-pic32mx.h"

struct PWM_PIC32MX_TC {
    struct PIC32MX_CLR_SET_INV TxCON;
    struct PIC32MX_CLR_SET_INV TMRx;
    struct PIC32MX_CLR_SET_INV PRx;
    /* TODO: 32bit extension */
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

struct PWM_PIC32MX_IC {
    struct PIC32MX_CLR_SET_INV ICxCON;
    struct PIC32MX_CLR_SET_INV ICxBUF;
};

#define ICxCON_ON     (1 << 15)
#define ICxCON_SIDL   (1 << 13)
#define ICxCON_FEDGE  (1 << 9)
#define ICxCON_C32    (1 << 8)
#define ICxCON_ICTMR  (1 << 7)
#define ICxCON_ICI_M  0x3u
#define ICxCON_ICI(x) (((x) & ICxCON_ICI_M) << 5)
#define ICxCON_ICOV   (1 << 4)
#define ICxCON_ICBNE  (1 << 3)
#define ICxCON_ICM_M  0x7u
#define ICxCON_ICM(x) ((x) & ICxCON_ICM_M)

struct PWM_PIC32MX_OC {
    struct PIC32MX_CLR_SET_INV OCxCON;
    struct PIC32MX_CLR_SET_INV OCxR;
    struct PIC32MX_CLR_SET_INV OCxRS;
};

#define OCxCON_ON     (1 << 15)
#define OCxCON_SIDL   (1 << 13)
#define OCxCON_OC32   (1 << 5)
#define OCxCON_OCFLT  (1 << 4)
#define OCxCON_OCTSEL (1 << 3)
#define OCxCON_OCM_M  0x7u
#define OCxCON_OCM(x) ((x) & OCxCON_OCM_M)

/* Function: pwm_pic32mx_init
 * Initializes a PWM
 *
 * Parameters:
 *  ctx - The PWM to init
 *  oc - The output compare block address
 *  tc - The corresponding timer register
 *  clkid - The timer clock id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_pic32mx_init(struct pwm *ctx, struct PWM_PIC32MX_OC *oc,
                     struct PWM_PIC32MX_TC *tc, clock_id_t clkid)
{
    ctx->oc = oc;
    ctx->tc = tc;
    ctx->clkid = clkid;
    ctx->ncycles = 0;
    ctx->invert = false;

    /* turn on & set PWM mode, fault pin disabled */
    ctx->oc->OCxCON.CLR = (uint32_t)OCxCON_OCM(OCxCON_OCM_M);
    ctx->oc->OCxCON.SET = (uint32_t)(OCxCON_ON | OCxCON_OCM(0x6));

    return 0;
}

/* Function: pwm_pic32mx_setup
 * Configures a PWM
 *
 * Parameters:
 *  ctx - The PWM to configure
 *  settings - The PWM settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_pic32mx_setup(struct pwm *ctx, struct pwm_pic32mx_settings *settings)
{
    if (!picoRTOS_assert(settings->octsel < PWM_PIC32MX_OCTSEL_COUNT)) return -EAGAIN;

    if (settings->octsel == PWM_PIC32MX_OCTSEL_TIMERY)
        ctx->oc->OCxCON.SET = (uint32_t)OCxCON_OCTSEL;
    else
        ctx->oc->OCxCON.CLR = (uint32_t)OCxCON_OCTSEL;

    ctx->invert = settings->invert;
    return 0;
}

/* Hooks */

static unsigned long prescaler[TxCON_TCKPS_M + 1] =
{ 1ul, 2ul, 4ul, 8ul, 16ul, 32ul, 64ul, 256ul };

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
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
            ctx->tc->TxCON.CLR = (uint32_t)TxCON_TCKPS(TxCON_TCKPS_M);
            ctx->tc->TxCON.SET = (uint32_t)TxCON_TCKPS(p);
            ctx->tc->PRx.REG = (uint32_t)PRx_PR(ncycles);
            /* remember */
            ctx->ncycles = (size_t)ncycles;
            return 0;
        }
    }

    /* out-of-bounds result */
    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    size_t ocxr;

    if (ctx->invert)
        duty_cycle = (pwm_duty_cycle_t)PWM_DUTY_CYCLE_MAX - duty_cycle;

    ocxr = ((size_t)duty_cycle * ctx->ncycles) >> 16;
    ctx->oc->OCxRS.REG = (uint32_t)(uint16_t)ocxr;

    return 0;
}

void pwm_start(struct pwm *ctx)
{
    ctx->tc->TxCON.SET = (uint32_t)TxCON_ON;
    ctx->oc->OCxCON.SET = (uint32_t)OCxCON_ON;
}

void pwm_stop(struct pwm *ctx)
{
    ctx->oc->OCxCON.CLR = (uint32_t)OCxCON_ON;
}

#include "pwm-sam.h"
#include "picoRTOS.h"

#include <stdint.h>

struct PWM_SAM {
    volatile uint32_t PWM_CLK;
    volatile uint32_t PWM_ENA;
    volatile uint32_t PWM_DIS;
    volatile uint32_t PWM_SR;
    volatile uint32_t PWM_IER1;
    volatile uint32_t PWM_IDR1;
    volatile uint32_t PWM_IMR1;
    volatile uint32_t PWM_ISR1;
    volatile uint32_t PWM_SCM;
    uint32_t RESERVED0;
    volatile uint32_t PWM_SCUC;
    volatile uint32_t PWM_SCUP;
    volatile uint32_t PWM_SCUPUPD;
    volatile uint32_t PWM_IER2;
    volatile uint32_t PWM_IDR2;
    volatile uint32_t PWM_IMR2;
    volatile uint32_t PWM_ISR2;
    volatile uint32_t PWM_OOV;
    volatile uint32_t PWM_OS;
    volatile uint32_t PWM_OSS;
    volatile uint32_t PWM_OSC;
    volatile uint32_t PWM_OSSUPD;
    volatile uint32_t PWM_OSCUPD;
    volatile uint32_t PWM_FMR;
    volatile uint32_t PWM_FSR;
    volatile uint32_t PWM_FCR;
    volatile uint32_t PWM_FPV;
    volatile uint32_t PWM_FPE1;
    volatile uint32_t PWM_FPE2;
    uint32_t RESERVED1[2];
    volatile uint32_t PWM_ELMR0;
    volatile uint32_t PWM_ELMR1;
    uint32_t RESERVED2[11];
    volatile uint32_t PWM_SMMR;
    uint32_t RESERVED3[12];
    volatile uint32_t PWM_WPCR;
    volatile uint32_t PWM_WPSR;
    uint32_t RESERVED4[17];
    volatile uint32_t PWM_CMPV0;
    volatile uint32_t PWM_CMPVUPD0;
    volatile uint32_t PWM_CMPM0;
    volatile uint32_t PWM_CMPMUPD0;
    volatile uint32_t PWM_CMPV1;
    volatile uint32_t PWM_CMPVUPD1;
    volatile uint32_t PWM_CMPM1;
    volatile uint32_t PWM_CMPMUPD1;
    volatile uint32_t PWM_CMPV2;
    volatile uint32_t PWM_CMPVUPD2;
    volatile uint32_t PWM_CMPM2;
    volatile uint32_t PWM_CMPMUPD2;
    volatile uint32_t PWM_CMPV3;
    volatile uint32_t PWM_CMPVUPD3;
    volatile uint32_t PWM_CMPM3;
    volatile uint32_t PWM_CMPMUPD3;
    volatile uint32_t PWM_CMPV4;
    volatile uint32_t PWM_CMPVUPD4;
    volatile uint32_t PWM_CMPM4;
    volatile uint32_t PWM_CMPMUPD4;
    volatile uint32_t PWM_CMPV5;
    volatile uint32_t PWM_CMPVUPD5;
    volatile uint32_t PWM_CMPM5;
    volatile uint32_t PWM_CMPMUPD5;
    volatile uint32_t PWM_CMPV6;
    volatile uint32_t PWM_CMPVUPD6;
    volatile uint32_t PWM_CMPM6;
    volatile uint32_t PWM_CMPMUPD6;
    volatile uint32_t PWM_CMPV7;
    volatile uint32_t PWM_CMPVUPD7;
    volatile uint32_t PWM_CMPM7;
    volatile uint32_t PWM_CMPMUPD7;
    uint32_t RESERVED5[20];
    struct PWM_SAM_CH {
        volatile uint32_t PWM_CMR;
        volatile uint32_t PWM_CDTY;
        volatile uint32_t PWM_CDTYUPD;
        volatile uint32_t PWM_CPRD;
        volatile uint32_t PWM_CPRDUPD;
        volatile uint32_t PWM_CCNT;
        volatile uint32_t PWM_DT;
        volatile uint32_t PWM_DTUPD;
    } PWM_CH[PWM_SAM_CHANNEL_COUNT];
};

#define PWM_CLK_PREB_M  0xfu
#define PWM_CLK_PREB(x) (((x) & PWM_CLK_PREB_M) << 24)
#define PWM_CLK_DIVB_M  0xffu
#define PWM_CLK_DIVB(x) (((x) & PWM_CLK_DIVB_M) << 16)
#define PWM_CLK_PREA_M  0xfu
#define PWM_CLK_PREA(x) (((x) & PWM_CLK_PREA_M) << 8)
#define PWM_CLK_DIVA_M  0xffu
#define PWM_CLK_DIVA(x) ((x) & PWM_CLK_DIVB_M)

#define PWM_CMRn_DTLI    (1 << 18)
#define PWM_CMRn_DTHI    (1 << 17)
#define PWM_CMRn_DTE     (1 << 16)
#define PWM_CMRn_CES     (1 << 10)
#define PWM_CMRn_CPOL    (1 << 9)
#define PWM_CMRn_CALG    (1 << 8)
#define PWM_CMRn_CPRE_M  0xfu
#define PWM_CMRn_CPRE(x) ((x) & PWM_CMRn_CPRE_M)

#define PWM_CDTYn_CDTY_M  0xffffffu
#define PWM_CDTYn_CDTY(x) ((x) & PWM_CDTYn_CDTY_M)

#define PWM_CPRDn_CPRD_M  0xffffffu
#define PWM_CPRDn_CPRD(x) ((x) & PWM_CPRDn_CPRD_M)

/* Function: pwm_sam_init
 * Initializes a PWM block
 *
 * Parameters:
 *  ctx - The PWM block to init
 *  base - The PWM block base address
 *  clkid - The clock id associated with the PWM block
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_sam_init(struct pwm_sam *ctx, struct PWM_SAM *base, clock_id_t clkid)
{
    ctx->base = base;
    ctx->freq = clock_get_freq(clkid);

    if (!picoRTOS_assert(ctx->freq > 0))
        return -EIO;

    return 0;
}

/* Function: pwm_sam_setup
 * Configures a PWM block
 *
 * Parameters:
 *  ctx - The PWM block to configure
 *  settings - The configuration setting to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_sam_setup(struct pwm_sam *ctx, struct pwm_sam_settings *settings)
{
    if (!picoRTOS_assert(settings->clock_a_pre < PWM_SAM_CLOCK_PRE_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->clock_a_div < PWM_SAM_CLOCK_DIV_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->clock_b_pre < PWM_SAM_CLOCK_PRE_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->clock_b_div < PWM_SAM_CLOCK_DIV_COUNT)) return -EINVAL;

    ctx->base->PWM_CLK = (uint32_t)(PWM_CLK_PREB(settings->clock_b_pre) |
                                    PWM_CLK_DIVB(settings->clock_b_div) |
                                    PWM_CLK_PREA(settings->clock_a_pre) |
                                    PWM_CLK_DIVA(settings->clock_a_div));

    ctx->clock_a_fact = (1ul << settings->clock_a_pre) * settings->clock_a_div;
    ctx->clock_b_fact = (1ul << settings->clock_b_pre) * settings->clock_b_div;

    return 0;
}

static clock_freq_t pwm_sam_get_freq(struct pwm_sam *ctx, pwm_sam_pwm_cpre_t cpre)
{
    if (!picoRTOS_assert(cpre < PWM_SAM_PWM_CPRE_COUNT)) return (clock_freq_t)-EINVAL;

    clock_freq_t ret;

    switch (cpre) {
    case PWM_SAM_PWM_CPRE_MCK:          /*@fallthrough@*/
    case PWM_SAM_PWM_CPRE_MCK_DIV_2:    /*@fallthrough@*/
    case PWM_SAM_PWM_CPRE_MCK_DIV_4:    /*@fallthrough@*/
    case PWM_SAM_PWM_CPRE_MCK_DIV_8:    /*@fallthrough@*/
    case PWM_SAM_PWM_CPRE_MCK_DIV_16:   /*@fallthrough@*/
    case PWM_SAM_PWM_CPRE_MCK_DIV_32:   /*@fallthrough@*/
    case PWM_SAM_PWM_CPRE_MCK_DIV_64:   /*@fallthrough@*/
    case PWM_SAM_PWM_CPRE_MCK_DIV_128:  /*@fallthrough@*/
    case PWM_SAM_PWM_CPRE_MCK_DIV_256:  /*@fallthrough@*/
    case PWM_SAM_PWM_CPRE_MCK_DIV_512:  /*@fallthrough@*/
    case PWM_SAM_PWM_CPRE_MCK_DIV_1024:
        ret = (clock_freq_t)((size_t)ctx->freq >> cpre);
        break;

    case PWM_SAM_PWM_CPRE_CLKA:
        ret = ctx->freq / (clock_freq_t)ctx->clock_a_fact;
        break;

    case PWM_SAM_PWM_CPRE_CLKB:
        ret = ctx->freq / (clock_freq_t)ctx->clock_b_fact;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return (clock_freq_t)-EIO;
    }

    return ret;
}

/* channels */

/* Function: pwm_sam_pwm_init
 * Initializes a PWM output
 *
 * Parameters:
 *  ctx - The PWM output to init
 *  pwm - The parent PWM block
 *  channel - The output channel
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_sam_pwm_init(struct pwm *ctx, struct pwm_sam *pwm, size_t channel)
{
    if (!picoRTOS_assert(channel < (size_t)PWM_SAM_CHANNEL_COUNT)) return -EINVAL;

    ctx->pwm = pwm;
    ctx->ch = &pwm->base->PWM_CH[channel];
    ctx->channel = channel;
    ctx->freq = pwm_sam_get_freq(ctx->pwm, PWM_SAM_PWM_CPRE_MCK);
    ctx->ncycles = 0;

    return 0;
}

/* Function: pwm_sam_pwm_setup
 * Configures a PWM output
 *
 * Parameters:
 *  ctx - The PWM output to init
 *  settings - The configuration settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_sam_pwm_setup(struct pwm *ctx, struct pwm_sam_pwm_settings *settings)
{
    if (!picoRTOS_assert(settings->cpre < PWM_SAM_PWM_CPRE_COUNT)) return -EINVAL;

    if (settings->cpol) ctx->ch->PWM_CMR |= PWM_CMRn_CPOL;
    else ctx->ch->PWM_CMR &= ~PWM_CMRn_CPOL;

    ctx->ch->PWM_CMR &= ~PWM_CMRn_CPRE(PWM_CMRn_CPRE_M);
    ctx->ch->PWM_CMR |= PWM_CMRn_CPRE(settings->cpre);

    ctx->freq = pwm_sam_get_freq(ctx->pwm, settings->cpre);
    return 0;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
    if (!picoRTOS_assert(period > 0)) return -EINVAL;

    ctx->ncycles = ((size_t)ctx->freq / (size_t)1000000) * (size_t)period;
    ctx->ch->PWM_CPRDUPD = (uint32_t)PWM_CPRDn_CPRD(ctx->ncycles);

    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    size_t cdty = ((size_t)duty_cycle * ctx->ncycles) >> 16;

    ctx->ch->PWM_CDTYUPD = (uint32_t)PWM_CPRDn_CPRD(cdty);
    return 0;
}

void pwm_start(struct pwm *ctx)
{
    ctx->pwm->base->PWM_ENA |= (1 << ctx->channel);
}

void pwm_stop(struct pwm *ctx)
{
    ctx->pwm->base->PWM_ENA &= ~(1 << ctx->channel);
}

#include "pwm-gd32vf103.h"
#include "picoRTOS.h"

#include <stdint.h>

#define PWM_MODE0 6
#define PWM_MODE1 7

struct PWM_GD32VF103_TIMER {
    volatile uint32_t TIMERx_CTL0;
    volatile uint32_t TIMERx_CTL1;
    volatile uint32_t TIMERx_SMCFG;
    volatile uint32_t TIMERx_DMAINTEN;
    volatile uint32_t TIMERx_INTF;      /* not on TIMER5 & 6 */
    volatile uint32_t TIMERx_SWEVG;     /* not on TIMER5 & 6 */
    volatile uint32_t TIMERx_CHCTL0;    /* not on TIMER5 & 6 */
    volatile uint32_t TIMERx_CHCTL1;    /* not on TIMER5 & 6 */
    volatile uint32_t TIMERx_CHCTL2;    /* not on TIMER5 & 6 */
    volatile uint32_t TIMERx_CNT;
    volatile uint32_t TIMERx_PSC;
    volatile uint32_t TIMERx_CAR;
    volatile uint32_t TIMERx_CREP;      /* not on TIMER5 & 6 */
    volatile uint32_t TIMERx_CH0CV;     /* not on TIMER5 & 6 */
    volatile uint32_t TIMERx_CH1CV;     /* not on TIMER5 & 6 */
    volatile uint32_t TIMERx_CH2CV;     /* not on TIMER5 & 6 */
    volatile uint32_t TIMERx_CH3CV;     /* not on TIMER5 & 6 */
    volatile uint32_t TIMERx_CCHP;      /* only on TIMER0 */
    volatile uint32_t TIMERx_DMACFG;    /* not on TIMER5 & 6 */
    volatile uint32_t TIMERx_DMATB;     /* not on TIMER5 & 6 */
};

#define TIMERx_CTL0_CKDIV_M  0x3u
#define TIMERx_CTL0_CKDIV(x) (((x) & TIMERx_CTL0_CKDIV_M) << 8)
#define TIMERx_CTL0_ARSE     (1 << 7)
#define TIMERx_CTL0_CAM_M    0x3u
#define TIMERx_CTL0_CAM(x)   (((x) & TIMERx_CTL0_CAM_M) << 5)
#define TIMERx_CTL0_DIR      (1 << 4)
#define TIMERx_CTL0_SPM      (1 << 3)
#define TIMERx_CTL0_UPS      (1 << 2)
#define TIMERx_CTL0_UPDIS    (1 << 1)
#define TIMERx_CTL0_CEN      (1 << 0)

#define TIMERx_CTL1_TI0S   (1 << 7)
#define TIMERx_CTL1_MMC_M  0x7u
#define TIMERx_CTL1_MMC(x) (((x) & TIMERx_CTL1_MMC_M) << 4)
#define TIMERx_CTL1_DMAS   (1 << 3)

#define TIMERx_INTF_CH3OF (1 << 12)
#define TIMERx_INTF_CH2OF (1 << 11)
#define TIMERx_INTF_CH1OF (1 << 10)
#define TIMERx_INTF_CH0OF (1 << 9)
#define TIMERx_INTF_TRGIF (1 << 6)
#define TIMERx_INTF_CH3IF (1 << 4)
#define TIMERx_INTF_CH2IF (1 << 3)
#define TIMERx_INTF_CH1IF (1 << 2)
#define TIMERx_INTF_CH0IF (1 << 1)
#define TIMERx_INTF_UPIF  (1 << 0)

#define TIMERx_SWEVG_TRGG (1 << 6)
#define TIMERx_SWEVG_CH3G (1 << 4)
#define TIMERx_SWEVG_CH2G (1 << 3)
#define TIMERx_SWEVG_CH1G (1 << 2)
#define TIMERx_SWEVG_CH0G (1 << 1)
#define TIMERx_SWEVG_UPG  (1 << 0)

/* output mode */
#define TIMERx_CHCTL0_CH1COMCEN    (1 << 15)
#define TIMERx_CHCTL0_CH1COMCTL_M  0x7u
#define TIMERx_CHCTL0_CH1COMCTL(x) (((x) & TIMERx_CHCTL0_CH1COMCTL_M) << 12)
#define TIMERx_CHCTL0_CH1COMSEN    (1 << 11)
#define TIMERx_CHCTL0_CH1COMFEN    (1 << 10)
#define TIMERx_CHCTL0_CH1MS_M      0x3u
#define TIMERx_CHCTL0_CH1MS(x)     (((x) & TIMERx_CHCTL0_CH1MS_M) << 8)
#define TIMERx_CHCTL0_CH0COMCEN    (1 << 7)
#define TIMERx_CHCTL0_CH0COMCTL_M  0x7u
#define TIMERx_CHCTL0_CH0COMCTL(x) (((x) & TIMERx_CHCTL0_CH0COMCTL_M) << 4)
#define TIMERx_CHCTL0_CH0COMSEN    (1 << 3)
#define TIMERx_CHCTL0_CH0COMFEN    (1 << 2)
#define TIMERx_CHCTL0_CH0MS_M      0x3u
#define TIMERx_CHCTL0_CH0MS(x)     ((x) & TIMERx_CHCTL0_CH1MS_M)

#define TIMERx_CHCTL1_CH3COMCEN    (1 << 15)
#define TIMERx_CHCTL1_CH3COMCTL_M  0x7u
#define TIMERx_CHCTL1_CH3COMCTL(x) (((x) & TIMERx_CHCTL1_CH3COMCTL_M) << 12)
#define TIMERx_CHCTL1_CH3COMSEN    (1 << 11)
#define TIMERx_CHCTL1_CH3COMFEN    (1 << 10)
#define TIMERx_CHCTL1_CH3MS_M      0x3u
#define TIMERx_CHCTL1_CH3MS(x)     (((x) & TIMERx_CHCTL1_CH3MS_M) << 8)
#define TIMERx_CHCTL1_CH2COMCEN    (1 << 7)
#define TIMERx_CHCTL1_CH2COMCTL_M  0x7u
#define TIMERx_CHCTL1_CH2COMCTL(x) (((x) & TIMERx_CHCTL1_CH2COMCTL_M) << 4)
#define TIMERx_CHCTL1_CH2COMSEN    (1 << 3)
#define TIMERx_CHCTL1_CH2COMFEN    (1 << 2)
#define TIMERx_CHCTL1_CH2MS_M      0x3u
#define TIMERx_CHCTL1_CH2MS(x)     ((x) & TIMERx_CHCTL1_CH2MS_M)

/* capture mode */
#define TIMERx_CHCTL0_CH1CAPFLT_M  0xfu
#define TIMERx_CHCTL0_CH1CAPFLT(x) (((x) & TIMERx_CHCTL0_CH1CAPFLT_M) << 12)
#define TIMERx_CHCTL0_CH1APPSC_M   0x3u
#define TIMERx_CHCTL0_CH1APPSC(x)  (((x) & TIMERx_CHCTL0_CH1APPSC_M) << 10)
#define TIMERx_CHCTL0_CH0CAPFLT_M  0xfu
#define TIMERx_CHCTL0_CH0CAPFLT(x) (((x) & TIMERx_CHCTL0_CH0CAPFLT_M) << 4)
#define TIMERx_CHCTL0_CH0APPSC_M   0x3u
#define TIMERx_CHCTL0_CH0APPSC(x)  (((x) & TIMERx_CHCTL0_CH0APPSC_M) << 2)

#define TIMERx_CHCTL1_CH3CAPFLT_M  0xfu
#define TIMERx_CHCTL1_CH3CAPFLT(x) (((x) & TIMERx_CHCTL3_CH1CAPFLT_M) << 12)
#define TIMERx_CHCTL1_CH3APPSC_M   0x3u
#define TIMERx_CHCTL1_CH3APPSC(x)  (((x) & TIMERx_CHCTL3_CH1APPSC_M) << 10)
#define TIMERx_CHCTL1_CH2CAPFLT_M  0xfu
#define TIMERx_CHCTL1_CH2CAPFLT(x) (((x) & TIMERx_CHCTL2_CH0CAPFLT_M) << 4)
#define TIMERx_CHCTL1_CH2APPSC_M   0x3u
#define TIMERx_CHCTL1_CH2APPSC(x)  (((x) & TIMERx_CHCTL2_CH0APPSC_M) << 2)

#define TIMERx_CHCTL2_CH3P  (1 << 13)
#define TIMERx_CHCTL2_CH3EN (1 << 12)
#define TIMERx_CHCTL2_CH2P  (1 << 9)
#define TIMERx_CHCTL2_CH2EN (1 << 8)
#define TIMERx_CHCTL2_CH1P  (1 << 5)
#define TIMERx_CHCTL2_CH1EN (1 << 4)
#define TIMERx_CHCTL2_CH0P  (1 << 1)
#define TIMERx_CHCTL2_CH0EN (1 << 0)

#define TIMERx_PSC_PSC_M  0xffffu
#define TIMERx_PSC_PSC(x) ((x) & TIMERx_PSC_PSC_M)

#define TIMERx_CAR_CARL_M  0xffffu
#define TIMERx_CAR_CARL(x) ((x) & TIMERx_CAR_CARL_M)

#define TIMERx_CHxCV_CHxVAL_M  0xffffu
#define TIMERx_CHxCV_CHxVAL(x) ((x) & TIMERx_CHxCV_CHxVAL_M)

int pwm_gd32vf103_init(struct pwm_gd32vf103 *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct PWM_GD32VF103_TIMER*)base;
    ctx->clkid = clkid;

    /* turn off */
    ctx->base->TIMERx_CTL0 &= ~TIMERx_CTL0_CEN;

    return 0;
}

static int set_frequency(struct pwm_gd32vf103 *ctx, unsigned long frequency)
{
    if (!picoRTOS_assert(frequency > 0)) return -EINVAL;

    unsigned long psc;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    psc = (unsigned long)freq / frequency;
    if (!picoRTOS_assert(psc <= (unsigned long)TIMERx_PSC_PSC_M))
        return -EINVAL;

    ctx->base->TIMERx_PSC = (uint32_t)TIMERx_PSC_PSC(psc);
    ctx->frequency = frequency;

    return 0;
}

int pwm_gd32vf103_setup(struct pwm_gd32vf103 *ctx, struct pwm_gd32vf103_settings *settings)
{
    if (!picoRTOS_assert(settings->waveform < PWM_GD32VF103_WAVEFORM_COUNT)) return -EINVAL;

    int res;

    if ((res = set_frequency(ctx, settings->frequency)) < 0)
        return res;

    /* waveform */
    ctx->base->TIMERx_CTL0 &= ~TIMERx_CTL0_CAM(TIMERx_CTL0_CAM_M);

    switch (settings->waveform) {
    case PWM_GD32VF103_WAVEFORM_EAPWM: ctx->base->TIMERx_CTL0 |= TIMERx_CTL0_DIR; break;
    case PWM_GD32VF103_WAVEFORM_CAPWM: ctx->base->TIMERx_CTL0 |= TIMERx_CTL0_CAM(1); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

int pwm_gd32vf103_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_gd32vf103 *parent, size_t channel)
{
    if (!picoRTOS_assert(channel < (size_t)PWM_GD32VF103_MAX_CH_COUNT)) return -EINVAL;

    ctx->parent = parent;
    ctx->channel = channel;

    switch (ctx->channel) {
    case 0:
        parent->base->TIMERx_CHCTL0 &= ~TIMERx_CHCTL0_CH0COMCTL(TIMERx_CHCTL0_CH0COMCTL_M);
        parent->base->TIMERx_CHCTL0 |= TIMERx_CHCTL0_CH0COMCTL(PWM_MODE0);
        parent->base->TIMERx_CHCTL0 |= TIMERx_CHCTL0_CH0COMSEN;
        break;

    case 1:
        parent->base->TIMERx_CHCTL0 &= ~TIMERx_CHCTL0_CH1COMCTL(TIMERx_CHCTL0_CH1COMCTL_M);
        parent->base->TIMERx_CHCTL0 |= TIMERx_CHCTL0_CH1COMCTL(PWM_MODE0);
        parent->base->TIMERx_CHCTL0 |= TIMERx_CHCTL0_CH0COMSEN;
        break;

    case 2:
        parent->base->TIMERx_CHCTL1 &= ~TIMERx_CHCTL1_CH2COMCTL(TIMERx_CHCTL1_CH2COMCTL_M);
        parent->base->TIMERx_CHCTL1 |= TIMERx_CHCTL1_CH2COMCTL(PWM_MODE0);
        parent->base->TIMERx_CHCTL1 |= TIMERx_CHCTL1_CH2COMSEN;
        break;

    case 3:
        parent->base->TIMERx_CHCTL1 &= ~TIMERx_CHCTL1_CH3COMCTL(TIMERx_CHCTL1_CH3COMCTL_M);
        parent->base->TIMERx_CHCTL1 |= TIMERx_CHCTL1_CH3COMCTL(PWM_MODE0);
        parent->base->TIMERx_CHCTL1 |= TIMERx_CHCTL1_CH3COMSEN;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

int pwm_gd32vf103_pwm_setup(struct pwm *ctx, struct pwm_gd32vf103_pwm_settings *settings)
{
    if (!picoRTOS_assert(settings->mode < PWM_GD32VF103_PWM_MODE_COUNT)) return -EINVAL;

    int chxcomctl = PWM_MODE0;
    struct pwm_gd32vf103 *parent = ctx->parent;

    if (settings->mode == PWM_GD32VF103_PWM_MODE_1)
        chxcomctl = PWM_MODE1;

    switch (ctx->channel) {
    case 0:
        parent->base->TIMERx_CHCTL0 &= ~TIMERx_CHCTL0_CH0COMCTL(TIMERx_CHCTL0_CH0COMCTL_M);
        parent->base->TIMERx_CHCTL0 |= TIMERx_CHCTL0_CH0COMCTL(chxcomctl);
        break;

    case 1:
        parent->base->TIMERx_CHCTL0 &= ~TIMERx_CHCTL0_CH1COMCTL(TIMERx_CHCTL0_CH1COMCTL_M);
        parent->base->TIMERx_CHCTL0 |= TIMERx_CHCTL0_CH1COMCTL(chxcomctl);
        break;

    case 2:
        parent->base->TIMERx_CHCTL1 &= ~TIMERx_CHCTL1_CH2COMCTL(TIMERx_CHCTL1_CH2COMCTL_M);
        parent->base->TIMERx_CHCTL1 |= TIMERx_CHCTL1_CH2COMCTL(chxcomctl);
        break;

    case 3:
        parent->base->TIMERx_CHCTL1 &= ~TIMERx_CHCTL1_CH3COMCTL(TIMERx_CHCTL1_CH3COMCTL_M);
        parent->base->TIMERx_CHCTL1 |= TIMERx_CHCTL1_CH3COMCTL(chxcomctl);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
    if (!picoRTOS_assert(period > 0)) return -EINVAL;

    /* gd32vf103 can only set the parent's period, beware */
    struct pwm_gd32vf103 *parent = ctx->parent;

    ctx->ncycles = (size_t)(parent->frequency / 1000000ul * (unsigned long)period);
    if (!picoRTOS_assert(ctx->ncycles <= (size_t)TIMERx_CAR_CARL_M))
        return -EINVAL;

    parent->base->TIMERx_CAR = (uint32_t)TIMERx_CAR_CARL(ctx->ncycles);
    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    struct pwm_gd32vf103 *parent = ctx->parent;
    size_t chxval = (size_t)duty_cycle * ctx->ncycles / (size_t)PWM_DUTY_CYCLE_MAX;

    switch (ctx->channel) {
    case 0: parent->base->TIMERx_CH0CV = (uint32_t)TIMERx_CHxCV_CHxVAL(chxval); break;
    case 1: parent->base->TIMERx_CH1CV = (uint32_t)TIMERx_CHxCV_CHxVAL(chxval); break;
    case 2: parent->base->TIMERx_CH2CV = (uint32_t)TIMERx_CHxCV_CHxVAL(chxval); break;
    case 3: parent->base->TIMERx_CH3CV = (uint32_t)TIMERx_CHxCV_CHxVAL(chxval); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

void pwm_start(struct pwm *ctx)
{
    struct pwm_gd32vf103 *parent = ctx->parent;

    switch (ctx->channel) {
    case 0: parent->base->TIMERx_CHCTL2 |= TIMERx_CHCTL2_CH0EN; break;
    case 1: parent->base->TIMERx_CHCTL2 |= TIMERx_CHCTL2_CH1EN; break;
    case 2: parent->base->TIMERx_CHCTL2 |= TIMERx_CHCTL2_CH2EN; break;
    case 3: parent->base->TIMERx_CHCTL2 |= TIMERx_CHCTL2_CH3EN; break;
    default: picoRTOS_break();
    }

    /* enable timer */
    parent->base->TIMERx_CTL0 |= TIMERx_CTL0_CEN;
}

void pwm_stop(struct pwm *ctx)
{
    struct pwm_gd32vf103 *parent = ctx->parent;

    switch (ctx->channel) {
    case 0: parent->base->TIMERx_CHCTL2 &= ~TIMERx_CHCTL2_CH0EN; break;
    case 1: parent->base->TIMERx_CHCTL2 &= ~TIMERx_CHCTL2_CH1EN; break;
    case 2: parent->base->TIMERx_CHCTL2 &= ~TIMERx_CHCTL2_CH2EN; break;
    case 3: parent->base->TIMERx_CHCTL2 &= ~TIMERx_CHCTL2_CH3EN; break;
    default: picoRTOS_break();
    }
}

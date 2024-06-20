#include "pwm-mc9s08ptxx_ftm.h"
#include "picoRTOS.h"

#include <stdint.h>

struct PWM_MC9S08PTXX_FTM {
    volatile uint8_t SC;
    volatile uint8_t CNTH;
    volatile uint8_t CNTL;
    volatile uint8_t MODH;
    volatile uint8_t MODL;

    struct {
        volatile uint8_t CnSC;
        volatile uint8_t CnVH;
        volatile uint8_t CnVL;
    } C[PWM_MC9S08PTXX_FTM_CHANNEL_COUNT];

    volatile uint8_t CNTINH;
    volatile uint8_t CNTINL;
    volatile uint8_t STATUS;
    volatile uint8_t MODE;
    volatile uint8_t SYNC;
    volatile uint8_t OUTINIT;
    volatile uint8_t OUTMASK;
    volatile uint8_t COMBINE0;
    volatile uint8_t COMBINE1;
    volatile uint8_t COMBINE2;
    volatile uint8_t DEADTIME;
    volatile uint8_t EXTRIG;
    volatile uint8_t POL;
    volatile uint8_t FMS;
    volatile uint8_t FILTER0;
    volatile uint8_t FILTER1;
    volatile uint8_t FLTFILTER;
    volatile uint8_t FLTCTRL;
};

#define SC_TOF     (1 << 7)
#define SC_TOIE    (1 << 6)
#define SC_CPWMS   (1 << 5)
#define SC_CLKS_M  0x3u
#define SC_CLKS(x) (((x) & SC_CLKS_M) << 3)
#define SC_PS_M    0x7u
#define SC_PS(x)   ((x) & SC_PS_M)

#define CnSC_CHF  (1 << 7)
#define CnSC_CHIE (1 << 6)
#define CnSC_MSB  (1 << 5)
#define CnSC_MSA  (1 << 4)
#define CnSC_ELSB (1 << 3)
#define CnSC_ELSA (1 << 2)

#define MODE_FAULTIE   (1 << 7)
#define MODE_FAULTM_M  0x3u
#define MODE_FAULTM(x) (((x) & MODE_FAULTM_M) << 5)
#define MODE_CAPTEST   (1 << 4)
#define MODE_PWMSYNC   (1 << 3)
#define MODE_WPDIS     (1 << 2)
#define MODE_INIT      (1 << 1)
#define MODE_FTMEN     (1 << 0)

#define SYNC_SWSYNC  (1 << 7)
#define SYNC_TRIG2   (1 << 6)
#define SYNC_TRIG1   (1 << 5)
#define SYNC_TRIG0   (1 << 4)
#define SYNC_SYNCHOM (1 << 3)
#define SYNC_REINIT  (1 << 2)
#define SYNC_CNTMAX  (1 << 1)
#define SYNC_CNTMIN  (1 << 0)

#define COMBINEn_FAULTEN (1 << 6)
#define COMBINEn_SYNCEN  (1 << 5)
#define COMBINEn_DTEN    (1 << 4)
#define COMBINEn_DECAP   (1 << 3)
#define COMBINEn_DECAPEN (1 << 2)
#define COMBINEn_COMP    (1 << 1)
#define COMBINEn_COMBINE (1 << 0)

int pwm_mc9s08ptxx_ftm_init(struct pwm_mc9s08ptxx_ftm *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct PWM_MC9S08PTXX_FTM*)base;
    ctx->freq = clock_get_freq(clkid) / (clock_freq_t)2; /* FIXME: FTMEN */

    picoRTOS_assert(ctx->freq > 0, return -EINVAL);

    /* disable all outputs */
    ctx->base->OUTMASK = (uint8_t)-1;
    ctx->base->SC &= ~SC_CLKS(SC_CLKS_M);

    return 0;
}

int pwm_mc9s08ptxx_ftm_setup(struct pwm_mc9s08ptxx_ftm *ctx,
                             struct pwm_mc9s08ptxx_ftm_settings *settings)
{
    picoRTOS_assert(settings->cpwms < PWM_MC9S08PTXX_FTM_CPWMS_COUNT, return -EINVAL);
    picoRTOS_assert(settings->clks < PWM_MC9S08PTXX_FTM_CLKS_COUNT, return -EINVAL);

    switch (settings->cpwms) {
    case PWM_MC9S08PTXX_FTM_CPWMS_UP:
        ctx->base->SC &= ~SC_CPWMS;
        break;

    case PWM_MC9S08PTXX_FTM_CPWMS_UPDOWN:
        ctx->base->SC |= SC_CPWMS;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    ctx->clks = settings->clks;
    return 0;
}

/* PWM */

int pwm_mc9s08ptxx_ftm_pwm_init(struct pwm *ctx, struct pwm_mc9s08ptxx_ftm *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)PWM_MC9S08PTXX_FTM_CHANNEL_COUNT, return -EINVAL);

    ctx->parent = parent;
    ctx->channel = channel;

    return 0;
}

int pwm_mc9s08ptxx_ftm_pwm_setup(struct pwm *ctx, struct pwm_mc9s08ptxx_ftm_pwm_settings *settings)
{
    picoRTOS_assert(settings->mode < PWM_MC9S08PTXX_FTM_PWM_MODE_COUNT, return -EINVAL);

    struct pwm_mc9s08ptxx_ftm *parent = ctx->parent;

    switch (settings->mode) {
    case PWM_MC9S08PTXX_FTM_PWM_MODE_HIGH_TRUE:
        parent->base->MODE |= MODE_WPDIS;
        parent->base->C[ctx->channel].CnSC &= ~(CnSC_MSA | CnSC_ELSA);
        parent->base->C[ctx->channel].CnSC |= (CnSC_MSB | CnSC_ELSB);
        parent->base->OUTINIT |= (1 << ctx->channel);
        break;

    case PWM_MC9S08PTXX_FTM_PWM_MODE_LOW_TRUE:
        parent->base->MODE |= MODE_WPDIS;
        parent->base->C[ctx->channel].CnSC &= ~(CnSC_MSA | CnSC_ELSB);
        parent->base->C[ctx->channel].CnSC |= (CnSC_MSB | CnSC_ELSA);
        parent->base->OUTINIT &= ~(1 << ctx->channel);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    return 0;
}

/* hooks */

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
#define MOD_MAX 65535

    uint8_t sc;
    uint8_t ps = 0;
    struct pwm_mc9s08ptxx_ftm *parent = ctx->parent;

    unsigned long mod = (unsigned long)period *
                        ((unsigned long)parent->freq / 1000000ul);

    /* prescaler */
    while (mod > (unsigned long)MOD_MAX) {
        mod >>= 1;
        ps++;
    }

    /* turn off */
    sc = parent->base->SC;
    parent->base->SC &= ~SC_CLKS(SC_CLKS_M);

    /* MOD */
    parent->base->MODH = (uint8_t)(mod >> 8);
    parent->base->MODL = (uint8_t)mod;
    /* Force MOD update */
    parent->base->CNTH = (uint8_t)-1;
    parent->base->CNTL = (uint8_t)-1;
    /* PS */
    parent->base->SC &= ~SC_PS(SC_PS_M);
    parent->base->SC |= SC_PS(ps);

    /* turn back on */
    parent->base->SC = sc;

    ctx->ncycles = mod;
    picoRTOS_assert(ctx->ncycles > 0, return -EINVAL);

    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    struct pwm_mc9s08ptxx_ftm *parent = ctx->parent;
    uint16_t value = (uint16_t)(((unsigned long)duty_cycle * ctx->ncycles) /
                                (unsigned long)PWM_DUTY_CYCLE_MAX);

    parent->base->C[ctx->channel].CnVH = (uint8_t)(value >> 8);
    parent->base->C[ctx->channel].CnVL = (uint8_t)value;

    /* force sync */
    parent->base->SYNC |= SYNC_SWSYNC;
    return 0;
}

void pwm_start(struct pwm *ctx)
{
    struct pwm_mc9s08ptxx_ftm *parent = ctx->parent;

    parent->base->OUTMASK &= ~(1 << ctx->channel);
    /* start FTM if not already done */
    parent->base->SC &= ~SC_CLKS(SC_CLKS_M);
    parent->base->SC |= SC_CLKS(parent->clks);
}

void pwm_stop(struct pwm *ctx)
{
    struct pwm_mc9s08ptxx_ftm *parent = ctx->parent;

    parent->base->OUTMASK |= (1 << ctx->channel);
}

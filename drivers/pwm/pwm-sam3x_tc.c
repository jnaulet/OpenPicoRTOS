#include "pwm-sam3x_tc.h"
#include "picoRTOS.h"

#include <stdint.h>

struct PWM_SAM3X_TC {
    volatile uint32_t TC_CCR;
    volatile uint32_t TC_CMR;
    volatile uint32_t TC_SMMR;
    uint32_t RESERVED0;
    volatile uint32_t TC_CV;
    volatile uint32_t TC_RA;
    volatile uint32_t TC_RB;
    volatile uint32_t TC_RC;
    volatile uint32_t TC_SR;
    volatile uint32_t TC_IER;
    volatile uint32_t TC_IDR;
    volatile uint32_t TC_IMR;
    uint32_t RESERVED1[4];
};

#define TC_CCR_SWTRG  (1 << 2)
#define TC_CCR_CLKDIS (1 << 1)
#define TC_CCR_CLKEN  (1 << 0)

/* Capture Mode */
#define TC_CMR_LDRB_M     0x3u
#define TC_CMR_LDRB(x)    (((x) & TC_CMR_LDRB_M) << 18)
#define TC_CMR_LDRA_M     0x3u
#define TC_CMR_LDRA(x)    (((x) & TC_CMR_LDRB_M) << 16)
#define TC_CMR_CPCTRG     (1 << 14)
#define TC_CMR_ABETRG     (1 << 10)
#define TC_CMR_ETRGEDG_M  0x3u
#define TC_CMR_ETRGEDG(x) (((x) & TC_CMR_ETRGEDG_M) << 8)
#define TC_CMR_LDBDIS     (1 << 7)
#define TC_CMR_LDBSTOP    (1 << 6)
/* Waveform Mode */
#define TC_CMR_BSWTRG_M   0x3u
#define TC_CMR_BSWTRG(x)  (((x) & TC_CMR_BSWTRG_M) << 30)
#define TC_CMR_BEEVT_M    0x3u
#define TC_CMR_BEEVT(x)   (((x) & TC_CMR_BEEVT_M) << 28)
#define TC_CMR_BCPC_M     0x3u
#define TC_CMR_BCPC(x)    (((x) & TC_CMR_BCPC_M) << 26)
#define TC_CMR_BCPB_M     0x3u
#define TC_CMR_BCPB(x)    (((x) & TC_CMR_BCPB_M) << 24)
#define TC_CMR_ASWTRG_M   0x3u
#define TC_CMR_ASWTRG(x)  (((x) & TC_CMR_ASWTRG_M) << 22)
#define TC_CMR_AEEVT_M    0x3u
#define TC_CMR_AEEVT(x)   (((x) & TC_CMR_AEEVT_M) << 20)
#define TC_CMR_ACPC_M     0x3u
#define TC_CMR_ACPC(x)    (((x) & TC_CMR_ACPC_M) << 18)
#define TC_CMR_ACPA_M     0x3u
#define TC_CMR_ACPA(x)    (((x) & TC_CMR_ACPA_M) << 16)
#define TC_CMR_WAVSEL_M   0x3u
#define TC_CMR_WAVSEL(x)  (((x) & TC_CMR_WAVSEL_M) << 13)
#define TC_CMR_ENETRG     (1 << 12)
#define TC_CMR_EEVT_M     0x3u
#define TC_CMR_EEVT(x)    (((x) & TC_CMR_EEVT_M) << 10)
#define TC_CMR_EEVTEDG_M  0x3u
#define TC_CMR_EEVTEDG(x) (((x) & TC_CMR_EEVTEDG_M) << 8)
#define TC_CMR_CPCDIS     (1 << 7)
#define TC_CMR_CPCSTOP    (1 << 6)
/* Common */
#define TC_CMR_WAVE       (1 << 15)
#define TC_CMR_BURST_M    0x3u
#define TC_CMR_BURST(x)   (((x) & TC_CMR_BURST_M) << 4)
#define TC_CMR_CLKI       (1 << 3)
#define TC_CMR_TCCLKS_M   0x7u
#define TC_CMR_TCCLKS(x)  ((x) & TC_CMR_TCCLKS_M)

#define TC_SMMR_DOWN (1 << 1)
#define TC_SMMR_GCEN (1 << 0)

#define TC_SR_MTIOB  (1 << 18)
#define TC_SR_MTIOA  (1 << 17)
#define TC_SR_CLKSTA (1 << 16)
#define TC_SR_ETRGS  (1 << 7)
#define TC_SR_LDRBS  (1 << 6)
#define TC_SR_LDRAS  (1 << 5)
#define TC_SR_CPCS   (1 << 4)
#define TC_SR_CPBS   (1 << 3)
#define TC_SR_CPAS   (1 << 2)
#define TC_SR_LOVRS  (1 << 1)
#define TC_SR_COVFS  (1 << 0)

#define TC_BCR_SYNC (1 << 0)

#define TC_BMR_MAXFILT_M  0x3fu
#define TC_BMR_MAXFILT(x) (((x) & TC_BMR_MAXFILT_M) << 20)
#define TC_BMR_IDXPHB     (1 << 17)
#define TC_BMR_SWAP       (1 << 16)
#define TC_BMR_INVIDX     (1 << 15)
#define TC_BMR_INVB       (1 << 14)
#define TC_BMR_INVA       (1 << 13)
#define TC_BMR_EDGPHA     (1 << 12)
#define TC_BMR_QDTRANS    (1 << 11)
#define TC_BMR_SPEEDEN    (1 << 10)
#define TC_BMR_POSEN      (1 << 9)
#define TC_BMR_QDEN       (1 << 8)
#define TC_BMR_TCnXCnS_M  0x3u
#define TC_BMR_TC2XC2S(x) (((x) & TC_BMR_TCnXCnS_M) << 4)
#define TC_BMR_TC1XC1S(x) (((x) & TC_BMR_TCnXCnS_M) << 4)
#define TC_BMR_TC0XC0S(x) ((x) & TC_BMR_TCnXCnS_M)

#define TC_FMR_ENCF1 (1 << 1)
#define TC_FMR_ENCF0 (1 << 0)

static clock_freq_t pwm_sam3x_tc_get_freq(clock_id_t clkid,
                                          pwm_sam3x_tc_tcclks_t tcclks)
{
    picoRTOS_assert(tcclks < PWM_SAM3X_TC_TCCLKS_COUNT,
                    return (clock_freq_t)-EINVAL);

    clock_freq_t freq = clock_get_freq(clkid);

    picoRTOS_assert(freq > 0, return (clock_freq_t)-EIO);

    switch (tcclks) {
    case PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK1: return freq / (clock_freq_t)2;
    case PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK2: return freq / (clock_freq_t)8;
    case PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK3: return freq / (clock_freq_t)32;
    case PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK4: return freq / (clock_freq_t)128;
    case PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK5: return freq; /* if tc->clkid is SLCK */
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-ENOSYS;
}

int pwm_sam3x_tc_init(struct pwm *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct PWM_SAM3X_TC*)base;
    ctx->clkid = clkid;
    ctx->tio = PWM_SAM3X_TC_TIOA;
    ctx->ncycles = 0;

    /* freq */
    ctx->freq = pwm_sam3x_tc_get_freq(ctx->clkid, PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK1);
    picoRTOS_assert(ctx->freq > 0, return -EIO);

    /* waveform mode */
    ctx->base->TC_CMR |= TC_CMR_WAVE;

    return 0;
}

int pwm_sam3x_tc_setup(struct pwm *ctx, const struct pwm_sam3x_tc_settings *settings)
{
    picoRTOS_assert(settings->tio < PWM_SAM3X_TC_TIO_COUNT, return -EINVAL);
    picoRTOS_assert(settings->wavsel < PWM_SAM3X_TC_WAVSEL_COUNT, return -EINVAL);
    picoRTOS_assert(settings->ncpn < PWM_SAM3X_TC_CP_COUNT, return -EINVAL);
    picoRTOS_assert(settings->ncpc < PWM_SAM3X_TC_CP_COUNT, return -EINVAL);

    ctx->freq = pwm_sam3x_tc_get_freq(ctx->clkid, settings->tcclks);
    picoRTOS_assert(ctx->freq > 0, return -EIO);

    ctx->base->TC_CMR &= ~TC_CMR_TCCLKS(TC_CMR_TCCLKS_M);
    ctx->base->TC_CMR |= TC_CMR_TCCLKS(settings->tcclks);
    ctx->base->TC_CMR &= ~TC_CMR_WAVSEL(TC_CMR_WAVSEL_M);
    ctx->base->TC_CMR |= TC_CMR_WAVSEL(settings->wavsel);

    if (settings->tio == PWM_SAM3X_TC_TIOA) {
        ctx->base->TC_CMR &= ~TC_CMR_ACPA(TC_CMR_ACPA_M);
        ctx->base->TC_CMR &= ~TC_CMR_ACPC(TC_CMR_ACPC_M);
        ctx->base->TC_CMR |= TC_CMR_ACPA(settings->ncpn);
        ctx->base->TC_CMR |= TC_CMR_ACPC(settings->ncpc);
    }else{
        ctx->base->TC_CMR &= ~TC_CMR_BCPB(TC_CMR_BCPB_M);
        ctx->base->TC_CMR &= ~TC_CMR_BCPC(TC_CMR_BCPC_M);
        ctx->base->TC_CMR |= TC_CMR_BCPB(settings->ncpn);
        ctx->base->TC_CMR |= TC_CMR_BCPC(settings->ncpc);
        /* ensure TIOB is an output */
        ctx->base->TC_CMR |= TC_CMR_EEVT(TC_CMR_EEVT_M);
    }

    ctx->tio = settings->tio;
    return 0;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
    ctx->ncycles = ((size_t)ctx->freq / (size_t)1000000) * (size_t)period;
    ctx->base->TC_RC = (uint32_t)ctx->ncycles;

    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    size_t rn = ((size_t)duty_cycle * ctx->ncycles) >> 16;

    if (ctx->tio == PWM_SAM3X_TC_TIOA) ctx->base->TC_RA = (uint32_t)rn;
    else ctx->base->TC_RB = (uint32_t)rn;

    return 0;
}

void pwm_start(struct pwm *ctx)
{
    ctx->base->TC_CCR = (uint32_t)(TC_CCR_SWTRG | TC_CCR_CLKEN);
}

void pwm_stop(struct pwm *ctx)
{
    ctx->base->TC_CCR = (uint32_t)TC_CCR_CLKDIS;
}

/* ipwm */

int ipwm_sam3x_tc_init(struct ipwm *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct PWM_SAM3X_TC*)base;
    ctx->clkid = clkid;
    ctx->state = IPWM_SAM3X_TC_STATE_IDLE;
    ctx->ldr = IPWM_SAM3X_TC_LDR_NONE;
    ctx->tcclks = PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK1;

    /* freq */
    ctx->freq = pwm_sam3x_tc_get_freq(ctx->clkid, ctx->tcclks);
    picoRTOS_assert(ctx->freq > 0, return -EIO);

    /* capture mode */
    ctx->base->TC_CMR &= ~TC_CMR_WAVE;

    return 0;
}

int ipwm_sam3x_tc_setup(struct ipwm *ctx, const struct ipwm_sam3x_tc_settings *settings)
{
    picoRTOS_assert(settings->tcclks < PWM_SAM3X_TC_TCCLKS_COUNT, return -EINVAL);
    picoRTOS_assert(settings->ldr < IPWM_SAM3X_TC_LDR_COUNT, return -EINVAL);

    ctx->freq = pwm_sam3x_tc_get_freq(ctx->clkid, settings->tcclks);
    picoRTOS_assert(ctx->freq > 0, return -EIO);

    ctx->base->TC_CMR &= ~TC_CMR_TCCLKS(TC_CMR_TCCLKS_M);
    ctx->base->TC_CMR |= TC_CMR_TCCLKS(settings->tcclks);

    ctx->tcclks = settings->tcclks;
    ctx->ldr = settings->ldr;
    return 0;
}

static int ipwm_get_period_idle(struct ipwm *ctx)
{
    ctx->base->TC_CMR = (uint32_t)(TC_CMR_TCCLKS(ctx->tcclks) |
                                   TC_CMR_LDBDIS |
                                   TC_CMR_LDRA(ctx->ldr) |
                                   TC_CMR_LDRB(ctx->ldr));

    ctx->base->TC_CCR = (uint32_t)(TC_CCR_SWTRG | TC_CCR_CLKEN);

    ctx->state = IPWM_SAM3X_TC_STATE_ACQ;
    return -EAGAIN;
}

static int ipwm_get_period_acq(struct ipwm *ctx, pwm_period_us_t *period)
{
    /* clear on read */
    if ((ctx->base->TC_SR & TC_SR_LDRBS) == 0)
        return -EAGAIN;

    /* compute result */
    int ncycles = (int)(ctx->base->TC_RB - ctx->base->TC_RA);

    *period = (pwm_period_us_t)(ncycles / ((int)ctx->freq / 1000000));
    ctx->state = IPWM_SAM3X_TC_STATE_IDLE;
    return 0;
}

int ipwm_get_period(struct ipwm *ctx, pwm_period_us_t *period)
{
    switch (ctx->state) {
    case IPWM_SAM3X_TC_STATE_IDLE: return ipwm_get_period_idle(ctx);
    case IPWM_SAM3X_TC_STATE_ACQ: return ipwm_get_period_acq(ctx, period);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int ipwm_get_duty_cycle_idle(struct ipwm *ctx)
{
    ctx->base->TC_CMR = (uint32_t)(TC_CMR_TCCLKS(ctx->tcclks) |
                                   TC_CMR_LDBDIS |
                                   TC_CMR_ETRGEDG(ctx->ldr) |
                                   TC_CMR_ABETRG | TC_CMR_CPCTRG |
                                   TC_CMR_LDRA(0x3 & ~ctx->ldr) |
                                   TC_CMR_LDRB(ctx->ldr));
    /* set timeout to 1 tick */
    ctx->base->TC_RC = (uint32_t)((int)ctx->freq / CONFIG_TICK_HZ);
    ctx->base->TC_CCR = (uint32_t)(TC_CCR_SWTRG | TC_CCR_CLKEN);

    ctx->state = IPWM_SAM3X_TC_STATE_ACQ;
    return -EAGAIN;
}

static int ipwm_get_duty_cycle_acq(struct ipwm *ctx, pwm_duty_cycle_t *duty_cycle)
{
    /* clear on read */
    uint32_t tc_sr = ctx->base->TC_SR;

    /* 0% & 100% duty cycle management */
    if ((tc_sr & TC_SR_CPCS) != 0) {
        if ((tc_sr & TC_SR_MTIOA) != 0 && ctx->ldr == IPWM_SAM3X_TC_LDR_RISING)
            *duty_cycle = (pwm_duty_cycle_t)PWM_DUTY_CYCLE_MAX;
        if ((tc_sr & TC_SR_MTIOA) == 0 && ctx->ldr == IPWM_SAM3X_TC_LDR_RISING)
            *duty_cycle = (pwm_duty_cycle_t)PWM_DUTY_CYCLE_MIN;
        if ((tc_sr & TC_SR_MTIOA) != 0 && ctx->ldr == IPWM_SAM3X_TC_LDR_FALLING)
            *duty_cycle = (pwm_duty_cycle_t)PWM_DUTY_CYCLE_MIN;
        if ((tc_sr & TC_SR_MTIOA) == 0 && ctx->ldr == IPWM_SAM3X_TC_LDR_FALLING)
            *duty_cycle = (pwm_duty_cycle_t)PWM_DUTY_CYCLE_MAX;
        /* out */
        ctx->state = IPWM_SAM3X_TC_STATE_IDLE;
        return 0;
    }

    if ((tc_sr & TC_SR_LDRAS) == 0)
        return -EAGAIN;

    /* compute result */
    int max = (int)ctx->base->TC_RB;
    int ncycles = (int)ctx->base->TC_RA;

    *duty_cycle = (pwm_duty_cycle_t)(ncycles * (PWM_DUTY_CYCLE_MAX / max));
    ctx->state = IPWM_SAM3X_TC_STATE_IDLE;
    return 0;
}

int ipwm_get_duty_cycle(struct ipwm *ctx, pwm_duty_cycle_t *duty_cycle)
{
    switch (ctx->state) {
    case IPWM_SAM3X_TC_STATE_IDLE: return ipwm_get_duty_cycle_idle(ctx);
    case IPWM_SAM3X_TC_STATE_ACQ: return ipwm_get_duty_cycle_acq(ctx, duty_cycle);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

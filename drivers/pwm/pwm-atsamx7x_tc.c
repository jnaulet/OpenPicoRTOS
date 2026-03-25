#include "pwm-atsamx7x_tc.h"
#include "picoRTOS.h"

#include <stdint.h>

struct PWM_ATSAMX7X_TC {
    struct PWM_ATSAMX7X_TC_CH {
        volatile uint32_t TC_CCRx;
        volatile uint32_t TC_CMRx;
        volatile uint32_t TC_SMMRx;
        volatile uint32_t TC_RABx;
        volatile uint32_t TC_CVx;
        volatile uint32_t TC_RAx;
        volatile uint32_t TC_RBx;
        volatile uint32_t TC_RCx;
        volatile uint32_t TC_SRx;
        volatile uint32_t TC_IERx;
        volatile uint32_t TC_IDRx;
        volatile uint32_t TC_IMRx;
        volatile uint32_t TC_EMRx;
        uint32_t RESERVED0[3];
    } CH[PWM_ATSAMX7X_TC_CH_COUNT];
    volatile uint32_t TC_BCR;
    volatile uint32_t TC_BMR;
    volatile uint32_t TC_QIER;
    volatile uint32_t TC_QIDR;
    volatile uint32_t TC_QIMR;
    volatile uint32_t TC_QISR;
    volatile uint32_t TC_FMR;
    uint32_t RESERVED1[2];
    volatile uint32_t TC_WPMR;
};

#define TC_CCRx_SWTRG  (1 << 2)
#define TC_CCRx_CLKDIS (1 << 1)
#define TC_CCRx_CLKEN  (1 << 0)

/* Capture Mode */
#define TC_CMRx_LDRB_M     0x3u
#define TC_CMRx_LDRB(x)    (((x) & TC_CMRx_LDRB_M) << 18)
#define TC_CMRx_LDRA_M     0x3u
#define TC_CMRx_LDRA(x)    (((x) & TC_CMRx_LDRA_M) << 16)
#define TC_CMRx_CPCTRG     (1 << 14)
#define TC_CMRx_ABETRG     (1 << 10)
#define TC_CMRx_ETRGEDG_M  0x3u
#define TC_CMRx_ETRGEDG(x) (((x) & TC_CMRx_ETRGEDG_M) << 8)
#define TC_CMRx_LDBDIS     (1 << 7)
#define TC_CMRx_LDBSTOP    (1 << 6)
/* Waveform Mode */
#define TC_CMRx_BSWTRG_M   0x3u
#define TC_CMRx_BSWTRG(x)  (((x) & TC_CMRx_BSWTRG_M) << 30)
#define TC_CMRx_BEEVT_M    0x3u
#define TC_CMRx_BEEVT(x)   (((x) & TC_CMRx_BEEVT_M) << 28)
#define TC_CMRx_BCPC_M     0x3u
#define TC_CMRx_BCPC(x)    (((x) & TC_CMRx_BCPC_M) << 26)
#define TC_CMRx_BCPB_M     0x3u
#define TC_CMRx_BCPB(x)    (((x) & TC_CMRx_BCPB_M) << 24)
#define TC_CMRx_ASWTRG_M   0x3u
#define TC_CMRx_ASWTRG(x)  (((x) & TC_CMRx_ASWTRG_M) << 22)
#define TC_CMRx_AEEVT_M    0x3u
#define TC_CMRx_AEEVT(x)   (((x) & TC_CMRx_AEEVT_M) << 20)
#define TC_CMRx_ACPC_M     0x3u
#define TC_CMRx_ACPC(x)    (((x) & TC_CMRx_ACPC_M) << 18)
#define TC_CMRx_ACPA_M     0x3u
#define TC_CMRx_ACPA(x)    (((x) & TC_CMRx_ACPA_M) << 16)
#define TC_CMRx_WAVSEL_M   0x3u
#define TC_CMRx_WAVSEL(x)  (((x) & TC_CMRx_WAVSEL_M) << 13)
#define TC_CMRx_ENETRG     (1 << 12)
#define TC_CMRx_EEVT_M     0x3u
#define TC_CMRx_EEVT(x)    (((x) & TC_CMRx_EEVT_M) << 10)
#define TC_CMRx_EEVTEDG_M  0x3u
#define TC_CMRx_EEVTEDG(x) (((x) & TC_CMRx_EEVTEDG_M) << 8)
#define TC_CMRx_CPCDIS     (1 << 7)
#define TC_CMRx_CPCSTOP    (1 << 6)
/* Common */
#define TC_CMRx_WAVE       (1 << 15)
#define TC_CMRx_BURST_M    0x3u
#define TC_CMRx_BURST(x)   (((x) & TC_CMRx_BURST_M) << 4)
#define TC_CMRx_CLKI       (1 << 3)
#define TC_CMRx_TCCLKS_M   0x7u
#define TC_CMRx_TCCLKS(x)  ((x) & TC_CMRx_TCCLKS_M)

#define TC_SMMRx_DOWN (1 << 1)
#define TC_SMMRx_GCEN (1 << 0)

#define TC_SRx_MTIOB  (1 << 18)
#define TC_SRx_MTIOA  (1 << 17)
#define TC_SRx_CLKSTA (1 << 16)
#define TC_SRx_ETRGS  (1 << 7)
#define TC_SRx_LDRBS  (1 << 6)
#define TC_SRx_LDRAS  (1 << 5)
#define TC_SRx_CPCS   (1 << 4)
#define TC_SRx_CPBS   (1 << 3)
#define TC_SRx_CPAS   (1 << 2)
#define TC_SRx_LOVRS  (1 << 1)
#define TC_SRx_COVFS  (1 << 0)

#define TC_BCRx_SYNC (1 << 0)

#define TC_BMRx_MAXFILT_M  0x3fu
#define TC_BMRx_MAXFILT(x) (((x) & TC_BMRx_MAXFILT_M) << 20)
#define TC_BMRx_IDXPHB     (1 << 17)
#define TC_BMRx_SWAP       (1 << 16)
#define TC_BMRx_INVIDX     (1 << 15)
#define TC_BMRx_INVB       (1 << 14)
#define TC_BMRx_INVA       (1 << 13)
#define TC_BMRx_EDGPHA     (1 << 12)
#define TC_BMRx_QDTRANS    (1 << 11)
#define TC_BMRx_SPEEDEN    (1 << 10)
#define TC_BMRx_POSEN      (1 << 9)
#define TC_BMRx_QDEN       (1 << 8)
#define TC_BMRx_TCnXCnS_M  0x3u
#define TC_BMRx_TC2XC2S(x) (((x) & TC_BMRx_TCnXCnS_M) << 4)
#define TC_BMRx_TC1XC1S(x) (((x) & TC_BMRx_TCnXCnS_M) << 4)
#define TC_BMRx_TC0XC0S(x) ((x) & TC_BMRx_TCnXCnS_M)

#define TC_FMRx_ENCF1 (1 << 1)
#define TC_FMRx_ENCF0 (1 << 0)

#define TC_WPMR_WPKEY_M  0xffffffu
#define TC_WPMR_WPKEY(x) (((x) & TC_WPMR_WPKEY_M) << 8)
#define TC_WPMR_WPEN     (1 << 0)

/* Function: pwm_atsamx7x_tc_init
 * Initializes a PWM TC block
 *
 * Parameters:
 *  ctx - The PWM block to init
 *  base - The PWM block base address
 *  clkid - The clock id associated with the PWM block
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_atsamx7x_tc_init(struct pwm_atsamx7x_tc *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct PWM_ATSAMX7X_TC*)base; // NOLINT
    ctx->clkid = clkid;

    return 0;
}

static clock_freq_t parent_get_freq(const struct pwm_atsamx7x_tc *ctx,
                                    pwm_atsamx7x_tc_tcclks_t tcclks)
{
    picoRTOS_assert(tcclks < PWM_ATSAMX7X_TC_TCCLKS_COUNT,
                    return (clock_freq_t)-EINVAL);

    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return (clock_freq_t)-EIO);

    switch (tcclks) {
    case PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK1: return freq / (clock_freq_t)2;
    case PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK2: return freq / (clock_freq_t)8;
    case PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK3: return freq / (clock_freq_t)32;
    case PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK4: return freq / (clock_freq_t)128;
    case PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK5: return freq; /* if tc->clkid is SLCK */
    default: break;
    }

    picoRTOS_assert_void(false);
    /*@notreached@*/
    return (clock_freq_t)-ENOSYS;
}

/* Function: pwm_atsamx7x_tc_pwm_init
 * Initializes a PWM TC output/channel
 *
 * Parameters:
 *  ctx - The PWM output to init
 *  parent - The parent PWM block
 *  channel - The output channel
 *  tio - The channel TIOA/B
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_atsamx7x_tc_pwm_init(struct pwm *ctx, struct pwm_atsamx7x_tc *parent,
                             size_t channel, pwm_atsamx7x_tc_tio_t tio)
{
    ctx->parent = parent;
    ctx->base = &parent->base->CH[channel];
    ctx->tio = tio;
    ctx->channel = channel;
    ctx->ncycles = 0;
    /* default clock */
    ctx->freq = parent_get_freq(ctx->parent, PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK1);
    picoRTOS_assert(ctx->freq > 0, return -EIO);

    ctx->x_cp_x = PWM_ATSAMX7X_TC_CP_NONE;
    ctx->x_cp_c = PWM_ATSAMX7X_TC_CP_NONE;

    /* waveform mode */
    ctx->base->TC_CMRx |= TC_CMRx_WAVE;

    return 0;
}

/* Function: pwm_atsamx7x_tc_pwm_setup
 * Configures a PWM block
 *
 * Parameters:
 *  ctx - The PWM channel to configure
 *  settings - The configuration settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_atsamx7x_tc_pwm_setup(struct pwm *ctx, const struct pwm_atsamx7x_tc_pwm_settings *settings)
{
    picoRTOS_assert(settings->wavsel < PWM_ATSAMX7X_TC_WAVSEL_COUNT, return -EINVAL);
    picoRTOS_assert(settings->x_cp_x < PWM_ATSAMX7X_TC_CP_COUNT, return -EINVAL);
    picoRTOS_assert(settings->x_cp_c < PWM_ATSAMX7X_TC_CP_COUNT, return -EINVAL);

    ctx->freq = parent_get_freq(ctx->parent, settings->tcclks);
    picoRTOS_assert(ctx->freq > 0, return -EIO);

    ctx->base->TC_CMRx &= ~TC_CMRx_TCCLKS(TC_CMRx_TCCLKS_M);
    ctx->base->TC_CMRx |= TC_CMRx_TCCLKS(settings->tcclks);
    ctx->base->TC_CMRx &= ~TC_CMRx_WAVSEL(TC_CMRx_WAVSEL_M);
    ctx->base->TC_CMRx |= TC_CMRx_WAVSEL(settings->wavsel);

    if (ctx->tio == PWM_ATSAMX7X_TC_TIOA) {
        ctx->base->TC_CMRx &= ~TC_CMRx_ACPA(TC_CMRx_ACPA_M);
        ctx->base->TC_CMRx &= ~TC_CMRx_ACPC(TC_CMRx_ACPC_M);
        ctx->base->TC_CMRx |= TC_CMRx_ACPA(settings->x_cp_x);
        ctx->base->TC_CMRx |= TC_CMRx_ACPC(settings->x_cp_c);
    }else{
        ctx->base->TC_CMRx &= ~TC_CMRx_BCPB(TC_CMRx_BCPB_M);
        ctx->base->TC_CMRx &= ~TC_CMRx_BCPC(TC_CMRx_BCPC_M);
        ctx->base->TC_CMRx |= TC_CMRx_BCPB(settings->x_cp_x);
        ctx->base->TC_CMRx |= TC_CMRx_BCPC(settings->x_cp_c);
        /* ensure TIOB is an output */
        ctx->base->TC_CMRx |= TC_CMRx_EEVT(TC_CMRx_EEVT_M);
    }

    ctx->x_cp_x = settings->x_cp_x;
    ctx->x_cp_c = settings->x_cp_c;

    return 0;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
    ctx->ncycles = ((size_t)ctx->freq / (size_t)1000000) * (size_t)period;
    ctx->base->TC_RCx = (uint32_t)ctx->ncycles;

    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    unsigned tc_cmrx;
    size_t rn = (((size_t)duty_cycle * ctx->ncycles) >> 16) + 1;

    if (ctx->tio == PWM_ATSAMX7X_TC_TIOA) {
        ctx->base->TC_RAx = (uint32_t)rn;
        tc_cmrx = ctx->base->TC_CMRx & ~TC_CMRx_ACPC(TC_CMRx_ACPC_M);
        if (duty_cycle == 0) tc_cmrx |= TC_CMRx_ACPC(ctx->x_cp_x);
        else tc_cmrx |= TC_CMRx_ACPC(ctx->x_cp_c);

    }else{
        ctx->base->TC_RBx = (uint32_t)rn;
        tc_cmrx = ctx->base->TC_CMRx & ~TC_CMRx_BCPC(TC_CMRx_BCPC_M);
        if (duty_cycle == 0) tc_cmrx |= TC_CMRx_BCPC(ctx->x_cp_x);
        else tc_cmrx |= TC_CMRx_BCPC(ctx->x_cp_c);
    }

    ctx->base->TC_CMRx = (uint32_t)tc_cmrx;
    return 0;
}

void pwm_start(struct pwm *ctx)
{
    ctx->base->TC_CCRx = (uint32_t)(TC_CCRx_SWTRG | TC_CCRx_CLKEN);
}

void pwm_stop(struct pwm *ctx)
{
    ctx->base->TC_CCRx = (uint32_t)TC_CCRx_CLKDIS;
}

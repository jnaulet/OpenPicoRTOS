#include "pwm-avr.h"
#include "picoRTOS.h"

struct PWM_AVR {
    volatile uint8_t TCCRnA;
    volatile uint8_t TCCRnB;
    volatile uint8_t TCCRnC;
    volatile uint8_t RESERVED;
    volatile uint16_t TCNTn;
    volatile uint16_t ICRn;
    volatile uint16_t OCRnA;
    volatile uint16_t OCRnB;
    volatile uint16_t OCRnC;
};

#define TCCRnA_COMn_M   0x3u
#define TCCRnA_COMnA(x) (((x) & TCCRnA_COMn_M) << 6)
#define TCCRnA_COMnB(x) (((x) & TCCRnA_COMn_M) << 4)
#define TCCRnA_COMnC(x) (((x) & TCCRnA_COMn_M) << 2)
#define TCCRnA_WGMnL_M  0x3u
#define TCCRnA_WGMnL(x) ((x) & TCCRnA_WGMnL_M)

#define TCCRnB_ICNCn    (1 << 7)
#define TCCRnB_ICESn    (1 << 6)
#define TCCRnB_WGMnH_M  0x3u
#define TCCRnB_WGMnH(x) (((x) & TCCRnB_WGMnH_M) << 3)
#define TCCRnB_CSn_M    0x7u
#define TCCRnB_CSn(x)   ((x) & TCCRnB_CSn_M)

#define TCCRnC_FOCnA (1 << 7)
#define TCCRnC_FOCnB (1 << 6)
#define TCCRnC_FOCnC (1 << 5)

/* Function: pwm_avr_init
 * Initializes a PWM block (TC)
 *
 * Parameters:
 *  ctx - The PWM block / TC to init
 *  base - The base address of the 16bit TC
 *  clkid - The clock id associated with the block (doesn't really matter on AVR chips)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_avr_init(struct pwm_avr *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct PWM_AVR*)base;
    ctx->freq = clock_get_freq(clkid);
    ctx->prescale = (uint32_t)1;
    ctx->waveform = (pwm_avr_waveform_t)0;
    ctx->cs = PWM_AVR_CS_NO_CLOCK;
    ctx->ncycles = 0;

    if (!picoRTOS_assert(ctx->freq > 0))
        return (int)ctx->freq;

    /* zero counter */
    ctx->base->TCNTn = (uint16_t)0;

    /* outputs */
    ctx->base->TCCRnA &= ~TCCRnA_COMnA(TCCRnA_COMn_M);
    ctx->base->TCCRnA &= ~TCCRnA_COMnB(TCCRnA_COMn_M);
    ctx->base->TCCRnA &= ~TCCRnA_COMnC(TCCRnA_COMn_M);

    return 0;
}

static int set_clock_select(struct pwm_avr *ctx, pwm_avr_cs_t cs)
{
    if (!picoRTOS_assert(cs < PWM_AVR_CS_COUNT)) return -EINVAL;

    ctx->base->TCCRnB &= ~TCCRnB_CSn(TCCRnB_CSn_M); /* stop */
    ctx->cs = cs;

    switch (cs) {
    case PWM_AVR_CS_CLKIO_1: ctx->prescale = (uint32_t)1; break;
    case PWM_AVR_CS_CLKIO_8: ctx->prescale = (uint32_t)8; break;
    case PWM_AVR_CS_CLKIO_64: ctx->prescale = (uint32_t)64; break;
    case PWM_AVR_CS_CLKIO_1024: ctx->prescale = (uint32_t)1024; break;
    default: break;
    }

    return 0;
}

static int set_waveform(struct pwm_avr *ctx, pwm_avr_waveform_t wf)
{
    if (!picoRTOS_assert(wf < PWM_AVR_WAVEFORM_COUNT)) return -EINVAL;

    /* zero-out WGM */
    ctx->base->TCCRnA &= ~TCCRnA_WGMnL(TCCRnA_WGMnL_M);
    ctx->base->TCCRnB &= ~TCCRnB_WGMnH(TCCRnB_WGMnH_M);

    switch (wf) {
    case PWM_AVR_WAVEFORM_FAST_PWM_8BIT:            /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_PHASE_CORRECT_8BIT:
        ctx->ncycles = (uint16_t)0xff;
        break;

    case PWM_AVR_WAVEFORM_FAST_PWM_9BIT:            /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_PHASE_CORRECT_9BIT:
        ctx->ncycles = (uint16_t)0x1ff;
        break;

    case PWM_AVR_WAVEFORM_FAST_PWM_10BIT:           /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_PHASE_CORRECT_10BIT:
        ctx->ncycles = (uint16_t)0x3ff;
        break;

    case PWM_AVR_WAVEFORM_PHASE_FREQ_CORRECT_ICRn:  /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_PHASE_FREQ_CORRECT_OCRnA: /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_PHASE_CORRECT_ICRn:       /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_PHASE_CORRECT_OCRnA:      /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_FAST_PWM_ICRn:            /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_FAST_PWM_OCRnA:
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EINVAL;
    }

    /* configure WGMn */
    ctx->base->TCCRnA |= TCCRnA_WGMnL(wf);
    ctx->base->TCCRnB |= TCCRnB_WGMnH(wf >> 2);

    ctx->waveform = wf;
    return 0;
}

/* Function: pwm_avr_setup
 * Configures a PWM block
 *
 * Parameters:
 *  ctx - The PWM to configure
 *  settings - The configuration settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_avr_setup(struct pwm_avr *ctx, struct pwm_avr_settings *settings)
{
    int res;

    /* clock select */
    if ((res = set_clock_select(ctx, settings->cs)) < 0)
        return res;

    /* waveform */
    if ((res = set_waveform(ctx, settings->waveform)) < 0)
        return res;

    return 0;
}

static int set_OCnA_mode(struct pwm_avr *ctx, pwm_avr_mode_t mode)
{
    if (!picoRTOS_assert(mode < PWM_AVR_MODE_COUNT)) return -EINVAL;

    if (mode == PWM_AVR_MODE_TOGGLE &&
        ctx->waveform != PWM_AVR_WAVEFORM_PHASE_FREQ_CORRECT_OCRnA &&
        ctx->waveform != PWM_AVR_WAVEFORM_PHASE_CORRECT_OCRnA &&
        ctx->waveform != PWM_AVR_WAVEFORM_FAST_PWM_ICRn &&
        ctx->waveform != PWM_AVR_WAVEFORM_FAST_PWM_OCRnA) {
        /* non-authorised values */
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    ctx->base->TCCRnA &= ~TCCRnA_COMnA(TCCRnA_COMn_M);
    ctx->base->TCCRnA |= TCCRnA_COMnA(mode);

    return 0;
}

static int set_OCnB_mode(struct pwm_avr *ctx, pwm_avr_mode_t mode)
{
    if (!picoRTOS_assert(mode < PWM_AVR_MODE_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(mode != PWM_AVR_MODE_TOGGLE)) return -EINVAL;

    ctx->base->TCCRnA &= ~TCCRnA_COMnB(TCCRnA_COMn_M);
    ctx->base->TCCRnA |= TCCRnA_COMnB(mode);

    return 0;
}

static int set_OCnC_mode(struct pwm_avr *ctx, pwm_avr_mode_t mode)
{
    if (!picoRTOS_assert(mode < PWM_AVR_MODE_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(mode != PWM_AVR_MODE_TOGGLE)) return -EINVAL;

    ctx->base->TCCRnA &= ~TCCRnA_COMnC(TCCRnA_COMn_M);
    ctx->base->TCCRnA |= TCCRnA_COMnC(mode);

    return 0;
}

static int set_output_compare_mode(struct pwm_avr *ctx,
                                   pwm_avr_oc_t oc,
                                   pwm_avr_mode_t mode)
{
    if (!picoRTOS_assert(oc < PWM_AVR_OC_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(mode < PWM_AVR_MODE_COUNT)) return -EINVAL;

    switch (oc) {
    case PWM_AVR_OCnA: return set_OCnA_mode(ctx, mode);
    case PWM_AVR_OCnB: return set_OCnB_mode(ctx, mode);
    case PWM_AVR_OCnC: return set_OCnC_mode(ctx, mode);
    default: break;
    }

    picoRTOS_break();
    /*@notreached */
    return -EINVAL;
}

static void start(struct pwm_avr *ctx)
{
    ctx->base->TCCRnB &= ~TCCRnB_CSn(TCCRnB_CSn_M);
    ctx->base->TCCRnB |= TCCRnB_CSn(ctx->cs);
}

static void stop(struct pwm_avr *ctx)
{
    ctx->base->TCCRnB &= ~TCCRnB_CSn(TCCRnB_CSn_M);
}

/* Function: pwm_avr_pwm_init
 * Initializes a PWM output
 *
 * Parameters:
 *  ctx - The PWM output to init
 *  parent - The parent PWM block / TC
 *  oc - The output compare / pin the PWM will outptu from
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_avr_pwm_init(struct pwm *ctx, struct pwm_avr *parent, pwm_avr_oc_t oc)
{
    if (!picoRTOS_assert(oc < PWM_AVR_OC_COUNT)) return -EINVAL;

    ctx->parent = parent;
    ctx->oc = oc;

    return 0;
}

/* Function: pwm_avr_pwm_setup
 * Configures a PWM output
 *
 * Parameters:
 *  ctx - The PWM output to configure
 *  settings - The configuration settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_avr_pwm_setup(struct pwm *ctx, struct pwm_settings *settings)
{
    int res;

    /* output compare */
    if ((res = set_output_compare_mode(ctx->parent, ctx->oc, settings->mode)) < 0)
        return res;

    return 0;
}

/* runtime */

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
    if (!picoRTOS_assert(period > (size_t)0)) return -EINVAL;

    struct pwm_avr *parent = ctx->parent;
    uint32_t hz = (uint32_t)parent->freq / parent->prescale;

    parent->ncycles = (uint16_t)((hz / (uint32_t)1000000ul) * (uint32_t)period);

    if (!picoRTOS_assert(parent->ncycles > 0)) return -EINVAL;

    switch (parent->waveform) {
    case PWM_AVR_WAVEFORM_PHASE_FREQ_CORRECT_ICRn:  /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_PHASE_CORRECT_ICRn:       /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_FAST_PWM_ICRn:
        parent->base->ICRn = parent->ncycles;
        break;

    case PWM_AVR_WAVEFORM_PHASE_FREQ_CORRECT_OCRnA: /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_PHASE_CORRECT_OCRnA:      /*@fallthrough@*/
    case PWM_AVR_WAVEFORM_FAST_PWM_OCRnA:
        parent->base->OCRnA = parent->ncycles;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    struct pwm_avr *parent = ctx->parent;
    uint16_t ncycles = parent->ncycles;
    uint16_t OCRn = (uint16_t)(((uint32_t)duty_cycle * (uint32_t)ncycles) >> 16);

    switch (ctx->oc) {
    case PWM_AVR_OCnA: parent->base->OCRnA = OCRn; break;
    case PWM_AVR_OCnB: parent->base->OCRnB = OCRn; break;
    case PWM_AVR_OCnC: parent->base->OCRnC = OCRn; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return 0;
}

void pwm_start(struct pwm *ctx)
{
    start(ctx->parent);
}

void pwm_stop(struct pwm *ctx)
{
    stop(ctx->parent);
}

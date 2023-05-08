#include "pwm-rp2040.h"
#include "picoRTOS.h"

#include <stdint.h>

struct PWM_RP2040 {
    struct PWM_RP2040_CH {
        volatile uint32_t CSR;
        volatile uint32_t DIV;
        volatile uint32_t CTR;
        volatile uint32_t CC;
        volatile uint32_t TOP;
    } CH[PWM_RP2040_PWM_CHANNEL_COUNT];
    volatile uint32_t EN;
    volatile uint32_t INTR;
    volatile uint32_t INTE;
    volatile uint32_t INTF;
    volatile uint32_t INTS;
};

#define CHn_CSR_EN         (1 << 0)
#define CHn_CSR_DIVMODE_M  0x3u
#define CHn_CSR_DIVMODE(x) (((x) & CHn_CSR_DIVMODE_M) << 4)

#define CHn_DIV_INT_M   0xffu
#define CHn_DIV_INT(x)  (((x) & CHn_DIV_INT_M) << 4)
#define CHn_DIV_FRAC_M  0xfu
#define CHn_DIV_FRAC(x) ((x) & CHn_DIV_FRAC_M)

#define CHn_CC_B_M  0xffffu
#define CHn_CC_B(x) (((x) & CHn_CC_B_M) << 16)
#define CHn_CC_A_M  0xffffu
#define CHn_CC_A(x) ((x) & CHn_CC_A_M)

/* Function: pwm_rp2040_init
 * Initializes a PWM block
 *
 * Parameters:
 *  ctx - The PWM block to init
 *  base - The PWM block base address
 *  clkid - The clock id associated with that block
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_rp2040_init(struct pwm_rp2040 *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct PWM_RP2040*)base;
    ctx->freq = clock_get_freq(clkid);

    if (!picoRTOS_assert(ctx->freq > 0))
        return (int)ctx->freq;

    return 0;
}

/* Function: pwm_rp2040_pwm_init
 * Initializes a PWM output
 *
 * Parameters:
 *  ctx - The PWM output to init
 *  parent - The parent PWM block
 *  channel - The pwm channel selected
 *  output - The output pin (A/B)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_rp2040_pwm_init(struct pwm *ctx, struct pwm_rp2040 *parent,
                        size_t channel, pwm_rp2040_pwm_output_t output)
{
    if (!picoRTOS_assert(channel < (size_t)PWM_RP2040_PWM_CHANNEL_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(output < PWM_RP2040_PWM_OUTPUT_COUNT)) return -EINVAL;

    ctx->parent = parent;
    ctx->ch = &parent->base->CH[channel];
    ctx->output = output;
    ctx->div = (size_t)1;
    ctx->ncycles = 0;

    /* init */
    ctx->ch->CSR = (uint32_t)0;
    ctx->ch->CTR = (uint32_t)0;

    return 0;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
#define PWM_RP2040_PWM_TOP_MAX 0xffff

    if (!picoRTOS_assert(period > 0)) return -EINVAL;

    /* real RP2040 channel frequency */
    size_t hz = (size_t)ctx->parent->freq / ctx->div;

    ctx->ncycles = (hz / (size_t)1000000) * (size_t)period;

    if (ctx->ncycles > (size_t)PWM_RP2040_PWM_TOP_MAX) {
        ctx->div = (ctx->ncycles / (size_t)PWM_RP2040_PWM_TOP_MAX) + 1;
        ctx->ncycles = ctx->ncycles / ctx->div;
    }

    if (!picoRTOS_assert(ctx->ncycles > 0)) return -EINVAL;

    ctx->ch->DIV = (uint32_t)CHn_DIV_INT(ctx->div);
    ctx->ch->TOP = (uint32_t)ctx->ncycles;

    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    size_t cc = ((size_t)duty_cycle * ctx->ncycles) >> 16;

    if (ctx->output == PWM_RP2040_PWM_OUTPUT_A) {
        ctx->ch->CC &= ~CHn_CC_A(CHn_CC_A_M);
        ctx->ch->CC |= CHn_CC_A(cc);
    }else{
        ctx->ch->CC &= ~CHn_CC_B(CHn_CC_B_M);
        ctx->ch->CC |= CHn_CC_B(cc);
    }

    return 0;
}

void pwm_start(struct pwm *ctx)
{
    ctx->ch->CSR |= CHn_CSR_EN;
}

void pwm_stop(struct pwm *ctx)
{
    ctx->ch->CSR &= ~CHn_CSR_EN;
}

#define IPWM_DIVMODE_FREE    0x0
#define IPWM_DIVMODE_GATED   0x1
#define IPWM_DIVMODE_RISING  0x2
#define IPWM_DIVMODE_FALLING 0x3

/* Function: pwm_rp2040_ipwm_init
 * Initializes an input PWM
 *
 * Parameters:
 *  ctx - The IPWM to init
 *  parent - The parent PWM block
 *  channel - The selected channel
 *
 * Returns:
 * 0 if success, -errno otherwise
 *
 * Remarks:
 * On RP2040, only pin B can act as an input
 */
int pwm_rp2040_ipwm_init(struct ipwm *ctx, struct pwm_rp2040 *parent, size_t channel)
{
    if (!picoRTOS_assert(channel < (size_t)PWM_RP2040_PWM_CHANNEL_COUNT)) return -EINVAL;

    ctx->parent = parent;
    ctx->ch = &parent->base->CH[channel];
    ctx->state = PWM_RP2040_IPWM_STATE_IDLE;
    ctx->div = (size_t)1;
    ctx->ref = 0;
    ctx->polarity = true;

    return 0;
}

/* Function: pwm_rp2040_ipwm_setup
 * Configures an IPWM
 *
 * Parameters:
 *  ctx - The IPWM to configure
 *  setttings - The configuration settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_rp2040_ipwm_setup(struct ipwm *ctx, struct pwm_rp2040_ipwm_settings *settings)
{
    if (!picoRTOS_assert(settings->div > 0)) return -EINVAL;

    ctx->div = settings->div;
    ctx->ch->DIV = (uint32_t)CHn_DIV_INT(ctx->div);

    ctx->polarity = settings->polarity;
    return 0;
}

static int ipwm_get_period_idle(struct ipwm *ctx)
{
    /* stop */
    ctx->ch->CSR &= ~CHn_CSR_EN;
    /* set divmode */
    ctx->ch->CSR &= ~CHn_CSR_DIVMODE(CHn_CSR_DIVMODE_M);
    if (ctx->polarity) ctx->ch->CSR |= CHn_CSR_DIVMODE(IPWM_DIVMODE_RISING);
    else ctx->ch->CSR |= CHn_CSR_DIVMODE(IPWM_DIVMODE_FALLING);
    /* reset counters */
    ctx->ch->CTR = (uint32_t)0;
    ctx->ref = picoRTOS_get_tick();
    /* start */
    ctx->ch->CSR |= CHn_CSR_EN;

    ctx->state = PWM_RP2040_IPWM_STATE_ACQ;
    return -EAGAIN;
}

static int ipwm_get_period_acq(struct ipwm *ctx, pwm_period_us_t *period)
{
    /* FIXME: beware of rollover */
    picoRTOS_tick_t elapsed = picoRTOS_get_tick() - ctx->ref;

    if (elapsed != 0) {
        size_t ncycles = (size_t)ctx->ch->CTR * ctx->div;
        /* TODO: threshold ? */
        if (ncycles < (size_t)2)
            return -EAGAIN;

        /* stop */
        ctx->ch->CSR &= ~CHn_CSR_EN;

        /* compute result */
        int elapsed_usec = (int)elapsed * (1000000 / CONFIG_TICK_HZ);
        *period = (pwm_period_us_t)((size_t)elapsed_usec / ncycles);

        ctx->state = PWM_RP2040_IPWM_STATE_IDLE;
        return 0;
    }

    return -EAGAIN;
}

int ipwm_get_period(struct ipwm *ctx, pwm_period_us_t *period)
{
    switch (ctx->state) {
    case PWM_RP2040_IPWM_STATE_IDLE: return ipwm_get_period_idle(ctx);
    case PWM_RP2040_IPWM_STATE_ACQ: return ipwm_get_period_acq(ctx, period);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int ipwm_get_duty_cycle_idle(struct ipwm *ctx)
{
    /* stop */
    ctx->ch->CSR &= ~CHn_CSR_EN;
    /* set divmode */
    ctx->ch->CSR &= ~CHn_CSR_DIVMODE(CHn_CSR_DIVMODE_M);
    ctx->ch->CSR |= CHn_CSR_DIVMODE(IPWM_DIVMODE_GATED);
    /* reset counters */
    ctx->ch->CTR = (uint32_t)0;
    ctx->ref = picoRTOS_get_tick();
    /* start */
    ctx->ch->CSR |= CHn_CSR_EN;

    ctx->state = PWM_RP2040_IPWM_STATE_ACQ;
    return -EAGAIN;
}

static int ipwm_get_duty_cycle_acq(struct ipwm *ctx, pwm_duty_cycle_t *duty_cycle)
{
    /* FIXME: beware of rollover */
    picoRTOS_tick_t elapsed = picoRTOS_get_tick() - ctx->ref;

    if (elapsed != 0) {
        size_t ncycles = (size_t)ctx->ch->CTR;

        /* stop */
        ctx->ch->CSR &= ~CHn_CSR_EN;

        /* computing result... */
        size_t hz = (size_t)ctx->parent->freq / ctx->div;
        size_t elapsed_cycles = (hz * (size_t)elapsed) / (size_t)CONFIG_TICK_HZ;

        /* final result */
        *duty_cycle = (pwm_duty_cycle_t)((ncycles * (size_t)PWM_DUTY_CYCLE_MAX) / elapsed_cycles);

        /* invert polarity if needed */
        if (!ctx->polarity)
            *duty_cycle = PWM_DUTY_CYCLE_MAX - *duty_cycle;

        ctx->state = PWM_RP2040_IPWM_STATE_IDLE;
        return 0;
    }

    return -EAGAIN;
}

int ipwm_get_duty_cycle(struct ipwm *ctx, pwm_duty_cycle_t *duty_cycle)
{
    switch (ctx->state) {
    case PWM_RP2040_IPWM_STATE_IDLE: return ipwm_get_duty_cycle_idle(ctx);
    case PWM_RP2040_IPWM_STATE_ACQ: return ipwm_get_duty_cycle_acq(ctx, duty_cycle);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

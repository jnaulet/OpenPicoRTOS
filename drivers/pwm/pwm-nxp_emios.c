#include "pwm-nxp_emios.h"
#include "picoRTOS.h"

#include <stdint.h>

struct PWM_NXP_EMIOS {
    volatile uint32_t MCR;
    volatile uint32_t GFLAG;
    volatile uint32_t OUDIS;
    volatile uint32_t UCDIS;
    uint32_t RESERVED[4];
    struct PWM_NXP_EMIOS_UC {
        volatile uint32_t A;
        volatile uint32_t B;
        volatile uint32_t CNT;
        volatile uint32_t C;
        volatile uint32_t S;
        volatile uint32_t ALTA;
        uint32_t RESERVED[2];
    } UC[PWM_NXP_EMIOS_CHANNEL_COUNT];
};

#define MCR_GTBE    (1 << 28)
#define MCR_GPREN   (1 << 26)
#define MCR_GPRE_M  0xffu
#define MCR_GPRE(x) (((x) & MCR_GPRE_M) << 8)

#define C_ODIS     (1 << 30)
#define C_UCPRE_M  0x3u
#define C_UCPRE(x) (((x) & C_UCPRE_M) << 26)
#define C_UCPREN   (1 << 25)
#define C_IF_M     0xfu
#define C_IF(x)    (((x) & C_IF_M) << 19)
#define C_EDPOL    (1 << 7)
#define C_MODE_M   0x7fu
#define C_MODE(x)  ((x) & C_MODE_M)

#define S_UCIN (1 << 2)
#define S_FLAG (1 << 0)

/* Function: pwm_nxp_emios_init
 * Inits an NXP eMIOS block
 *
 * Parameters:
 *  ctx - The eMIOS block to init
 *  base - The eMIOS block base address
 *  clkid - The eMIOS input clock id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_nxp_emios_init(struct pwm_nxp_emios *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct PWM_NXP_EMIOS*)base;
    ctx->gpre = (size_t)1;
    ctx->freq = clock_get_freq(clkid);

    picoRTOS_assert(ctx->freq > 0, return (int)ctx->freq);

    /* turn general clock on */
    ctx->base->MCR = (uint32_t)MCR_GTBE;

    return 0;
}

/* Function: pwm_nxp_emios_setup
 * Configures an eMIOS block
 *
 * Parameters:
 *  ctx - The eMIOS block to configure
 *  settings - The eMIOS specific parameters
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_nxp_emios_setup(struct pwm_nxp_emios *ctx, struct pwm_nxp_emios_settings *settings)
{
    picoRTOS_assert(settings->gpre >= (size_t)PWM_NXP_EMIOS_GPRE_MIN, return -EINVAL);
    picoRTOS_assert(settings->gpre <= (size_t)PWM_NXP_EMIOS_GPRE_MAX, return -EINVAL);

    /* set prescaler */
    ctx->base->MCR |= MCR_GPREN;
    ctx->base->MCR |= MCR_GPRE(settings->gpre - 1);

    ctx->gpre = settings->gpre;
    return 0;
}

static int nxp_emios_get_UC(struct pwm_nxp_emios *ctx, size_t channel,
                            /*@out@*/ struct PWM_NXP_EMIOS_UC **uc)
{
    if (channel < (size_t)PWM_NXP_EMIOS_CHANNEL_COUNT) {
        *uc = &ctx->base->UC[channel];
        return 0;
    }

    *uc = NULL;
    picoRTOS_break();
    /*@notreached@*/ return -EINVAL;
}

/* Function: pwm_nxp_emios_pwm_init
 * Creates a PWM output from an eMIOS block
 *
 * Parameters:
 *  ctx - The pwm to init
 *  parent - The parent eMIOS block
 * channel - The channel number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_nxp_emios_pwm_init(struct pwm *ctx, struct pwm_nxp_emios *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)PWM_NXP_EMIOS_CHANNEL_COUNT, return -EINVAL);

    int res;

    ctx->parent = parent;
    ctx->ncycles = 0;
    ctx->uc = NULL;
    ctx->ucpre = (size_t)1;

    if ((res = nxp_emios_get_UC(ctx->parent, channel, &ctx->uc)) < 0)
        return res;

    ctx->uc->A = (uint32_t)0;

    ctx->uc->C = (uint32_t)C_MODE(0x58);    /* set mode to OPWMFM */
    ctx->uc->C &= ~C_EDPOL;                 /* clear on match */

    return 0;
}

/* Function: pwm_nxp_emios_pwm_setup
 * Configures a PWM output from an eMIOS block
 *
 * Parameters:
 *  ctx - The pwm to init
 *  settings - The pwm output settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_nxp_emios_pwm_setup(struct pwm *ctx, struct pwm_nxp_emios_pwm_settings *settings)
{
    picoRTOS_assert(settings->ucpre < PWM_NXP_EMIOS_UCPRE_COUNT, return -EINVAL);

    ctx->uc->C &= ~C_UCPRE(C_UCPRE_M);
    ctx->uc->C |= C_UCPRE(settings->ucpre);

    ctx->ucpre = (size_t)settings->ucpre + 1;

    if (settings->polarity) ctx->uc->C |= C_EDPOL;
    else ctx->uc->C &= ~C_EDPOL;

    return 0;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
#define PWM_NXP_EMIOS_PWM_PERIOD_COUNT 0x100000

    picoRTOS_assert(period > 0, return -EINVAL);

    /* real nxp_emios channel frequency */
    size_t hz = (size_t)ctx->parent->freq / (ctx->parent->gpre * ctx->ucpre);

    ctx->ncycles = (hz / (size_t)1000000) * (size_t)period;

    picoRTOS_assert(ctx->ncycles > 0, return -EINVAL);
    picoRTOS_assert(ctx->ncycles < (size_t)PWM_NXP_EMIOS_PWM_PERIOD_COUNT, return -EINVAL);

    ctx->uc->B = (uint32_t)ctx->ncycles;

    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    ctx->uc->A = (uint32_t)((size_t)duty_cycle * ctx->ncycles) >> 16;
    return 0;
}

void pwm_start(struct pwm *ctx)
{
    ctx->uc->C |= C_UCPREN;        /* start clock */
}

void pwm_stop(struct pwm *ctx)
{
    ctx->uc->C &= ~C_UCPREN;        /* stop clock */
}

/* IPWM */

int pwm_nxp_emios_ipwm_init(/*@out@*/ struct ipwm *ctx, struct pwm_nxp_emios *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)PWM_NXP_EMIOS_CHANNEL_COUNT, return -EINVAL);

    int res;

    ctx->parent = parent;
    ctx->ncycles = 0;
    ctx->uc = NULL;
    ctx->ucpre = (size_t)1;
    ctx->state = PWM_NXP_EMIOS_IPWM_STATE_IDLE;

    if ((res = nxp_emios_get_UC(ctx->parent, channel, &ctx->uc)) < 0)
        return res;

    ctx->uc->A = (uint32_t)0;

    ctx->uc->C = (uint32_t)C_MODE(0x4);     /* set mode to IPWM */
    ctx->uc->C |= C_EDPOL;                  /* trig on rising edge */

    return 0;
}

int pwm_nxp_emios_ipwm_setup(struct ipwm *ctx, struct pwm_nxp_emios_ipwm_settings *settings)
{
    picoRTOS_assert(settings->ucpre < PWM_NXP_EMIOS_UCPRE_COUNT, return -EINVAL);
    picoRTOS_assert(settings->filter < IPWM_NXP_EMIOS_IF_COUNT, return -EINVAL);

    ctx->uc->C &= ~C_UCPRE(C_UCPRE_M);
    ctx->uc->C |= C_UCPRE(settings->ucpre);

    ctx->ucpre = (size_t)settings->ucpre + 1;

    if (settings->polarity) ctx->uc->C |= C_EDPOL;
    else ctx->uc->C &= ~C_EDPOL;

    ctx->uc->C &= ~C_IF(C_IF_M);
    if (settings->filter > IPWM_NXP_EMIOS_IF_BYPASSED)
        ctx->uc->C |= C_IF(1 << ((size_t)settings->filter - 1));

    return 0;
}

static int ipwm_get_period_idle(struct ipwm *ctx)
{
    /* stop clock */
    ctx->uc->C &= ~C_UCPREN;

    /* set mode to IPM */
    ctx->uc->C &= ~C_MODE(C_MODE_M);
    ctx->uc->C |= C_MODE(0x5);

    /* reset counter */
    ctx->uc->CNT = (uint32_t)0;

    /* reset flag */
    ctx->uc->S |= S_FLAG;

    /* start clock */
    ctx->uc->C |= C_UCPREN;

    ctx->state = PWM_NXP_EMIOS_IPWM_STATE_ACQ;
    return -EAGAIN;
}

static int ipwm_get_period_acq(struct ipwm *ctx, pwm_period_us_t *period)
{
    struct pwm_nxp_emios *parent = ctx->parent;

    if ((ctx->uc->S & S_FLAG) == 0)
        return -EAGAIN;

    /* read A, then B */
    uint32_t A = ctx->uc->A;
    uint32_t B = ctx->uc->B;

    /* convert to us format */
    uint32_t ncycles = (uint32_t)0xffffff & (A - B);
    size_t hz = (size_t)parent->freq / (parent->gpre * ctx->ucpre);

    *period = (pwm_period_us_t)(ncycles * 1000000ul / (uint32_t)hz);

    /* stop clock */
    ctx->uc->C &= ~C_UCPREN;

    /* reset flag */
    ctx->uc->S |= S_FLAG;

    ctx->state = PWM_NXP_EMIOS_IPWM_STATE_IDLE;
    return 0;
}

int ipwm_get_period(struct ipwm *ctx, pwm_period_us_t *period)
{
    switch (ctx->state) {
    case PWM_NXP_EMIOS_IPWM_STATE_IDLE: return ipwm_get_period_idle(ctx);
    case PWM_NXP_EMIOS_IPWM_STATE_ACQ: return ipwm_get_period_acq(ctx, period);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int ipwm_get_duty_cycle_idle(struct ipwm *ctx)
{
    /* stop clock */
    ctx->uc->C &= ~C_UCPREN;

    /* set mode to WPTA */
    ctx->uc->C &= ~C_MODE(C_MODE_M);
    ctx->uc->C |= C_MODE(0x14);

    /* reset counter(s) */
    ctx->uc->CNT = (uint32_t)0;
    ctx->uc->A = (uint32_t)0;

    /* set capture duration (1 tick) */
    size_t hz = (size_t)ctx->parent->freq / (ctx->parent->gpre * ctx->ucpre);

    ctx->uc->B = (uint32_t)(hz / (size_t)CONFIG_TICK_HZ);

    /* reset flag */
    ctx->uc->S |= S_FLAG;

    /* start clock */
    ctx->uc->C |= C_UCPREN;

    ctx->state = PWM_NXP_EMIOS_IPWM_STATE_ACQ;
    return -EAGAIN;
}

static int ipwm_get_duty_cycle_acq(struct ipwm *ctx, pwm_duty_cycle_t *duty_cycle)
{
    if ((ctx->uc->S & S_FLAG) == 0)
        return -EAGAIN;

    size_t ncycles = (size_t)ctx->uc->ALTA;
    size_t period = (size_t)ctx->uc->B;

    *duty_cycle = (pwm_duty_cycle_t)((ncycles * (size_t)PWM_DUTY_CYCLE_MAX) / period);

    /* stop clock */
    ctx->uc->C &= ~C_UCPREN;

    /* reset flag */
    ctx->uc->S |= S_FLAG;

    ctx->state = PWM_NXP_EMIOS_IPWM_STATE_IDLE;
    return 0;
}

int ipwm_get_duty_cycle(struct ipwm *ctx, pwm_duty_cycle_t *duty_cycle)
{
    switch (ctx->state) {
    case PWM_NXP_EMIOS_IPWM_STATE_IDLE: return ipwm_get_duty_cycle_idle(ctx);
    case PWM_NXP_EMIOS_IPWM_STATE_ACQ: return ipwm_get_duty_cycle_acq(ctx, duty_cycle);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

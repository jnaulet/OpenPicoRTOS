#include "pwm-tinyavr_tca.h"
#include "picoRTOS.h"

#include <stdint.h>

struct PWM_TINYAVR_TCA {
    volatile uint8_t CTRLA;
    volatile uint8_t CTRLB;
    volatile uint8_t CTRLC;
    volatile uint8_t CTRLD;
    volatile uint8_t CTRLECLR;
    volatile uint8_t CTRLESET;
    volatile uint8_t CTRLFCLR;
    volatile uint8_t CTRLFSET;
    uint8_t RESERVED0;
    volatile uint8_t EVCTRL;
    volatile uint8_t INTCTRL;
    volatile uint8_t INTFLAGS;
    uint8_t RESERVED1[2];
    volatile uint8_t DBGCTRL;
    volatile uint8_t TEMP;
    uint8_t RESERVED2[16];
    volatile uint16_t CNT;
    uint8_t RESERVED3[4];
    volatile uint16_t PER;
    volatile uint16_t CMPn[PWM_TINYAVR_TCA_CMP_COUNT];
    uint8_t RESERVED4[8];
    volatile uint16_t PERBUF;
    volatile uint16_t CMPnBUF[PWM_TINYAVR_TCA_CMP_COUNT];
};

#define CTRLA_CLKSEL_M  0x7u
#define CTRLA_CLKSEL(x) (((x) & CTRLA_CLKSEL_M) << 1)
#define CTRLA_ENABLE    (1 << 0)

#define CTRLB_CMP2EN    (1 << 6)
#define CTRLB_CMP1EN    (1 << 5)
#define CTRLB_CMP0EN    (1 << 4)
#define CTRLB_ALUPD     (1 << 3)
#define CTRLB_WGMODE_M  0x7u
#define CTRLB_WGMODE(x) ((x) & CTRLB_WGMODE_M)

#define CTRLC_CMP2OV (1 << 2)
#define CTRLC_CMP1OV (1 << 1)
#define CTRLC_CMP0OV (1 << 0)

#define CTRLE_CMD_M  0x3u
#define CTRLE_CMD(x) (((x) & CTRLE_CMD_M) << 2)
#define CTRLE_LUPD   (1 << 1)
#define CTRLE_DIR    (1 << 0)

/* Function: pwm_tinyavr_tca_init
 * Initializes a PWM block (TCAx)
 *
 * Parameters:
 *  ctx - The PWM block / TCA to init
 *  base - The base address of the 16bit TCA
 *  clkid - The clock id associated with the block
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_tinyavr_tca_init(struct pwm_tinyavr_tca *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct PWM_TINYAVR_TCA*)base;
    ctx->freq = clock_get_freq(clkid);
    ctx->div = 1ul;

    picoRTOS_assert(ctx->freq > 0, return (int)ctx->freq);

    ctx->base->CTRLA = (uint8_t)CTRLA_ENABLE;
    return 0;
}

static int set_clksel(struct pwm_tinyavr_tca *ctx, pwm_tinyavr_tca_clksel_t clksel)
{
    picoRTOS_assert(clksel < PWM_TINYAVR_TCA_CLKSEL_COUNT, return -EINVAL);

    switch (clksel) {
    case PWM_TINYAVR_TCA_CLKSEL_DIV1: ctx->div = 1ul; break;
    case PWM_TINYAVR_TCA_CLKSEL_DIV2: ctx->div = 2ul; break;
    case PWM_TINYAVR_TCA_CLKSEL_DIV4: ctx->div = 4ul; break;
    case PWM_TINYAVR_TCA_CLKSEL_DIV8: ctx->div = 8ul; break;
    case PWM_TINYAVR_TCA_CLKSEL_DIV16: ctx->div = 16ul; break;
    case PWM_TINYAVR_TCA_CLKSEL_DIV64: ctx->div = 64ul; break;
    case PWM_TINYAVR_TCA_CLKSEL_DIV256: ctx->div = 256ul; break;
    case PWM_TINYAVR_TCA_CLKSEL_DIV1024: ctx->div = 1024ul; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EINVAL;
    }

    ctx->base->CTRLA &= ~CTRLA_CLKSEL(CTRLA_CLKSEL_M);
    ctx->base->CTRLA |= CTRLA_CLKSEL(clksel);

    return 0;
}

/* Function: pwm_tinyavr_tca_setup
 * Configures a PWM block
 *
 * Parameters:
 *  ctx - The PWM to configure
 *  settings - The configuration settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_tinyavr_tca_setup(struct pwm_tinyavr_tca *ctx, const struct pwm_tinyavr_tca_settings *settings)
{
    picoRTOS_assert(settings->wgmode < PWM_TINYAVR_TCA_WGMODE_COUNT, return -EINVAL);

    int res;

    if ((res = set_clksel(ctx, settings->clksel)) < 0)
        return res;

    ctx->base->CTRLB &= ~CTRLB_WGMODE(CTRLB_WGMODE_M);
    ctx->base->CTRLB |= CTRLB_WGMODE(settings->wgmode);

    return 0;
}

/* Function: pwm_tinyavr_tca_pwm_init
 * Initializes a PWM output
 *
 * Parameters:
 *  ctx - The PWM output to init
 *  parent - The parent PWM block / TCAx
 *  cmp - The pin to output PWM to
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_tinyavr_tca_pwm_init(struct pwm *ctx,
                             struct pwm_tinyavr_tca *parent,
                             pwm_tinyavr_tca_cmp_t cmp)
{
    picoRTOS_assert(cmp < PWM_TINYAVR_TCA_CMP_COUNT, return -EINVAL);

    ctx->parent = parent;
    ctx->cmp = cmp;

    return 0;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
    picoRTOS_assert(period > 0, return -EINVAL);

    struct pwm_tinyavr_tca *parent = ctx->parent;

    /* real tinyAVR channel frequency */
    unsigned long hz = (unsigned long)parent->freq / parent->div;

    ctx->ncycles = (size_t)((hz / 1000000ul) * (unsigned long)period);
    parent->base->PER = (uint16_t)ctx->ncycles;

    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    size_t index = (size_t)ctx->cmp;
    struct pwm_tinyavr_tca *parent = ctx->parent;
    unsigned long cmpn = ((unsigned long)duty_cycle * ctx->ncycles) >> 16;

    parent->base->CMPn[index] = (uint16_t)cmpn;
    return 0;
}

void pwm_start(struct pwm *ctx)
{
    struct pwm_tinyavr_tca *parent = ctx->parent;

    parent->base->CTRLESET = (uint8_t)CTRLE_CMD(0x2);

    switch (ctx->cmp) {
    case PWM_TINYAVR_TCA_CMP0: parent->base->CTRLB |= CTRLB_CMP0EN; break;
    case PWM_TINYAVR_TCA_CMP1: parent->base->CTRLB |= CTRLB_CMP1EN; break;
    case PWM_TINYAVR_TCA_CMP2: parent->base->CTRLB |= CTRLB_CMP2EN; break;
    default:
        picoRTOS_break();
    }
}

void pwm_stop(struct pwm *ctx)
{
    struct pwm_tinyavr_tca *parent = ctx->parent;

    switch (ctx->cmp) {
    case PWM_TINYAVR_TCA_CMP0: parent->base->CTRLB &= ~CTRLB_CMP0EN; break;
    case PWM_TINYAVR_TCA_CMP1: parent->base->CTRLB &= ~CTRLB_CMP1EN; break;
    case PWM_TINYAVR_TCA_CMP2: parent->base->CTRLB &= ~CTRLB_CMP2EN; break;
    default:
        picoRTOS_break();
    }
}

#include "pwm-same5x_tc.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <stdint.h>

struct PWM_SAME5X_TC {
    volatile uint32_t CTRLA;
    volatile uint8_t CTRLBCLR;
    volatile uint8_t CTRLBSET;
    volatile uint16_t EVCTRL;
    volatile uint8_t INTENCLR;
    volatile uint8_t INTENSET;
    volatile uint8_t INTFLAG;
    volatile uint8_t STATUS;
    volatile uint8_t WAVE;
    volatile uint8_t DRVCTRL;
    uint8_t RESERVED0;
    volatile uint8_t DBGCTRL;
    volatile uint32_t SYNCBUSY;
    union {
        /* 8-bit mode */
        struct {
            volatile uint8_t COUNT;
            uint8_t RESERVED1[6];
            volatile uint8_t PER;
            volatile uint8_t CC0;
            volatile uint8_t CC1;
            uint8_t RESERVED2[16];
            volatile uint8_t PERBUF;
            volatile uint8_t CCBUF0;
            volatile uint8_t CCBUF1;
        } TC8;
        /* 16-bit mode */
        struct {
            volatile uint16_t COUNT;
            uint16_t RESERVED1[3];
            volatile uint16_t CC0;
            volatile uint16_t CC1;
            uint8_t RESERVED2[8];
            volatile uint16_t CCBUF0;
            volatile uint16_t CCBUF1;
        } TC16;
        /* 32-bit mode */
        struct {
            volatile uint16_t COUNT;
            uint32_t RESERVED1;
            volatile uint32_t CC0;
            volatile uint32_t CC1;
            uint32_t RESERVED2[3];
            volatile uint32_t CCBUF0;
            volatile uint32_t CCBUF1;
        } TC32;
    };
};

#define CTRLA_CAPTMODE1_M   0x3u
#define CTRLA_CAPTMODE1(x) (((x) & CTRLA_CAPTMODE1_M) << 27)
#define CTRLA_CAPTMODE0_M   0x3u
#define CTRLA_CAPTMODE0(x) (((x) & CTRLA_CAPTMODE1_M) << 24)
#define CTRLA_COPEN1       (1 << 21)
#define CTRLA_COPEN0       (1 << 20)
#define CTRLA_CAPTEN1      (1 << 17)
#define CTRLA_CAPTEN0      (1 << 16)
#define CTRLA_ALOCK        (1 << 11)
#define CTRLA_PRESCALER_M  0x7u
#define CTRLA_PRESCALER(x) (((x) & CTRLA_PRESCALER_M) << 8)
#define CTRLA_ONDEMAND     (1 << 7)
#define CTRLA_RUNSTDBY     (1 << 6)
#define CTRLA_PRESCSYNC_M  0x3u
#define CTRLA_PRESCSYNC(x) (((x) & CTRLA_PRESCSYNC_M) << 4)
#define CTRLA_MODE_M       0x3u
#define CTRLA_MODE(x)      (((x) & CTRLA_MODE_M) << 2)
#define CTRLA_ENABLE       (1 << 1)
#define CTRLA_SWRST        (1 << 0)

#define CTRLB_CMD_M  0x7u
#define CTRLB_CMD(x) (((x) & CTRLB_CMD_M) << 5)
#define CTRLB_ONESHOT (1 << 2)
#define CTRLB_LUPD    (1 << 1)
#define CTRLB_DIR     (1 << 0)

#define WAVE_WAVEGEN_M  0x3u
#define WAVE_WAVEGEN(x) ((x) & WAVE_WAVEGEN_M)

#define DRVCTRL_INVEN1 (1 << 1)
#define DRVCTRL_INVEN0 (1 << 0)

#define SYNCBUSY_CC1    (1 << 7)
#define SYNCBUSY_CC0    (1 << 6)
#define SYNCBUSY_COUNT  (1 << 4)
#define SYNCBUSY_STATUS (1 << 3)
#define SYNCBUSY_CTRLB  (1 << 2)
#define SYNCBUSY_ENABLE (1 << 1)
#define SYNCBUSY_SWRST  (1 << 0)

static int sync_busywait(struct pwm_same5x_tc *ctx, uint32_t mask)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((ctx->base->SYNCBUSY & mask) != 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

/* Function: pwm_same5x_tc_init
 * Initializes a SAME5x TC for PWM
 *
 * Parameters:
 *  ctx - The TC/PWM to init
 *  base - The TC base address
 *  clkid - The clock id for this TC
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_same5x_tc_init(struct pwm_same5x_tc *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct PWM_SAME5X_TC*)base;
    ctx->freq = clock_get_freq(clkid);
    ctx->prescaler = 1ul;
    ctx->mode = PWM_SAME5X_TC_MODE_16;
    ctx->wavegen = PWM_SAME5X_TC_WAVEGEN_NFRQ;

    picoRTOS_assert(ctx->freq > 0, return -EIO);
    return 0;
}

static int set_prescaler(struct pwm_same5x_tc *ctx, pwm_same5x_tc_prescaler_t prescaler)
{
    picoRTOS_assert(prescaler < PWM_SAME5X_TC_PRESCALER_COUNT, return -EINVAL);

    switch (prescaler) {
    case PWM_SAME5X_TC_PRESCALER_DIV1: ctx->prescaler = 1ul; break;
    case PWM_SAME5X_TC_PRESCALER_DIV2: ctx->prescaler = 2ul; break;
    case PWM_SAME5X_TC_PRESCALER_DIV4: ctx->prescaler = 4ul; break;
    case PWM_SAME5X_TC_PRESCALER_DIV8: ctx->prescaler = 8ul; break;
    case PWM_SAME5X_TC_PRESCALER_DIV16: ctx->prescaler = 16ul; break;
    case PWM_SAME5X_TC_PRESCALER_DIV64: ctx->prescaler = 64ul; break;
    case PWM_SAME5X_TC_PRESCALER_DIV256: ctx->prescaler = 256ul; break;
    case PWM_SAME5X_TC_PRESCALER_DIV1024: ctx->prescaler = 1024ul; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    ctx->base->CTRLA &= ~CTRLA_PRESCALER(CTRLA_PRESCALER_M);
    ctx->base->CTRLA |= CTRLA_PRESCALER(prescaler);

    return 0;
}

/* Function: pwm_same5x_tc_setup
 * Configures a SAME5x TC used as PWM
 *
 * Parameters:
 *  ctx - The TC/PWM to configure
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_same5x_tc_setup(struct pwm_same5x_tc *ctx, struct pwm_same5x_tc_settings *settings)
{
    picoRTOS_assert(settings->mode < PWM_SAME5X_TC_MODE_COUNT, return -EINVAL);
    picoRTOS_assert(settings->wavegen < PWM_SAME5X_TC_WAVEGEN_COUNT, return -EINVAL);

    int res;

    if ((res = set_prescaler(ctx, settings->prescaler)) < 0)
        return res;

    /* mode */
    ctx->base->CTRLA &= ~CTRLA_MODE(CTRLA_MODE_M);
    ctx->base->CTRLA |= CTRLA_MODE(settings->mode);
    ctx->mode = settings->mode;

    /* wavegen */
    ctx->base->WAVE = (uint32_t)WAVE_WAVEGEN(settings->wavegen);
    ctx->wavegen = settings->wavegen;

    return 0;
}

static int set_invert(struct pwm_same5x_tc *ctx, size_t channel, bool invert)
{
    picoRTOS_assert(channel < (size_t)PWM_SAME5X_TC_CHANNEL_COUNT, return -EINVAL);

    if (channel == 0) {
        if (invert) ctx->base->DRVCTRL |= DRVCTRL_INVEN0;
        else ctx->base->DRVCTRL &= ~DRVCTRL_INVEN0;
    }else{
        if (invert) ctx->base->DRVCTRL |= DRVCTRL_INVEN1;
        else ctx->base->DRVCTRL &= ~DRVCTRL_INVEN1;
    }

    return 0;
}

/* Function: pwm_same5x_tc_pwm_init
 * Initializes a SAME5x PWM channel
 *
 * Parameters:
 *  ctx - The PWM channel to init
 *  parent - The parent SAME5x TC base address
 *  channel - The PWM channel number (0-1)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_same5x_tc_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_same5x_tc *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)PWM_SAME5X_TC_CHANNEL_COUNT, return -EINVAL);

    ctx->parent = parent;
    ctx->channel = channel;

    return 0;
}

/* Function: pwm_same5x_tc_pwm_setup
 * Configures a SAME5x PWM channel
 *
 * Parameters:
 *  ctx - The PWM channel to configure
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_same5x_tc_pwm_setup(struct pwm *ctx, struct pwm_same5x_tc_pwm_settings *settings)
{
    return set_invert(ctx->parent, ctx->channel, settings->invert);
}

static int set_period8(struct pwm_same5x_tc *ctx, size_t ncycles)
{
    picoRTOS_assert(ncycles < (size_t)0x100, return -EINVAL);

    ctx->base->TC8.PER = (uint8_t)ncycles;
    return 0;
}

static int set_period16(struct pwm_same5x_tc *ctx, size_t ncycles)
{
    picoRTOS_assert(ncycles < (size_t)0x10000, return -EINVAL);
    picoRTOS_assert(ctx->wavegen == PWM_SAME5X_TC_WAVEGEN_MFRQ ||
                    ctx->wavegen == PWM_SAME5X_TC_WAVEGEN_MPWM,
                    return -EINVAL);

    ctx->base->TC16.CC0 = (uint16_t)ncycles;
    return sync_busywait(ctx, (uint32_t)SYNCBUSY_CC0);
}

static int set_period32(struct pwm_same5x_tc *ctx, size_t ncycles)
{
    picoRTOS_assert(ctx->wavegen == PWM_SAME5X_TC_WAVEGEN_MFRQ ||
                    ctx->wavegen == PWM_SAME5X_TC_WAVEGEN_MPWM,
                    return -EINVAL);

    ctx->base->TC32.CC0 = (uint32_t)ncycles;
    return sync_busywait(ctx, (uint32_t)SYNCBUSY_CC0);
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
    struct pwm_same5x_tc *parent = ctx->parent;
    unsigned long freq = (unsigned long)parent->freq / parent->prescaler;

    parent->ncycles = (size_t)(freq / 1000000ul) * (size_t)period;

    switch (parent->mode) {
    case PWM_SAME5X_TC_MODE_8: return set_period8(parent, parent->ncycles);
    case PWM_SAME5X_TC_MODE_16: return set_period16(parent, parent->ncycles);
    case PWM_SAME5X_TC_MODE_32: return set_period32(parent, parent->ncycles);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int set_duty_cycle8(struct pwm *ctx, size_t ccx)
{
    picoRTOS_assert(ccx < (size_t)0x100, return -EINVAL);

    struct pwm_same5x_tc *parent = ctx->parent;

    if (ctx->channel == 0) {
        parent->base->TC8.CC0 = (uint8_t)ccx;
        return sync_busywait(parent, (uint32_t)SYNCBUSY_CC0);
    }else{
        parent->base->TC8.CC1 = (uint8_t)ccx;
        return sync_busywait(parent, (uint32_t)SYNCBUSY_CC1);
    }
}

static int set_duty_cycle16(struct pwm *ctx, size_t ccx)
{
    picoRTOS_assert(ccx < (size_t)0x10000, return -EINVAL);

    struct pwm_same5x_tc *parent = ctx->parent;

    parent->base->TC16.CC1 = (uint16_t)ccx;
    return sync_busywait(parent, (uint32_t)SYNCBUSY_CC1);
}

static int set_duty_cycle32(struct pwm *ctx, size_t ccx)
{
    struct pwm_same5x_tc *parent = ctx->parent;

    parent->base->TC32.CC1 = (uint32_t)ccx;
    return sync_busywait(parent, (uint32_t)SYNCBUSY_CC1);
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    struct pwm_same5x_tc *parent = ctx->parent;
    size_t ccx = ((size_t)duty_cycle * parent->ncycles) >> 16;

    switch (parent->mode) {
    case PWM_SAME5X_TC_MODE_8: return set_duty_cycle8(ctx, ccx);
    case PWM_SAME5X_TC_MODE_16: return set_duty_cycle16(ctx, ccx);
    case PWM_SAME5X_TC_MODE_32: return set_duty_cycle32(ctx, ccx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int cmd_read_back_as_zero(struct pwm_same5x_tc *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((ctx->base->CTRLBSET >> 5) != 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

void pwm_start(struct pwm *ctx)
{
    struct pwm_same5x_tc *parent = ctx->parent;

    parent->base->CTRLA |= CTRLA_ENABLE;
    (void)sync_busywait(parent, (uint32_t)SYNCBUSY_ENABLE);

    parent->base->CTRLBSET = (uint8_t)CTRLB_CMD(0x1);
    (void)sync_busywait(parent, (uint32_t)SYNCBUSY_CTRLB);
    (void)cmd_read_back_as_zero(parent);
}

void pwm_stop(struct pwm *ctx)
{
    struct pwm_same5x_tc *parent = ctx->parent;

    parent->base->CTRLA &= ~CTRLA_ENABLE;
    (void)sync_busywait(parent, (uint32_t)SYNCBUSY_ENABLE);

    parent->base->CTRLBSET = (uint8_t)CTRLB_CMD(0x2);
    (void)sync_busywait(parent, (uint32_t)SYNCBUSY_CTRLB);
    (void)cmd_read_back_as_zero(parent);
}

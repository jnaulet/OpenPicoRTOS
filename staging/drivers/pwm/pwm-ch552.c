#include "pwm-ch552.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_PWM_CK_SE) PWM_CK_SE;
__sfr __at(ADDR_PWM_CTRL) PWM_CTRL;
__sfr __at(ADDR_PWM_DATA1) PWM_DATA1;
__sfr __at(ADDR_PWM_DATA2) PWM_DATA2;
#else
static unsigned char PWM_CK_SE;
static unsigned char PWM_CTRL;
static unsigned char PWM_DATA1;
static unsigned char PWM_DATA2;
#endif

#define PWM_CTRL_bPWM_IE_END  (1 << 7)
#define PWM_CTRL_bPWM2_POLAR  (1 << 6)
#define PWM_CTRL_bPWM1_POLAR  (1 << 5)
#define PWM_CTRL_bPWM_IF_END  (1 << 4)
#define PWM_CTRL_bPWM2_OUT_EN (1 << 3)
#define PWM_CTRL_bPWM1_OUT_EN (1 << 2)
#define PWM_CTRL_bPWM_CLR_ALL (1 << 1)

int pwm_ch552_init(/*@out@*/ struct pwm_ch552 *ctx, clock_id_t clkid)
{
    ctx->clkid = clkid;
    PWM_CTRL &= ~PWM_CTRL_bPWM_CLR_ALL;
    return 0;
}

int pwm_ch552_pwm_init(struct pwm *ctx, struct pwm_ch552 *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)PWM_CH552_CHANNEL_COUNT, return -EINVAL);

    ctx->parent = parent;
    ctx->channel = channel;
    return 0;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
    picoRTOS_assert(period > 0, return -EINVAL);

    clock_freq_t freq = clock_get_freq(ctx->parent->clkid);

    /* beware this is contagious */
    picoRTOS_assert(freq > 0, return -EIO);
    PWM_CK_SE = (unsigned char)(((unsigned long)freq >> 8) / (1000000ul / (unsigned long)period));

    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    if (ctx->channel == 0) PWM_DATA1 = (unsigned char)(duty_cycle >> 8);
    else PWM_DATA2 = (unsigned char)(duty_cycle >> 8);

    return 0;
}

void pwm_start(struct pwm *ctx)
{
    if (ctx->channel == 0) PWM_CTRL |= PWM_CTRL_bPWM1_OUT_EN;
    else PWM_CTRL |= PWM_CTRL_bPWM2_OUT_EN;
}

void pwm_stop(struct pwm *ctx)
{
    if (ctx->channel == 0) PWM_CTRL &= ~PWM_CTRL_bPWM1_OUT_EN;
    else PWM_CTRL &= ~PWM_CTRL_bPWM2_OUT_EN;
}

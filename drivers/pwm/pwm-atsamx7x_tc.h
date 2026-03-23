#ifndef PWM_ATSAMX7X_TC_H
#define PWM_ATSAMX7X_TC_H

#include "pwm.h"
#include "clock.h"

#define PWM_ATSAMX7X_TC_CH_COUNT 3

struct PWM_ATSAMX7X_TC;
struct PWM_ATSAMX7X_TC_CH;

struct pwm_atsamx7x_tc {
    /*@temp@*/ struct PWM_ATSAMX7X_TC *base;
    clock_id_t clkid;
};

int pwm_atsamx7x_tc_init(/*@out@*/ struct pwm_atsamx7x_tc *ctx, int base, clock_id_t clkid);

typedef enum {
    PWM_ATSAMX7X_TC_TIOA,
    PWM_ATSAMX7X_TC_TIOB,
    PWM_ATSAMX7X_TC_TIO_COUNT
} pwm_atsamx7x_tc_tio_t;

typedef enum {
    PWM_ATSAMX7X_TC_CP_NONE     = 0,
    PWM_ATSAMX7X_TC_CP_SET      = 1,
    PWM_ATSAMX7X_TC_CP_CLEAR    = 2,
    PWM_ATSAMX7X_TC_CP_TOGGLE   = 3,
    PWM_ATSAMX7X_TC_CP_COUNT
} pwm_atsamx7x_tc_cp_t;

struct pwm {
    /*@temp@*/ struct pwm_atsamx7x_tc *parent;
    /*@temp@*/ struct PWM_ATSAMX7X_TC_CH *base;
    pwm_atsamx7x_tc_tio_t tio;
    size_t channel;
    size_t ncycles;
    clock_freq_t freq;
    pwm_atsamx7x_tc_cp_t x_cp_x;
    pwm_atsamx7x_tc_cp_t x_cp_c;
};

typedef enum {
    PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK1,    /* PCK6-7 */
    PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK2,    /* MCK / 8 */
    PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK3,    /* MCK / 32 */
    PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK4,    /* MCK / 128 */
    PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK5,    /* SLCK */
    PWM_ATSAMX7X_TC_TCCLKS_XC0,             /* XC0 */
    PWM_ATSAMX7X_TC_TCCLKS_XC1,             /* XC1 */
    PWM_ATSAMX7X_TC_TCCLKS_XC2,             /* XC2 */
    PWM_ATSAMX7X_TC_TCCLKS_COUNT
} pwm_atsamx7x_tc_tcclks_t;

typedef enum {
    PWM_ATSAMX7X_TC_WAVSEL_UP           = 0,
    PWM_ATSAMX7X_TC_WAVSEL_UPDOWN       = 1,
    PWM_ATSAMX7X_TC_WAVSEL_UP_RC        = 2,
    PWM_ATSAMX7X_TC_WAVSEL_UPDOWN_RC    = 3,
    PWM_ATSAMX7X_TC_WAVSEL_COUNT
} pwm_atsamx7x_tc_wavsel_t;

struct pwm_atsamx7x_tc_pwm_settings {
    pwm_atsamx7x_tc_tcclks_t tcclks;
    pwm_atsamx7x_tc_wavsel_t wavsel;
    pwm_atsamx7x_tc_cp_t x_cp_x;
    pwm_atsamx7x_tc_cp_t x_cp_c;
};

int pwm_atsamx7x_tc_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_atsamx7x_tc *parent, size_t channel, pwm_atsamx7x_tc_tio_t tio);
int pwm_atsamx7x_tc_pwm_setup(struct pwm *ctx, const struct pwm_atsamx7x_tc_pwm_settings *settings);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

#ifndef PWM_TINYAVR_TCA_H
#define PWM_TINYAVR_TCA_H

#include "pwm.h"
#include "clock.h"

struct PWM_TINYAVR_TCA;

struct pwm_tinyavr_tca {
    /*@temp@*/ struct PWM_TINYAVR_TCA *base;
    clock_freq_t freq;
    unsigned long div;
};

typedef enum {
    PWM_TINYAVR_TCA_CLKSEL_DIV1     = 0,
    PWM_TINYAVR_TCA_CLKSEL_DIV2     = 1,
    PWM_TINYAVR_TCA_CLKSEL_DIV4     = 2,
    PWM_TINYAVR_TCA_CLKSEL_DIV8     = 3,
    PWM_TINYAVR_TCA_CLKSEL_DIV16    = 4,
    PWM_TINYAVR_TCA_CLKSEL_DIV64    = 5,
    PWM_TINYAVR_TCA_CLKSEL_DIV256   = 6,
    PWM_TINYAVR_TCA_CLKSEL_DIV1024  = 7,
    PWM_TINYAVR_TCA_CLKSEL_COUNT
} pwm_tinyavr_tca_clksel_t;

typedef enum {
    PWM_TINYAVR_TCA_WGMODE_NORMAL       = 0,
    PWM_TINYAVR_TCA_WGMODE_FRQ          = 1,
    PWM_TINYAVR_TCA_WGMODE_RESERVED0    = 2,
    PWM_TINYAVR_TCA_WGMODE_SINGLESLOPE  = 3,
    PWM_TINYAVR_TCA_WGMODE_RESERVED1    = 4,
    PWM_TINYAVR_TCA_WGMODE_DSTOP        = 5,
    PWM_TINYAVR_TCA_WGMODE_DSBOTH       = 6,
    PWM_TINYAVR_TCA_WGMODE_DSBOTTOM     = 7,
    PWM_TINYAVR_TCA_WGMODE_COUNT
} pwm_tinyavr_tca_wgmode_t;

struct pwm_tinyavr_tca_settings {
    pwm_tinyavr_tca_clksel_t clksel;
    pwm_tinyavr_tca_wgmode_t wgmode;
};

int pwm_tinyavr_tca_init(/*@out@*/ struct pwm_tinyavr_tca *ctx, struct PWM_TINYAVR_TCA *base, clock_id_t clkid);
int pwm_tinyavr_tca_setup(struct pwm_tinyavr_tca *ctx, struct pwm_tinyavr_tca_settings *settings);

/* PWM channels */

typedef enum {
    PWM_TINYAVR_TCA_CMP0    = 0,
    PWM_TINYAVR_TCA_CMP1    = 1,
    PWM_TINYAVR_TCA_CMP2    = 2,
    PWM_TINYAVR_TCA_CMP_COUNT
} pwm_tinyavr_tca_cmp_t;

struct pwm {
    /*@temp@*/ struct pwm_tinyavr_tca *pwm;
    pwm_tinyavr_tca_cmp_t cmp;
    size_t ncycles;
};

int pwm_tinyavr_tca_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_tinyavr_tca *pwm, pwm_tinyavr_tca_cmp_t cmp);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

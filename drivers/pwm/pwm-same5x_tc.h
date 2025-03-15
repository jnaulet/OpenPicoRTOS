#ifndef PWM_SAME5X_TC_H
#define PWM_SAME5X_TC_H

#include "pwm.h"
// #include "ipwm.h"
#include "clock.h"
#include <stdbool.h>

struct PWM_SAME5X_TC;

typedef enum {
    PWM_SAME5X_TC_PRESCALER_DIV1    = 0,
    PWM_SAME5X_TC_PRESCALER_DIV2    = 1,
    PWM_SAME5X_TC_PRESCALER_DIV4    = 2,
    PWM_SAME5X_TC_PRESCALER_DIV8    = 3,
    PWM_SAME5X_TC_PRESCALER_DIV16   = 4,
    PWM_SAME5X_TC_PRESCALER_DIV64   = 5,
    PWM_SAME5X_TC_PRESCALER_DIV256  = 6,
    PWM_SAME5X_TC_PRESCALER_DIV1024 = 7,
    PWM_SAME5X_TC_PRESCALER_COUNT
} pwm_same5x_tc_prescaler_t;

typedef enum {
    PWM_SAME5X_TC_MODE_16   = 0,
    PWM_SAME5X_TC_MODE_8    = 1,
    PWM_SAME5X_TC_MODE_32   = 2,
    PWM_SAME5X_TC_MODE_COUNT
} pwm_same5x_tc_mode_t;

typedef enum {
    PWM_SAME5X_TC_WAVEGEN_NFRQ  = 0,
    PWM_SAME5X_TC_WAVEGEN_MFRQ  = 1,
    PWM_SAME5X_TC_WAVEGEN_NPWM  = 2,
    PWM_SAME5X_TC_WAVEGEN_MPWM  = 3,
    PWM_SAME5X_TC_WAVEGEN_COUNT
} pwm_same5x_tc_wavegen_t;

struct pwm_same5x_tc {
    /*@temp@*/ struct PWM_SAME5X_TC *base;
    clock_freq_t freq;
    unsigned long prescaler;
    pwm_same5x_tc_mode_t mode;
    pwm_same5x_tc_wavegen_t wavegen;
    size_t ncycles;
};

struct pwm_same5x_tc_settings {
    pwm_same5x_tc_prescaler_t prescaler;
    pwm_same5x_tc_mode_t mode;
    pwm_same5x_tc_wavegen_t wavegen;
};

int pwm_same5x_tc_init(/*@out@*/ struct pwm_same5x_tc *ctx, int base, clock_id_t clkid);
int pwm_same5x_tc_setup(struct pwm_same5x_tc *ctx, const struct pwm_same5x_tc_settings *settings);

/* channel */

#define PWM_SAME5X_TC_CHANNEL_COUNT 2

struct pwm {
    /*@temp@*/ struct pwm_same5x_tc *parent;
    size_t channel;
};

struct pwm_same5x_tc_pwm_settings {
    bool invert;
};

int pwm_same5x_tc_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_same5x_tc *parent, size_t channel);
int pwm_same5x_tc_pwm_setup(struct pwm *ctx, const struct pwm_same5x_tc_pwm_settings *settings);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

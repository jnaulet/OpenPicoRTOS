#ifndef PWM_MC9S08PTXX_FTM_H
#define PWM_MC9S08PTXX_FTM_H

#include "pwm.h"
#include "clock.h"

#include <stdint.h>
#include <stddef.h>

#define PWM_MC9S08PTXX_FTM_CHANNEL_COUNT 6

struct PWM_MC9S08PTXX_FTM;

typedef enum {
    PWM_MC9S08PTXX_FTM_CPWMS_UP,
    PWM_MC9S08PTXX_FTM_CPWMS_UPDOWN,
    PWM_MC9S08PTXX_FTM_CPWMS_COUNT
} pwm_mc9s08ptxx_ftm_cpwms_t;

typedef enum {
    PWM_MC9S08PTXX_FTM_CLKS_NONE    = 0,
    PWM_MC9S08PTXX_FTM_CLKS_SYS     = 1,
    PWM_MC9S08PTXX_FTM_CLKS_FIXED   = 2,
    PWM_MC9S08PTXX_FTM_CLKS_EXT     = 3,
    PWM_MC9S08PTXX_FTM_CLKS_COUNT
} pwm_mc9s08ptxx_ftm_clks_t;

struct pwm_mc9s08ptxx_ftm {
    /*@temp@*/ struct PWM_MC9S08PTXX_FTM *base;
    clock_freq_t freq;
    pwm_mc9s08ptxx_ftm_clks_t clks;
};

struct pwm_mc9s08ptxx_ftm_settings {
    pwm_mc9s08ptxx_ftm_cpwms_t cpwms; /* center-aligned PWM select */
    pwm_mc9s08ptxx_ftm_clks_t clks;
};

int pwm_mc9s08ptxx_ftm_init(/*@out@*/ struct pwm_mc9s08ptxx_ftm *ctx, int base, clock_id_t clkid);
int pwm_mc9s08ptxx_ftm_setup(struct pwm_mc9s08ptxx_ftm *ctx, const struct pwm_mc9s08ptxx_ftm_settings *settings);

struct pwm {
    /*@temp@*/ struct pwm_mc9s08ptxx_ftm *parent;
    size_t channel;
    unsigned long ncycles;
};

typedef enum {
    PWM_MC9S08PTXX_FTM_PWM_MODE_HIGH_TRUE,
    PWM_MC9S08PTXX_FTM_PWM_MODE_LOW_TRUE,
    PWM_MC9S08PTXX_FTM_PWM_MODE_COUNT
} pwm_mc9s08ptxx_ftm_pwm_mode_t;

struct pwm_mc9s08ptxx_ftm_pwm_settings {
    pwm_mc9s08ptxx_ftm_pwm_mode_t mode;
};

int pwm_mc9s08ptxx_ftm_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_mc9s08ptxx_ftm *parent, size_t channel);
int pwm_mc9s08ptxx_ftm_pwm_setup(struct pwm *ctx, const struct pwm_mc9s08ptxx_ftm_pwm_settings *settings);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

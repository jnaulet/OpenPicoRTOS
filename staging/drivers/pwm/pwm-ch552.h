#ifndef PWM_CH552_H
#define PWM_CH552_H

#include "clock.h"
#include "pwm.h"

#define PWM_CH552_CHANNEL_COUNT 2

struct pwm_ch552 {
    clock_id_t clkid;
};

int pwm_ch552_init(/*@out@*/ struct pwm_ch552 *ctx, clock_id_t clkid);

struct pwm {
    /*@temp@*/ struct pwm_ch552 *parent;
    unsigned int channel;
};

int pwm_ch552_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_ch552 *parent, size_t channel);

/*
 * Implements:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

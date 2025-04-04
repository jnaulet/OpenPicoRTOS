#ifndef PWM_N76E003_H
#define PWM_N76E003_H

#include "clock.h"
#include "pwm.h"

#define PWM_N76E003_CHANNEL_COUNT 6

struct pwm_n76e003 {
    clock_id_t clkid;
    unsigned int channel;
    unsigned long pwmp;
};

int pwm_n76e003_init(/*@out@*/ struct pwm_n76e003 *ctx, clock_id_t clkid);
int pwm_n76e003_set_period(struct pwm_n76e003 *ctx, unsigned long pwmp);

struct pwm {
    /*@temp@*/ struct pwm_n76e003 *parent;
    unsigned int channel;
};

int pwm_n76e003_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_n76e003 *parent, size_t channel);

/*
 * Implements:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

#ifndef PWM_GD32VF103_H
#define PWM_GD32VF103_H

#include "pwm.h"
#include "clock.h"

struct PWM_GD32VF103_TIMER;

#define PWM_GD32VF103_MAX_CH_COUNT 4 /* for TIMER0-4 */

struct pwm_gd32vf103 {
    /*@temp@*/ struct PWM_GD32VF103_TIMER *base;
    clock_id_t clkid;
    unsigned long frequency;
};

typedef enum {
    PWM_GD32VF103_WAVEFORM_EAPWM_UP,
    PWM_GD32VF103_WAVEFORM_EAPWM_DOWN,
    PWM_GD32VF103_WAVEFORM_CAPWM_UP,
    PWM_GD32VF103_WAVEFORM_CAPWM_DOWN,
    PWM_GD32VF103_WAVEFORM_CAPWM_UP_DOWN,
    PWM_GD32VF103_WAVEFORM_COUNT
} pwm_gd32vf103_waveform_t;

struct pwm_gd32vf103_settings {
    unsigned long frequency;
    pwm_gd32vf103_waveform_t waveform;
};

int pwm_gd32vf103_init(/*@out@*/ struct pwm_gd32vf103 *ctx, int base, clock_id_t clkid);
int pwm_gd32vf103_setup(struct pwm_gd32vf103 *ctx, const struct pwm_gd32vf103_settings *settings);

/* channels */

struct pwm {
    /*@temp@*/ struct pwm_gd32vf103 *parent;
    size_t channel;
    size_t ncycles;
};

typedef enum {
    PWM_GD32VF103_PWM_MODE_0    = 0,
    PWM_GD32VF103_PWM_MODE_1    = 1,
    PWM_GD32VF103_PWM_MODE_COUNT
} pwm_gdvf103_pwm_mode_t;

struct pwm_gd32vf103_pwm_settings {
    pwm_gdvf103_pwm_mode_t mode;
};

int pwm_gd32vf103_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_gd32vf103 *parent, size_t channel);
int pwm_gd32vf103_pwm_setup(struct pwm *ctx, const struct pwm_gd32vf103_pwm_settings *settings);

/* Implements:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

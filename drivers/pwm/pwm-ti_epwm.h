#ifndef PWM_TI_EPWM_H
#define PWM_TI_EPWM_H

#include "pwm.h"
#include "clock.h"

struct C99_EPWM_REGS;

typedef enum {
    PWM_TI_EPWM_WAVEFORM_UP     = 0,
    PWM_TI_EPWM_WAVEFORM_DOWN   = 1,
    PWM_TI_EPWM_WAVEFORM_UPDOWN = 2,
    PWM_TI_EPWM_WAVEFORM_COUNT
} pwm_ti_epwm_waveform_t;

struct pwm_ti_epwm {
    /*@temp@*/ struct C99_EPWM_REGS *base;
    clock_freq_t clk_freq;
    unsigned long freq;
    pwm_ti_epwm_waveform_t waveform;
};

struct pwm_ti_epwm_settings {
    unsigned long frequency;
    pwm_ti_epwm_waveform_t waveform;
};

typedef enum {
    PWM_TI_EPWM_AQ_DISABLE  = 0,
    PWM_TI_EPWM_AQ_CLEAR    = 1,
    PWM_TI_EPWM_AQ_SET      = 2,
    PWM_TI_EPWM_AQ_TOGGLE   = 3,
    PWM_TI_EPWM_AQ_COUNT
} pwm_ti_epwm_aq_t;

int pwm_ti_epwm_init(/*@out@*/ struct pwm_ti_epwm *ctx, int base, clock_id_t clkid);
int pwm_ti_epwm_setup(struct pwm_ti_epwm *ctx, struct pwm_ti_epwm_settings *settings);

typedef enum {
    PWM_TI_EPWM_CMPA,
    PWM_TI_EPWM_CMPB,
    PWM_TI_EPWM_CMP_COUNT
} pwm_ti_epwm_cmp_t;

struct pwm {
    /*@temp@*/ struct pwm_ti_epwm *parent;
    pwm_ti_epwm_cmp_t cmp;
    uint32_t ncycles;
};

struct pwm_ti_epwm_pwm_settings {
    pwm_ti_epwm_aq_t zero_aq;
    pwm_ti_epwm_aq_t prd_aq;
    pwm_ti_epwm_aq_t cmpa_up_aq;
    pwm_ti_epwm_aq_t cmpa_down_aq;
    pwm_ti_epwm_aq_t cmpb_up_aq;
    pwm_ti_epwm_aq_t cmpb_down_aq;
};

int pwm_ti_epwm_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_ti_epwm *parent, pwm_ti_epwm_cmp_t cmp);
int pwm_ti_epwm_pwm_setup(struct pwm *ctx, struct pwm_ti_epwm_pwm_settings *settings);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

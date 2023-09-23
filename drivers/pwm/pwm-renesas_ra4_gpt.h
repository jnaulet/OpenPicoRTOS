#ifndef PWM_RENESAS_RA4_GPT_H
#define PWM_RENESAS_RA4_GPT_H

#include "pwm.h"
#include "clock.h"

#include <stdbool.h>

#define PWM_RENESAS_RA4_GPT_CHANNEL_COUNT 8

struct PWM_RENESAS_RA4_GPT;

struct pwm_renesas_ra4_gpt {
    /*@temp@*/ struct PWM_RENESAS_RA4_GPT *base;
    clock_freq_t freq;
};

int pwm_renesas_ra4_gpt_init(/*@out@*/ struct pwm_renesas_ra4_gpt *ctx, int base, clock_id_t clkid);
/* TODO: pwm_renesas_ra4_gpt_setup() */

typedef enum {
    PWM_RENESAS_RA4_GPT_OUTPUT_A,
    PWM_RENESAS_RA4_GPT_OUTPUT_B,
    PWM_RENESAS_RA4_GPT_OUTPUT_COUNT
} pwm_renesas_ra4_gpt_output_t;

struct pwm {
    /*@temp@*/ struct pwm_renesas_ra4_gpt *parent;
    pwm_renesas_ra4_gpt_output_t output;
    size_t ncycles;
};

typedef enum {
    PWM_RENESAS_RA4_GPT_OADF_PROHIBITED = 0,
    PWM_RENESAS_RA4_GPT_OADF_HI_Z       = 1,
    PWM_RENESAS_RA4_GPT_OADF_0          = 2,
    PWM_RENESAS_RA4_GPT_OADF_1          = 3,
    PWM_RENESAS_RA4_GPT_OADF_COUNT
} pwm_renesas_ra4_gpt_oadf_t;

#define PWM_RENESAS_RA4_PWM_PIN_FUNCTION_SELECT_COUNT 32u

struct pwm_renesas_ra4_gpt_pwm_settings {
    unsigned int pin_function_select; /* see user manual */
    bool output_value_at_stop;
    bool output_setting_at_start_stop;
    pwm_renesas_ra4_gpt_oadf_t oadf;
};

int pwm_renesas_ra4_gpt_pwm_init(/*@out@*/ struct pwm *ctx,
                                 struct pwm_renesas_ra4_gpt *parent,
                                 pwm_renesas_ra4_gpt_output_t output);

int pwm_renesas_ra4_gpt_pwm_setup(struct pwm *ctx, struct pwm_renesas_ra4_gpt_pwm_settings *settings);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

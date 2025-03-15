#ifndef PWM_RP2040_H
#define PWM_RP2040_H

#include "picoRTOS.h"

#include "pwm.h"
#include "ipwm.h"
#include "clock.h"

#include <stddef.h>

struct PWM_RP2040;
struct PWM_RP2040_CH;

struct pwm_rp2040 {
    /*@temp@*/ struct PWM_RP2040 *base;
    clock_freq_t freq;
};

int pwm_rp2040_init(/*@out@*/ struct pwm_rp2040 *ctx, int base, clock_id_t clkid);

#define PWM_RP2040_PWM_CHANNEL_COUNT 8

typedef enum {
    PWM_RP2040_PWM_OUTPUT_A,
    PWM_RP2040_PWM_OUTPUT_B,
    PWM_RP2040_PWM_OUTPUT_COUNT
} pwm_rp2040_pwm_output_t;

struct pwm {
    /*@temp@*/ struct pwm_rp2040 *parent;
    /*@temp@*/ struct PWM_RP2040_CH *ch;
    pwm_rp2040_pwm_output_t output;
    size_t div;
    size_t ncycles;
};

int pwm_rp2040_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_rp2040 *parent, size_t channel, pwm_rp2040_pwm_output_t output);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

typedef enum {
    PWM_RP2040_IPWM_STATE_IDLE,
    PWM_RP2040_IPWM_STATE_ACQ,
    PWM_RP2040_IPWM_STATE_COUNT
} pwm_rp2040_ipwm_state_t;

struct ipwm {
    /*@temp@*/ struct pwm_rp2040 *parent;
    /*@temp@*/ struct PWM_RP2040_CH *ch;
    pwm_rp2040_ipwm_state_t state;
    size_t div;
    picoRTOS_tick_t ref;
    bool polarity;
};

struct pwm_rp2040_ipwm_settings {
    size_t div;
    bool polarity;
};

int pwm_rp2040_ipwm_init(/*@out@*/ struct ipwm *ctx, struct pwm_rp2040 *parent, size_t channel);
int pwm_rp2040_ipwm_setup(struct ipwm *ctx, const struct pwm_rp2040_ipwm_settings *settings);

/* Runtime calls:
 * int ipwm_get_period(struct ipwm *ctx, pwm_period_us_t *period);
 * int ipwm_get_duty_cycle(struct ipwm *ctx, pwm_duty_cycle_t *duty_cycle);
 */

#endif

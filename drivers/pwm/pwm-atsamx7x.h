#ifndef PWM_ATSAMX7X_H
#define PWM_ATSAMX7X_H

#include "pwm.h"
#include "clock.h"
#include <stdbool.h>

struct PWM_ATSAMX7X;
struct PWM_ATSAMX7X_CH;

#define PWM_ATSAMX7X_CHANNEL_COUNT 8

struct pwm_atsamx7x {
    /*@temp@*/ struct PWM_ATSAMX7X *base;
    clock_freq_t freq;
    unsigned long clock_a_fact;
    unsigned long clock_b_fact;
};

typedef enum {
    PWM_ATSAMX7X_CLOCK_PRE_MCK             = 0,
    PWM_ATSAMX7X_CLOCK_PRE_MCK_DIV_2       = 1,
    PWM_ATSAMX7X_CLOCK_PRE_MCK_DIV_4       = 2,
    PWM_ATSAMX7X_CLOCK_PRE_MCK_DIV_8       = 3,
    PWM_ATSAMX7X_CLOCK_PRE_MCK_DIV_16      = 4,
    PWM_ATSAMX7X_CLOCK_PRE_MCK_DIV_32      = 5,
    PWM_ATSAMX7X_CLOCK_PRE_MCK_DIV_64      = 6,
    PWM_ATSAMX7X_CLOCK_PRE_MCK_DIV_128     = 7,
    PWM_ATSAMX7X_CLOCK_PRE_MCK_DIV_256     = 8,
    PWM_ATSAMX7X_CLOCK_PRE_MCK_DIV_512     = 9,
    PWM_ATSAMX7X_CLOCK_PRE_MCK_DIV_1024    = 10,
    PWM_ATSAMX7X_CLOCK_PRE_COUNT
} pwm_atsamx7x_clock_pre_t;

#define PWM_ATSAMX7X_CLOCK_DIV_COUNT 256ul

struct pwm_atsamx7x_settings {
    pwm_atsamx7x_clock_pre_t clock_a_pre;
    unsigned long clock_a_div;
    pwm_atsamx7x_clock_pre_t clock_b_pre;
    unsigned long clock_b_div;
};

int pwm_atsamx7x_init(/*@out@*/ struct pwm_sam *ctx, int base, clock_id_t clkid);
int pwm_atsamx7x_setup(struct pwm_sam *ctx, struct pwm_sam_settings *settings);

/* channels */

struct pwm {
    /*@temp@*/ struct pwm_atsamx7x *parent;
    /*@temp@*/ struct PWM_ATSAMX7X_CH *ch;
    size_t channel;
    clock_freq_t freq;
    size_t ncycles;
};

typedef enum {
    PWM_ATSAMX7X_PWM_CPRE_MCK          = 0,
    PWM_ATSAMX7X_PWM_CPRE_MCK_DIV_2    = 1,
    PWM_ATSAMX7X_PWM_CPRE_MCK_DIV_4    = 2,
    PWM_ATSAMX7X_PWM_CPRE_MCK_DIV_8    = 3,
    PWM_ATSAMX7X_PWM_CPRE_MCK_DIV_16   = 4,
    PWM_ATSAMX7X_PWM_CPRE_MCK_DIV_32   = 5,
    PWM_ATSAMX7X_PWM_CPRE_MCK_DIV_64   = 6,
    PWM_ATSAMX7X_PWM_CPRE_MCK_DIV_128  = 7,
    PWM_ATSAMX7X_PWM_CPRE_MCK_DIV_256  = 8,
    PWM_ATSAMX7X_PWM_CPRE_MCK_DIV_512  = 9,
    PWM_ATSAMX7X_PWM_CPRE_MCK_DIV_1024 = 10,
    PWM_ATSAMX7X_PWM_CPRE_CLKA         = 11,
    PWM_ATSAMX7X_PWM_CPRE_CLKB         = 12,
    PWM_ATSAMX7X_PWM_CPRE_COUNT
} pwm_atsamx7x_pwm_cpre_t;

struct pwm_atsamx7x_pwm_settings {
    bool cpol;
    pwm_atsamx7x_pwm_cpre_t cpre;
};

int pwm_atsamx7x_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_sam *parent, size_t channel);
int pwm_atsamx7x_pwm_setup(struct pwm *ctx, struct pwm_sam_pwm_settings *settings);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

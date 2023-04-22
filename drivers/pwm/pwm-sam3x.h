#ifndef PWM_SAM3X_H
#define PWM_SAM3X_H

#include "pwm.h"
#include "clock.h"
#include <stdbool.h>

struct PWM_SAM3X;
struct PWM_SAM3X_CH;

#define PWM_SAM3X_CHANNEL_COUNT 8

struct pwm_sam3x {
    /*@temp@*/ struct PWM_SAM3X *base;
    clock_freq_t freq;
    unsigned long clock_a_fact;
    unsigned long clock_b_fact;
};

typedef enum {
    PWM_SAM3X_CLOCK_PRE_MCK             = 0,
    PWM_SAM3X_CLOCK_PRE_MCK_DIV_2       = 1,
    PWM_SAM3X_CLOCK_PRE_MCK_DIV_4       = 2,
    PWM_SAM3X_CLOCK_PRE_MCK_DIV_8       = 3,
    PWM_SAM3X_CLOCK_PRE_MCK_DIV_16      = 4,
    PWM_SAM3X_CLOCK_PRE_MCK_DIV_32      = 5,
    PWM_SAM3X_CLOCK_PRE_MCK_DIV_64      = 6,
    PWM_SAM3X_CLOCK_PRE_MCK_DIV_128     = 7,
    PWM_SAM3X_CLOCK_PRE_MCK_DIV_256     = 8,
    PWM_SAM3X_CLOCK_PRE_MCK_DIV_512     = 9,
    PWM_SAM3X_CLOCK_PRE_MCK_DIV_1024    = 10,
    PWM_SAM3X_CLOCK_PRE_COUNT
} pwm_sam3x_clock_pre_t;

#define PWM_SAM3X_CLOCK_DIV_COUNT 256ul

struct pwm_sam3x_settings {
    pwm_sam3x_clock_pre_t clock_a_pre;
    unsigned long clock_a_div;
    pwm_sam3x_clock_pre_t clock_b_pre;
    unsigned long clock_b_div;
};

int pwm_sam3x_init(/*@out@*/ struct pwm_sam *ctx, struct PWM_SAM3X *base, clock_id_t clkid);
int pwm_sam3x_setup(struct pwm_sam *ctx, struct pwm_sam_settings *settings);

/* channels */

struct pwm {
    /*@temp@*/ struct pwm_sam3x *parent;
    /*@temp@*/ struct PWM_SAM3X_CH *ch;
    size_t channel;
    clock_freq_t freq;
    size_t ncycles;
};

typedef enum {
    PWM_SAM3X_PWM_CPRE_MCK          = 0,
    PWM_SAM3X_PWM_CPRE_MCK_DIV_2    = 1,
    PWM_SAM3X_PWM_CPRE_MCK_DIV_4    = 2,
    PWM_SAM3X_PWM_CPRE_MCK_DIV_8    = 3,
    PWM_SAM3X_PWM_CPRE_MCK_DIV_16   = 4,
    PWM_SAM3X_PWM_CPRE_MCK_DIV_32   = 5,
    PWM_SAM3X_PWM_CPRE_MCK_DIV_64   = 6,
    PWM_SAM3X_PWM_CPRE_MCK_DIV_128  = 7,
    PWM_SAM3X_PWM_CPRE_MCK_DIV_256  = 8,
    PWM_SAM3X_PWM_CPRE_MCK_DIV_512  = 9,
    PWM_SAM3X_PWM_CPRE_MCK_DIV_1024 = 10,
    PWM_SAM3X_PWM_CPRE_CLKA         = 11,
    PWM_SAM3X_PWM_CPRE_CLKB         = 12,
    PWM_SAM3X_PWM_CPRE_COUNT
} pwm_sam3x_pwm_cpre_t;

struct pwm_sam3x_pwm_settings {
    bool cpol;
    pwm_sam3x_pwm_cpre_t cpre;
};

int pwm_sam3x_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_sam *parent, size_t channel);
int pwm_sam3x_pwm_setup(struct pwm *ctx, struct pwm_sam_pwm_settings *settings);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

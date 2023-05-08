#ifndef PWM_PIC32MX_H
#define PWM_PIC32MX_H

#include "pwm.h"
#include "clock.h"

#include <stdbool.h>

/* Only support for 16bit timer x for now */

struct PWM_PIC32MX_TC;
struct PWM_PIC32MX_OC;

struct pwm {
    /*@temp@*/ struct PWM_PIC32MX_OC *oc;
    /*@temp@*/ struct PWM_PIC32MX_TC *tc;
    clock_id_t clkid;
    size_t ncycles;
    bool invert;
};

typedef enum {
    PWM_PIC32MX_OCTSEL_TIMERX   = 0,
    PWM_PIC32MX_OCTSEL_TIMERY   = 1,
    PWM_PIC32MX_OCTSEL_COUNT
} pwm_pic32mx_octsel_t;

struct pwm_pic32mx_settings {
    pwm_pic32mx_octsel_t octsel;
    bool invert;
};

int pwm_pic32mx_init(/*@out@*/ struct pwm *ctx, int oc_base, int tc_base, clock_id_t clkid);
int pwm_pic32mx_setup(struct pwm *ctx, struct pwm_pic32mx_settings *settings);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

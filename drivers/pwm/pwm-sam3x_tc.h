#ifndef PWM_SAM3X_TC_H
#define PWM_SAM3X_TC_H

#include "pwm.h"
#include "ipwm.h"
#include "clock.h"

struct PWM_SAM3X_TC;

typedef enum {
    PWM_SAM3X_TC_TIOA,
    PWM_SAM3X_TC_TIOB,
    PWM_SAM3X_TC_TIO_COUNT
} pwm_sam3x_tc_tio_t;

struct pwm {
    /*@temp@*/ struct PWM_SAM3X_TC *base;
    clock_id_t clkid;
    pwm_sam3x_tc_tio_t tio;
    clock_freq_t freq;
    size_t ncycles;
};

typedef enum {
    PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK1,   /* MCK / 2 */
    PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK2,   /* MCK / 8 */
    PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK3,   /* MCK / 32 */
    PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK4,   /* MCK / 128 */
    PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK5,   /* SLCK */
    PWM_SAM3X_TC_TCCLKS_XC0,            /* XC0 */
    PWM_SAM3X_TC_TCCLKS_XC1,            /* XC1 */
    PWM_SAM3X_TC_TCCLKS_XC2,            /* XC2 */
    PWM_SAM3X_TC_TCCLKS_COUNT
} pwm_sam3x_tc_tcclks_t;

typedef enum {
    PWM_SAM3X_TC_WAVSEL_UP          = 0,
    PWM_SAM3X_TC_WAVSEL_UPDOWN      = 1,
    PWM_SAM3X_TC_WAVSEL_UP_RC       = 2,
    PWM_SAM3X_TC_WAVSEL_UPDOWN_RC   = 3,
    PWM_SAM3X_TC_WAVSEL_COUNT
} pwm_sam3x_tc_wavsel_t;

typedef enum {
    PWM_SAM3X_TC_CP_NONE    = 0,
    PWM_SAM3X_TC_CP_SET     = 1,
    PWM_SAM3X_TC_CP_CLEAR   = 2,
    PWM_SAM3X_TC_CP_TOGGLE  = 3,
    PWM_SAM3X_TC_CP_COUNT
} pwm_sam3x_tc_cp_t;

struct pwm_sam3x_tc_settings {
    pwm_sam3x_tc_tio_t tio;
    pwm_sam3x_tc_tcclks_t tcclks;
    pwm_sam3x_tc_wavsel_t wavsel;
    pwm_sam3x_tc_cp_t ncpn;
    pwm_sam3x_tc_cp_t ncpc;
};

int pwm_sam3x_tc_init(/*@out@*/ struct pwm *ctx, int base, clock_id_t clkid);
int pwm_sam3x_tc_setup(struct pwm *ctx, struct pwm_sam3x_tc_settings *settings);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

typedef enum {
    IPWM_SAM3X_TC_LDR_NONE      = 0,
    IPWM_SAM3X_TC_LDR_RISING    = 1,
    IPWM_SAM3X_TC_LDR_FALLING   = 2,
    IPWM_SAM3X_TC_LDR_EDGE      = 3,
    IPWM_SAM3X_TC_LDR_COUNT
} ipwm_sam3x_tc_ldr_t;

typedef enum {
    IPWM_SAM3X_TC_STATE_IDLE,
    IPWM_SAM3X_TC_STATE_ACQ,
    IPWM_SAM3X_TC_STATE_COUNT
} ipwm_sam3x_tc_state_t;

struct ipwm {
    /*@temp@*/ struct PWM_SAM3X_TC *base;
    clock_id_t clkid;
    ipwm_sam3x_tc_state_t state;
    clock_freq_t freq;
    pwm_sam3x_tc_tcclks_t tcclks;
    ipwm_sam3x_tc_ldr_t ldr;
};

struct ipwm_sam3x_tc_settings {
    pwm_sam3x_tc_tcclks_t tcclks;
    ipwm_sam3x_tc_ldr_t ldr;
};

int ipwm_sam3x_tc_init(/*@out@*/ struct ipwm *ctx, int base, clock_id_t clkid);
int ipwm_sam3x_tc_setup(struct ipwm *ctx, struct ipwm_sam3x_tc_settings *settings);

/* Runtime calls:
 * int ipwm_get_period(struct ipwm *ctx, pwm_period_us_t *period);
 * int ipwm_get_duty_cycle(struct ipwm *ctx, pwm_duty_cycle_t *duty_cycle);
 */

#endif

#ifndef PWM_NXP_EMIOS_H
#define PWM_NXP_EMIOS_H

#include "pwm.h"
#include "ipwm.h"
#include "clock.h"
#include <stddef.h>
#include <stdbool.h>

#define PWM_NXP_EMIOS_CHANNEL_COUNT 32

#define PWM_NXP_EMIOS_GPRE_MIN 1
#define PWM_NXP_EMIOS_GPRE_MAX 256

struct PWM_NXP_EMIOS;
struct PWM_NXP_EMIOS_UC;

struct pwm_nxp_emios {
    /*@temp@*/ struct PWM_NXP_EMIOS *base;
    clock_freq_t freq;
    size_t gpre;
};

struct pwm_nxp_emios_settings {
    size_t gpre;
};

int pwm_nxp_emios_init(/*@out@*/ struct pwm_nxp_emios *ctx, int base, clock_id_t clkid);
int pwm_nxp_emios_setup(struct pwm_nxp_emios *ctx, struct pwm_nxp_emios_settings *settings);

struct pwm {
    /*@temp@*/ struct pwm_nxp_emios *parent;
    /*@temp@*/ struct PWM_NXP_EMIOS_UC *uc;
    size_t ncycles;
    size_t ucpre;
};

typedef enum {
    PWM_NXP_EMIOS_UCPRE_1   = 0,
    PWM_NXP_EMIOS_UCPRE_2   = 1,
    PWM_NXP_EMIOS_UCPRE_3   = 2,
    PWM_NXP_EMIOS_UCPRE_4   = 3,
    PWM_NXP_EMIOS_UCPRE_COUNT
} pwm_nxp_emios_ucpre_t;

struct pwm_nxp_emios_pwm_settings {
    pwm_nxp_emios_ucpre_t ucpre;
    bool polarity;
};

int pwm_nxp_emios_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_nxp_emios *parent, size_t channel);
int pwm_nxp_emios_pwm_setup(struct pwm *ctx, struct pwm_nxp_emios_pwm_settings *settings);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

typedef enum {
    PWM_NXP_EMIOS_IPWM_STATE_IDLE,
    PWM_NXP_EMIOS_IPWM_STATE_ACQ,
    PWM_NXP_EMIOS_IPWM_STATE_COUNT
} pwm_nxp_emios_ipwm_state_t;

struct ipwm {
    /*@temp@*/ struct pwm_nxp_emios *parent;
    /*@temp@*/ struct PWM_NXP_EMIOS_UC *uc;
    pwm_nxp_emios_ipwm_state_t state;
    size_t ncycles;
    size_t ucpre;
};

typedef enum {
    IPWM_NXP_EMIOS_IF_BYPASSED  = 0,
    IPWM_NXP_EMIOS_IF_02        = 1,
    IPWM_NXP_EMIOS_IF_04        = 2,
    IPWM_NXP_EMIOS_IF_08        = 3,
    IPWM_NXP_EMIOS_IF_16        = 4,
    IPWM_NXP_EMIOS_IF_COUNT
} pwm_nxp_emios_ipwm_if_t;

struct pwm_nxp_emios_ipwm_settings {
    pwm_nxp_emios_ucpre_t ucpre;
    bool polarity;
    pwm_nxp_emios_ipwm_if_t filter;
};

int pwm_nxp_emios_ipwm_init(/*@out@*/ struct ipwm *ctx, struct pwm_nxp_emios *parent, size_t channel);
int pwm_nxp_emios_ipwm_setup(struct ipwm *ctx, struct pwm_nxp_emios_ipwm_settings *settings);

/* Runtime calls:
 * int ipwm_get_period(struct ipwm *ctx, pwm_period_us_t *period);
 * int ipwm_get_duty_cycle(struct ipwm *ctx, pwm_duty_cycle_t *duty_cycle);
 */

#endif

#ifndef PWM_AVR_H
#define PWM_AVR_H

#include "pwm.h"
#include "clock.h"

#include <stdint.h>

struct PWM_AVR;

typedef enum {
    PWM_AVR_CS_NO_CLOCK         = 0,
    PWM_AVR_CS_CLKIO_1          = 1,
    PWM_AVR_CS_CLKIO_8          = 2,
    PWM_AVR_CS_CLKIO_64         = 3,
    PWM_AVR_CS_CLKIO_256        = 4,
    PWM_AVR_CS_CLKIO_1024       = 5,
    PWM_AVR_CS_EXT_CLK_FALLING  = 6,
    PWM_AVR_CS_EXT_CLK_RISING   = 7,
    PWM_AVR_CS_COUNT
} pwm_avr_cs_t;

typedef enum {
    PWM_AVR_WAVEFORM_PHASE_CORRECT_8BIT         = 1,
    PWM_AVR_WAVEFORM_PHASE_CORRECT_9BIT         = 2,
    PWM_AVR_WAVEFORM_PHASE_CORRECT_10BIT        = 3,
    PWM_AVR_WAVEFORM_FAST_PWM_8BIT              = 5,
    PWM_AVR_WAVEFORM_FAST_PWM_9BIT              = 6,
    PWM_AVR_WAVEFORM_FAST_PWM_10BIT             = 7,
    PWM_AVR_WAVEFORM_PHASE_FREQ_CORRECT_ICRn    = 8,
    PWM_AVR_WAVEFORM_PHASE_FREQ_CORRECT_OCRnA   = 9,
    PWM_AVR_WAVEFORM_PHASE_CORRECT_ICRn         = 10,
    PWM_AVR_WAVEFORM_PHASE_CORRECT_OCRnA        = 11,
    PWM_AVR_WAVEFORM_FAST_PWM_ICRn              = 14,
    PWM_AVR_WAVEFORM_FAST_PWM_OCRnA             = 15,
    PWM_AVR_WAVEFORM_COUNT
} pwm_avr_waveform_t;

/* check the documenttion for more info
 * (depends on waveform) */
typedef enum {
    PWM_AVR_MODE_NORMAL = 0,
    PWM_AVR_MODE_TOGGLE = 1,
    PWM_AVR_MODE_CLEAR  = 2,
    PWM_AVR_MODE_SET    = 3,
    PWM_AVR_MODE_COUNT
} pwm_avr_mode_t;

typedef enum {
    PWM_AVR_OCnA,
    PWM_AVR_OCnB,
    PWM_AVR_OCnC,
    PWM_AVR_OC_COUNT
} pwm_avr_oc_t;

struct pwm_avr {
    /*@temp@*/ struct PWM_AVR *base;
    clock_freq_t freq;
    uint32_t prescale;
    pwm_avr_waveform_t waveform;
    pwm_avr_cs_t cs;
    uint16_t ncycles;
};

struct pwm_avr_settings {
    pwm_avr_cs_t cs;
    pwm_avr_waveform_t waveform;
};

int pwm_avr_init(/*@out@*/ struct pwm_avr *ctx, struct PWM_AVR *base, clock_id_t clkid);
int pwm_avr_setup(struct pwm_avr *ctx, struct pwm_avr_settings *settings);

struct pwm_settings {
    pwm_avr_mode_t mode;
};

struct pwm {
    /*@temp@*/ struct pwm_avr *pwm_avr;
    pwm_avr_oc_t oc;
};

int pwm_avr_pwm_init(/*@out@*/ struct pwm *ctx, struct pwm_avr *pwm_avr, pwm_avr_oc_t oc);
int pwm_avr_pwm_setup(struct pwm *ctx, struct pwm_settings *settings);

/* Runtime calls:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 *
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

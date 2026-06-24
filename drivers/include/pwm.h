#ifndef HAL_PWM_H
#define HAL_PWM_H

#include <errno.h>
#include <stdint.h>
#include <stddef.h>

struct pwm;

typedef uint32_t pwm_period_us_t;
typedef uint16_t pwm_duty_cycle_t;

/* Constants: PWM duty cycle
 *
 * PWM_DUTY_CYCLE_MIN - 0% duty cycle
 * PWM_DUTY_CYCLE_MAX - 100% duty cycle
 */
#define PWM_DUTY_CYCLE_MIN 0        /* 0% */
#define PWM_DUTY_CYCLE_MAX 65535    /* 100% */

/* Macro: PWM_DUTY_CYCLE_PCENT(x)
 * Converts a value in % to pwm_duty_cycle_t */
#define PWM_DUTY_CYCLE_PCENT(x) (pwm_duty_cycle_t)(((x) * PWM_DUTY_CYCLE_MAX) / 100)
/* Macro: PWM_DUTY_CYCLE_PMIL(x)
 * Converts a value in 1/10th of a % to pwm_duty_cycle_t */
#define PWM_DUTY_CYCLE_PMIL(x) (pwm_duty_cycle_t)(((x) * PWM_DUTY_CYCLE_MAX) / 1000)
/* Macro: PWM_DUTY_CYCLE_PTTHOU(x)
 * Converts a value in 1/100th of a % to pwm_duty_cycle_t */
#define PWM_DUTY_CYCLE_PTTHOU(x) (pwm_duty_cycle_t)(((x) * PWM_DUTY_CYCLE_MAX) / 10000)

/**
 * int **pwm_set_period**(**struct pwm** \*<ins>ctx</ins>, **pwm_period_us_t** <ins>period</ins>);
 * > Defines a PWM period
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);

/**
 * int **pwm_set_duty_cycle**(**struct pwm** \*<ins>ctx</ins>, **pwm_duty_cycle_t** <ins>duty_cycle</ins>);
 * > Defines a PWM duty cycle
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);

/**
 * void **pwm_start**(**struct pwm** \*<ins>ctx</ins>);
 * > Starts the PWM
 */
void pwm_start(struct pwm *ctx);

/**
 * void **pwm_stop**(**struct pwm** \*<ins>ctx</ins>);
 * > Stops the PWM
 */
void pwm_stop(struct pwm *ctx);

#endif

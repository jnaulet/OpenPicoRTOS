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

/* Function: pwm_set_period
 * Defines a PWM period
 *
 * Parameters:
 *  ctx - A PWM instance
 *  period - The value of the period in microseconds
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);

/* Function: pwm_set_duty_cycle
 * Defines a PWM duty cycle
 *
 * Parameters:
 *  ctx - A PWM instance
 *  duty_cycle - The value of the period in pwm_duty_cycle_t (0-65535)
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);

/* Function: pwm_start
 * Starts the PWM
 *
 * Parameters:
 *  ctx - A PWM instance
 */
void pwm_start(struct pwm *ctx);

/* Function: pwm_stop
 * Stops the PWM
 *
 * Parameters:
 *  ctx - A PWM instance
 */
void pwm_stop(struct pwm *ctx);

#endif

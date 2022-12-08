#ifndef HAL_IPWM_H
#define HAL_IPWM_H

#include <errno.h>
#include "pwm.h"

struct ipwm;

/* Function: ipwm_get_period
 * Measures the period of a train of pulses
 *
 * Parameters:
 *  ctx - A IPWM instance
 *  period - A buffer where to store the measured data
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int ipwm_get_period(struct ipwm *ctx, pwm_period_us_t *period);

/* Function: ipwm_get_duty_cycle
 * Measures the duty cycle of a PWM
 *
 * Parameters:
 *  ctx - A IPWM instance
 *  duty_cycle - A buffer where to store the measured data
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int ipwm_get_duty_cycle(struct ipwm *ctx, pwm_duty_cycle_t *duty_cycle);

#endif

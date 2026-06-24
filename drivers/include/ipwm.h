#ifndef HAL_IPWM_H
#define HAL_IPWM_H

#include <errno.h>
#include "pwm.h"

struct ipwm;

/**
 * int **ipwm_get_period**(**struct ipwm** \*<ins>ctx</ins>,
 * **pwm_period_us_t** \*<ins>period</ins>);
 * > Measures the <ins>period</ins> of a train of pulses
 *
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int ipwm_get_period(struct ipwm *ctx, pwm_period_us_t *period);

/**
 * int **ipwm_get_duty_cycle**(**struct ipwm** \*<ins>ctx</ins>, 
 * **pwm_duty_cycle_t** \*<ins>duty_cycle</ins>);
 * > Measures the <ins>duty cycle</ins> of a PWM
 *
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int ipwm_get_duty_cycle(struct ipwm *ctx, pwm_duty_cycle_t *duty_cycle);

#endif

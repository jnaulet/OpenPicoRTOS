#ifndef HAL_PWM_SAFETY_H
#define HAL_PWM_SAFETY_H

#include "pwm.h"

/* Function: pwm_claim
 * Claims the pwm for this process/thread (opt. MPU-related feature)
 *
 * Parameters:
 *  pwm - A pwm instance
 */
struct pwm *pwm_claim(/*@returned@*/ struct pwm *ctx);

#endif

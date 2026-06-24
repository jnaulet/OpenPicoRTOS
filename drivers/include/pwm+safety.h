#ifndef HAL_PWM_SAFETY_H
#define HAL_PWM_SAFETY_H

#include "pwm.h"

/**
 * struct pwm \***pwm_claim**(**struct pwm** \*<ins>ctx</ins>);
 * > Claims the pwm channel <ins>ctx</ins> & its dependencies
 * ### NOTES
 * > This feature is related to MPU & will only have an effect if
 * > `CONFIG_MPU` is active.
 */
struct pwm *pwm_claim(/*@returned@*/ struct pwm *ctx);

#endif

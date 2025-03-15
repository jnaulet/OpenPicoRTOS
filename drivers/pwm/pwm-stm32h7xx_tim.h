#ifndef PWM_STM32H7XX_TIM_H
#define PWM_STM32H7XX_TIM_H

#include "pwm.h"
#include "clock.h"

#define PWM_STM32H7XX_TIM_CHANNEL_MAX 6

struct PWM_STM32H7XX_TIM;

struct pwm_stm32h7xx_tim {
    /*@temp@*/ struct PWM_STM32H7XX_TIM *base;
    clock_id_t clkid;
    size_t channel_count;
    clock_freq_t freq;
};

typedef enum {
    PWM_STM32H7XX_TIM_ALIGN_EDGE_UP,
    PWM_STM32H7XX_TIM_ALIGN_EDGE_DOWN,
    PWM_STM32H7XX_TIM_ALIGN_CENTER_MODE_1,
    PWM_STM32H7XX_TIM_ALIGN_CENTER_MODE_2,
    PWM_STM32H7XX_TIM_ALIGN_CENTER_MODE_3,
    PWM_STM32H7XX_TIM_ALIGN_COUNT
} pwm_stm32h7xx_tim_align_t;

struct pwm_stm32h7xx_tim_settings {
    pwm_stm32h7xx_tim_align_t align;
};

int pwm_stm32h7xx_tim_init(/*@out@*/ struct pwm_stm32h7xx_tim *ctx,
                           int base, clock_id_t clkid,
                           size_t channel_count);

int pwm_stm32h7xx_tim_setup(struct pwm_stm32h7xx_tim *ctx,
                            const struct pwm_stm32h7xx_tim_settings *settings);

/* PWM */

struct pwm {
    /*@temp@*/ struct pwm_stm32h7xx_tim *parent;
    size_t channel;
    size_t ncycles;
};

typedef enum {
    PWM_STM32H7XX_TIM_OC_PWM_MODE_1,
    PWM_STM32H7XX_TIM_OC_PWM_MODE_2,
    PWM_STM32H7XX_TIM_OC_COMBINED_PWM_MODE_1,
    PWM_STM32H7XX_TIM_OC_COMBINED_PWM_MODE_2,
    PWM_STM32H7XX_TIM_OC_ASYMETRIC_PWM_MODE_1,
    PWM_STM32H7XX_TIM_OC_ASYMETRIC_PWM_MODE_2,
    PWM_STM32H7XX_TIM_OC_COUNT
} pwm_stm32h7xx_tim_oc_t;

struct pwm_stm32h7xx_tim_pwm_settings {
    pwm_stm32h7xx_tim_oc_t oc;
};

int pwm_stm32h7xx_tim_pwm_init(/*@out@*/ struct pwm *ctx,
                               struct pwm_stm32h7xx_tim *parent,
                               size_t channel);

int pwm_stm32h7xx_tim_pwm_setup(struct pwm *ctx, const struct pwm_stm32h7xx_tim_pwm_settings *settings);

/* Implements:
 * int pwm_set_period(struct pwm *ctx, pwm_period_us_t period);
 * int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle);
 * void pwm_start(struct pwm *ctx);
 * void pwm_stop(struct pwm *ctx);
 */

#endif

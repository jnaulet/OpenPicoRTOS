#ifndef HAL_GPIO_SAFETY_H
#define HAL_GPIO_SAFETY_H

#include "gpio.h"

/**
 * struct gpio \***gpio_claim**(**struct gpio** \*<ins>ctx</ins>);
 * > Claims the gpio for this process/thread (opt. MPU-related feature)
 */
struct gpio *gpio_claim(/*@returned@*/ struct gpio *ctx);

/* TODO: more */

#endif

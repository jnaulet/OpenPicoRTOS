#ifndef HAL_GPIO_SAFETY_H
#define HAL_GPIO_SAFETY_H

#include "gpio.h"

/* Function: gpio_claim
 * Claims the gpio for this process/thread (opt. MPU-related feature)
 *
 * Parameters:
 *  gpio - A gpio instance
 */
struct gpio *gpio_claim(/*@returned@*/ struct gpio *ctx);

/* TODO: more */

#endif

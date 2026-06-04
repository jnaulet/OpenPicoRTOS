#ifndef HAL_CAN_SAFETY_H
#define HAL_CAN_SAFETY_H

#include "can.h"

/* Function: can_claim
 * Claims the can for this process/thread (MPU-related feature)
 *
 * Parameters:
 *  can - A can instance
 */
struct can *can_claim(/*@returned@*/ struct can *ctx);

#endif

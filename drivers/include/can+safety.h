#ifndef HAL_CAN_SAFETY_H
#define HAL_CAN_SAFETY_H

#include "can.h"

/**
 * struct can \***can_claim**(**struct can** \*<ins>ctx</ins>);
 * > Claims the can for this process/thread (MPU-related feature)
 * ### NOTES
 * > This feature is related to MPU & will only have an effect if
 * > `CONFIG_MPU` is active.
 */
struct can *can_claim(/*@returned@*/ struct can *ctx);

#endif

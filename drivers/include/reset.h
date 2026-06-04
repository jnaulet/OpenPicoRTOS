#ifndef HAL_RESET_H
#define HAL_RESET_H

#include <errno.h>

struct reset;

/* Function: reset_perform
 * Performs a reset
 *
 * Parameters:
 *  ctx - A reset instance
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int reset_perform(struct reset *ctx);

#endif

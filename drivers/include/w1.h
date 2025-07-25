#ifndef HAL_W1_H
#define HAL_W1_H

#include <errno.h>
#include <stddef.h>

struct w1;

/* Function: w1_reset
 * Send reset on a W1 interface
 *
 * Parameters:
 *  ctx - A W1 instance
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int w1_reset(struct w1 *ctx);

/* Function: w1_write
 * Write data to a W1 interface
 *
 * Parameters:
 *  ctx - A W1 instance
 *  buf - A buffer containing the data to write
 *  n - The size of the data to write (in bytes)
 *
 * Returns:
 * The number of byte written if success, -errno otherwise
 */
int w1_write(struct w1 *ctx, const void *buf, size_t n);

/* Function: w1_read
 * Read data from a W1 interface
 *
 * Parameters:
 *  ctx - A W1 instance
 *  buf - A buffer to receive the data
 *  n - The size of the buffer (in bytes)
 *
 * Returns:
 * The number of byte read if success, -errno otherwise
 */
int w1_read(struct w1 *ctx, void *buf, size_t n);

#endif

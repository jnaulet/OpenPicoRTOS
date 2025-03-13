#ifndef HAL_RNG_H
#define HAL_RNG_H

#include <errno.h>
#include <stddef.h>

struct rng;

/* Function: rng_read
 * Read an arbitrary number of bytes from a RNG
 *
 * Parameters:
 *  rng - A rng instance
 *
 * Returns:
 * The number of bytes read if success, -errno otherwise
 */
int rng_read(struct rng *ctx, void *buf, size_t n);

#endif

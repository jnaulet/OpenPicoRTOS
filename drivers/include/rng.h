#ifndef HAL_RNG_H
#define HAL_RNG_H

#include <errno.h>

struct rng;

/* Function: rng_read
 * Read an arbitrary number of bytes from a rng
 *
 * Parameters:
 *  rng - A rng instance
 *
 * Returns:
 * The nuber of bytes read if success, -errno otherwise
 */
int rng_read(struct rng *ctx, void *buf, size_t n);

#endif

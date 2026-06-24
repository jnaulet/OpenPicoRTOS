#ifndef HAL_RNG_H
#define HAL_RNG_H

#include <errno.h>
#include <stddef.h>

struct rng;

/**
 * int **rng_read**(**struct rng** \*<ins>ctx</ins>, **void** \*<ins>buf</ins>, **size_t** <ins>n</ins>);
 * > Read an arbitrary number of bytes from a RNG
 * ### RETURN
 * Returns the number of bytes read if success, -errno otherwise
 */
int rng_read(struct rng *ctx, void *buf, size_t n);

/**
 * struct rng \***rng_claim**(**struct rng** \*<ins>ctx</ins>);
 * > Claims the rng for this process/thread (MPU-related feature)
 */
/*@unused@*/ struct rng *rng_claim(/*@returned@*/ struct rng *ctx);

#endif

#ifndef RNG_XORSHIFT32_H
#define RNG_XORSHIFT32_H

#include "rng.h"
#include <stdint.h>

struct rng {
  uint32_t pool;
};

int rng_xorshift32_init(/*@out@*/ struct rng *ctx, uint32_t seed);

/* Implements:
 * int rng_read(struct rng *ctx, void *buf, size_t n);
 */

#endif

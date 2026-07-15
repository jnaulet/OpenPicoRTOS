#ifndef RNG_ATSAMX7X_H
#define RNG_ATSAMX7X_H

#include "rng.h"
#include <stdint.h>

struct RNG_ATSAMX7X_TRNG;

struct rng {
    /*@temp@*/ struct RNG_ATSAMX7X_TRNG *base;
    uint32_t pool;
};

int rng_atsamx7x_init(/*@out@*/ struct rng *ctx, int base);

/* Implements:
 * int rng_read(struct rng *ctx, void *buf, size_t n);
 * struct rng *rng_claim(struct rng *ctx);
 */

#endif

#ifndef RNG_STM32H7XX_TRNG_H
#define RNG_STM32H7XX_TRNG_H

#include "rng.h"
#include "clock.h"

struct RNG_STM32H7XX_TRNG;

struct rng {
    /*@temp@*/ struct RNG_STM32H7XX_TRNG *base;
};

int rng_stm32h7xx_trng_init(/*@out@*/ struct rng *ctx, int base, clock_id_t clkid);

/* Implements:
 * int rng_read(struct rng *ctx, void *buf, size_t n);
 */

#endif

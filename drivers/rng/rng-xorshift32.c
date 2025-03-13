#include "rng-xorshift32.h"
#include "picoRTOS.h"

#include <string.h>

static uint32_t xorshift32(uint32_t value)
{
    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;

    return value;
}

/* Function:
 * Initializes a pseudo-random xorshift32 instance
 *
 * Parameters:
 *  ctx - The RNG to init
 *  seed - The xorshift32 seed
 *
 * Returns:
 * Always 0
 */
int rng_xorshift32_init(/*@out@*/ struct rng *ctx, uint32_t seed)
{
    /* force 1st shift */
    ctx->pool = xorshift32(seed);
    return 0;
}

/*
 * hal
 */

int rng_read(struct rng *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    size_t count = n;
    uint8_t *buf8 = (uint8_t*)buf;

    do {
        switch (count) {
        default: *buf8++ = (uint8_t)(ctx->pool >> 24); count--; /* >= 4 */ /*@fallthrough@*/
        case 3: *buf8++ = (uint8_t)(ctx->pool >> 16); count--;  /*@fallthrough@*/
        case 2: *buf8++ = (uint8_t)(ctx->pool >> 8); count--;   /*@fallthrough@*/
        case 1: *buf8++ = (uint8_t)(ctx->pool >> 0); count--;   /*@fallthrough@*/
        case 0: break;
        }

        /* inc & shift */
        ctx->pool = xorshift32(ctx->pool);

    } while (count > 0);

    return (int)(n - count);
}

#include "rng-xorshift32.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <string.h>

static uint32_t xorshift32(uint32_t value)
{
    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;

    return value;
}

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

    while (count > sizeof(ctx->pool)) {
        *buf8++ = (uint8_t)(ctx->pool >> 24);
        *buf8++ = (uint8_t)(ctx->pool >> 16);
        *buf8++ = (uint8_t)(ctx->pool >> 8);
        *buf8++ = (uint8_t)(ctx->pool >> 0);
        /* inc & shift */
        count -= sizeof(ctx->pool);
        ctx->pool = xorshift32(ctx->pool);
    }

    switch (count) {
    case 4: *buf8++ = (uint8_t)(ctx->pool >> 24);   /*@fallthrough@*/
    case 3: *buf8++ = (uint8_t)(ctx->pool >> 16);   /*@fallthrough@*/
    case 2: *buf8++ = (uint8_t)(ctx->pool >> 8);    /*@fallthrough@*/
    case 1: *buf8++ = (uint8_t)(ctx->pool >> 0);    /*@fallthrough@*/
    default: break;
    }

    /* final shift */
    ctx->pool = xorshift32(ctx->pool);
    return (int)n;
}

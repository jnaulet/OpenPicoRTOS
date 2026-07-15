#include "rng-atsamx7x.h"

#include <picoRTOS.h>
#include <picoRTOS_port.h>

struct RNG_ATSAMX7X_TRNG {
    volatile uint32_t TRNG_CR;
    uint32_t RESERVED0[3];
    volatile uint32_t TRNG_IER;
    volatile uint32_t TRNG_IDR;
    volatile uint32_t TRNG_IMR;
    volatile uint32_t TRNG_ISR;
    uint32_t RESERVED1[12];
    volatile uint32_t TRNG_ODATA;
};

#define TRNG_CR_WAKEY_M  0xffffffu
#define TRNG_CR_WAKEY(x) (((x) & TRNG_CR_WAKEY_M) << 8)
#define TRNG_CR_ENABLE   (1 << 0)

#define TRNG_IxR_DATRDY (1 << 0)

int rng_atsamx7x_init(/*@out@*/ struct rng *ctx, int base)
{
    ctx->base = (struct RNG_ATSAMX7X_TRNG *)base; // NOLINT
    ctx->base->TRNG_CR = (uint32_t)(TRNG_CR_WAKEY(0x524e47) |
                                    TRNG_CR_ENABLE);
    return 0;
}

int rng_read(struct rng *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    size_t count = n;
    uint8_t *buf8 = (uint8_t*)buf;

    do {
        if ((ctx->base->TRNG_ISR & TRNG_IxR_DATRDY) != 0) {
            /* refresh pool */
            ctx->pool = ctx->base->TRNG_ODATA;

            switch (count) {
            default: *buf8++ = (uint8_t)(ctx->pool >> 24); count--; /* >= 4 */ /*@fallthrough@*/
            case 3: *buf8++ = (uint8_t)(ctx->pool >> 16); count--;  /*@fallthrough@*/
            case 2: *buf8++ = (uint8_t)(ctx->pool >> 8); count--;   /*@fallthrough@*/
            case 1: *buf8++ = (uint8_t)(ctx->pool >> 0); count--;   /*@fallthrough@*/
            case 0: break;
            }

        }else
            break;

    } while (count > 0);

    if (n == count) return -EAGAIN;
    return (int)(n - count);
}

struct rng *rng_claim(struct rng *ctx)
{
    picoRTOS_mpu_add_region(ctx->base, sizeof(*ctx->base),
                            MM_URW | MM_NON_CACHEABLE);
    return ctx;
}

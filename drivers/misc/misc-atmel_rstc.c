#include "misc-atmel_rstc.h"
#include <stdint.h>

struct ATMEL_RSTC {
    volatile uint32_t RSTC_CR;
    volatile uint32_t RSTC_SR;
    volatile uint32_t RSTC_MR;
};

#define RSTC_CR_KEY_M   0xffu
#define RSTC_CR_KEY(x)  (((x) & RSTC_CR_KEY_M) << 24)
#define RSTC_CR_EXTRST  (1 << 3)
#define RSTC_CR_PROCRST (1 << 0)

#define RSTC_SR_SRCMP      (1 << 17)
#define RSTC_SR_NRSTL      (1 << 16)
#define RSTC_SR_RSTTYPE_M  0x7u
#define RSTC_SR_RSTTYPE_S  8
#define RSTC_SR_RSTTYPE(x) (((x) & RSTC_SR_RSTTYPE_M) << RSTC_SR_RSTTYPE_S)
#define RSTC_SR_URSTS      (1 << 0)

#define RSTC_MR_KEY_M    0xffu
#define RSTC_MR_KEY(x)   (((x) & RSTC_MR_KEY_M) << 24)
#define RSTC_MR_ERSTL_M  0xfu
#define RSTC_MR_ERSTL(x) (((x) & RSTC_MR_ERSTL_M) << 8)
#define RSTC_MR_URSTIEN  (1 << 4)
#define RSTC_MR_URSTEN   (1 << 0)

int atmel_rstc_init(/*@out@*/ struct atmel_rstc *ctx, int base)
{
    ctx->base = (struct ATMEL_RSTC*)base; // NOLINT
    return 0;
}

int atmel_rstc_reset(struct atmel_rstc *ctx)
{
    ctx->base->RSTC_CR = (uint32_t)(RSTC_CR_KEY(0xa5) |
                                    RSTC_CR_EXTRST |
                                    RSTC_CR_PROCRST);
    return 0;
}

int atmel_rstc_get_rsttyp(struct atmel_rstc *ctx, atmel_rstc_rsttyp_t *rsttyp)
{
    *rsttyp = (atmel_rstc_rsttyp_t)((ctx->base->RSTC_SR >> RSTC_SR_RSTTYPE_S) &
                                    RSTC_SR_RSTTYPE_M);
    return 0;
}

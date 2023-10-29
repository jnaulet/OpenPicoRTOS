#include "misc-nxp_igf.h"
#include "picoRTOS.h"

#include <stdint.h>

#define IGF_A_COUNT 31
#define IGF_B_COUNT 13

struct NXP_IGF {
    volatile uint32_t MCR0;
    volatile uint32_t MSR0;
    uint32_t RESERVED0[2];
    volatile uint32_t PRESR0;
    uint32_t RESERVED1[2];
    volatile uint32_t RTHR0;
    volatile uint32_t FTHR0;
    uint32_t RESERVED2[7];
    struct {
        volatile uint32_t MCR;
        volatile uint32_t MSR;
        uint32_t RESERVED0[6];
        volatile uint32_t RTHR;
        uint32_t RESERVED1[8];
    } A[IGF_A_COUNT];
    volatile uint32_t MCR32;
    volatile uint32_t MSR32;
    uint32_t RESERVED3[2];
    volatile uint32_t PRESR32;
    uint32_t RESERVED4[2];
    volatile uint32_t RTHR32;
    uint32_t RESERVED5[8];
    struct {
        volatile uint32_t MCR;
        volatile uint32_t MSR;
        uint32_t RESERVED0[6];
        volatile uint32_t RTHR;
        uint32_t RESERVED1[8];
    } B[IGF_B_COUNT];
};

#define MCR_FRZ  (1 << 29)
#define MCR_POL  (1 << 7)
#define MCR_FGEN (1 << 6)

/*@exposed@*/ static uint32_t *get_MCR(struct nxp_igf *ctx)
{
    if (ctx->channel == (size_t)0)
        return (uint32_t*)&ctx->base->MCR0;

    if (ctx->channel < (size_t)32)
        return (uint32_t*)&ctx->base->A[ctx->channel - (size_t)1].MCR;

    if (ctx->channel == (size_t)32)
        return (uint32_t*)&ctx->base->MCR32;

    return (uint32_t*)&ctx->base->B[ctx->channel - (size_t)33].MCR;
}

int nxp_igf_init(struct nxp_igf *ctx, int base, size_t channel)
{
    picoRTOS_assert(channel < (size_t)NXP_IGF_CHANNEL_COUNT, return -EINVAL);

    uint32_t *MCR;

    ctx->base = (struct NXP_IGF*)base;
    ctx->channel = channel;

    /* default: bypass */
    MCR = get_MCR(ctx);
    *MCR = (uint32_t)MCR_FGEN;

    return 0;
}

int nxp_igf_setup(struct nxp_igf *ctx, struct nxp_igf_settings *settings)
{
    uint32_t *MCR = get_MCR(ctx);

    if (settings->invert) *MCR |= MCR_POL;
    else *MCR &= ~MCR_POL;

    return 0;
}

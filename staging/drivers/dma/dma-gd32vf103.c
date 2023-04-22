#include "dma-gd32vf103.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#ifndef DMA_PADDR_BASE
# error DMA_PADDR_BASE is not defined
#endif

struct DMA_GD32VF103 {
    volatile uint32_t DMA_INTF;
    volatile uint32_t DMA_INTC;
    struct DMA_GD32VF103_CH {
        volatile uint32_t DMA_CHxCTL;
        volatile uint32_t DMA_CHxCNT;
        volatile uint32_t DMA_CHxPADDR;
        volatile uint32_t DMA_CHxMADDR;
        uint32_t RESERVED0;
    } CH[DMA_GD32VF103_MAX_CH_COUNT];
};

#define DMA_INTF_ERRIF0  (1 << 3)
#define DMA_INTF_HTFIFO0 (1 << 2)
#define DMA_INTF_FTFIF0  (1 << 1)
#define DMA_INTF_GIF0    (1 << 0)

#define DMA_CHxCTL_M2M       (1 << 14)
#define DMA_CHxCTL_PRIO_M    0x3u
#define DMA_CHxCTL_PRIO(x)   (((x) & DMA_CHxCTL_PRIO_M) << 12)
#define DMA_CHxCTL_MWIDTH_M  0x3u
#define DMA_CHxCTL_MWIDTH(x) (((x) & DMA_CHxCTL_MWIDTH_M) << 10)
#define DMA_CHxCTL_PWIDTH_M  0x3u
#define DMA_CHxCTL_PWIDTH(x) (((x) & DMA_CHxCTL_PWIDTH_M) << 8)
#define DMA_CHxCTL_MNAGA     (1 << 7)
#define DMA_CHxCTL_PNAGA     (1 << 6)
#define DMA_CHxCTL_CMEN      (1 << 5)
#define DMA_CHxCTL_DIR       (1 << 4)
#define DMA_CHxCTL_ERRIE     (1 << 3)
#define DMA_CHxCTL_HTFIE     (1 << 2)
#define DMA_CHxCTL_FTFIE     (1 << 1)
#define DMA_CHxCTL_CHEN      (1 << 0)

int dma_gd32vf103_init(struct dma *ctx, struct DMA_GD32VF103 *base, size_t channel)
{
    if (!picoRTOS_assert(channel < (size_t)DMA_GD32VF103_MAX_CH_COUNT)) return -EINVAL;

    ctx->base = base;
    ctx->channel = channel;
    ctx->ch = &ctx->base->CH[channel];

    return 0;
}

static int dma_prepare(struct dma *ctx, struct dma_xfer *xfer)
{
    if (!picoRTOS_assert(xfer->size > 0)) return -EAGAIN;
    if (!picoRTOS_assert(xfer->size <= sizeof(uint32_t))) return -EAGAIN;
    if (!picoRTOS_assert(xfer->byte_count > 0)) return -EAGAIN;

    /* this one is tricky, as the DMA is designed to transfer data between
     * memory & peripherals */
    ctx->ch->DMA_CHxCTL = (uint32_t)(DMA_CHxCTL_MWIDTH(xfer->size - 1) |
                                     DMA_CHxCTL_PWIDTH(xfer->size - 1));

    ctx->ch->DMA_CHxCNT = (uint32_t)(xfer->byte_count / xfer->size);

    if (xfer->saddr >= (intptr_t)DMA_PADDR_BASE) {
        /* peripheral to memory */
        ctx->ch->DMA_CHxPADDR = (uint32_t)xfer->saddr;
        ctx->ch->DMA_CHxMADDR = (uint32_t)xfer->daddr;
        ctx->ch->DMA_CHxCTL &= ~DMA_CHxCTL_DIR;
        /* increment read */
        if (xfer->incr_read) ctx->ch->DMA_CHxCTL |= DMA_CHxCTL_PNAGA;
        else ctx->ch->DMA_CHxCTL &= ~DMA_CHxCTL_PNAGA;
        /* increment write */
        if (xfer->incr_write) ctx->ch->DMA_CHxCTL |= DMA_CHxCTL_MNAGA;
        else ctx->ch->DMA_CHxCTL &= ~DMA_CHxCTL_MNAGA;

    }else{
        /* memory to peripheral */
        ctx->ch->DMA_CHxPADDR = (uint32_t)xfer->daddr;
        ctx->ch->DMA_CHxMADDR = (uint32_t)xfer->saddr;
        ctx->ch->DMA_CHxCTL |= DMA_CHxCTL_DIR;
        /* increment read */
        if (xfer->incr_read) ctx->ch->DMA_CHxCTL |= DMA_CHxCTL_MNAGA;
        else ctx->ch->DMA_CHxCTL &= ~DMA_CHxCTL_MNAGA;
        /* increment write */
        if (xfer->incr_write) ctx->ch->DMA_CHxCTL |= DMA_CHxCTL_PNAGA;
        else ctx->ch->DMA_CHxCTL &= ~DMA_CHxCTL_PNAGA;
    }

    return 0;
}

int dma_setup(struct dma *ctx, struct dma_xfer *xfer)
{
    int res;

    if ((res = dma_prepare(ctx, xfer)) < 0)
        return res;

    /* start in circular mode */
    ctx->ch->DMA_CHxCTL |= (DMA_CHxCTL_CMEN | DMA_CHxCTL_CHEN);
    return 0;
}

int dma_xfer(struct dma *ctx, struct dma_xfer *xfer)
{
    int res;

    if ((res = dma_prepare(ctx, xfer)) < 0)
        return res;

    /* trigger */
    ctx->ch->DMA_CHxCTL |= DMA_CHxCTL_CHEN;
    return 0;
}

int dma_xfer_done(struct dma *ctx)
{
    size_t shift = ctx->channel * (size_t)4;
    uint32_t mask = (uint32_t)DMA_INTF_FTFIF0 << shift;

    if ((ctx->base->DMA_INTF & mask) == 0)
        return -EAGAIN;

    ctx->base->DMA_INTC = (uint32_t)mask;
    return 0;
}

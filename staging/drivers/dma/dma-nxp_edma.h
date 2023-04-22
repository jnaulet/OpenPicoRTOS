#ifndef DMA_NXP_EDMA_H
#define DMA_NXP_EDMA_H

#include "dma.h"

struct DMA_NXP_EDMA;

#define DMA_NXP_EDMA_CHANNEL_COUNT 64

struct dma_nxp_edma {
    /*@temp@*/ struct DMA_NXP_EDMA *base;
};

int dma_nxp_edma_init(/*@out@*/ struct dma_nxp_edma *ctx, int base);

struct dma {
    /*@temp@*/ struct dma_nxp_edma *parent;
    size_t channel;
};

int dma_nxp_edma_dma_init(/*@out@*/ struct dma *ctx, struct dma_nxp_edma *parent, size_t channel);

/* Runtime calls:
 * int dma_setup(struct dma *ctx, struct dma_xfer *xfer);
 * int dma_xfer(struct dma *ctx, struct dma_xfer *xfer);
 * int dma_xfer_done(struct dma *ctx);
 */

#endif

#ifndef DMA_RP2040_H
#define DMA_RP2040_H

#include "dma.h"

struct DMA_RP2040;
struct DMA_RP2040_CH;

#define DMA_RP2040_CHANNEL_COUNT 12

struct dma {
    /*@temp@*/ struct DMA_RP2040 *base;
    /*@temp@*/ struct DMA_RP2040_CH *ch;
};

int dma_r2040_init(/*@out@*/ struct dma *ctx, struct DMA_RP2040 *base, size_t channel);

/* Runtime calls:
 * int dma_setup((struct dma *ctx, struct dma_xfer *xfer);
 * int dma_xfer(struct dma *ctx, struct dma_xfer *xfer);
 * int dma_xfer_done(struct dma *ctx);
 */

#endif

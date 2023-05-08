#ifndef DMA_GD32VF103_H
#define DMA_GD32VF103_H

#include "dma.h"
#include <stddef.h>

struct DMA_GD32VF103;
struct DMA_GD32VF103_CH;

#define DMA_GD32VF103_MAX_CH_COUNT 7 /* DMA0, DMA1 has only 5 */

struct dma {
    /*@temp@*/ struct DMA_GD32VF103 *base;
    /*@temp@*/ struct DMA_GD32VF103_CH *ch;
    size_t channel;
};

int dma_gd32vf103_init(/*@out@*/ struct dma *ctx, int base, size_t channel);

/* Runtime calls:
 * int dma_setup((struct dma *ctx, struct dma_xfer *xfer);
 * int dma_xfer(struct dma *ctx, struct dma_xfer *xfer);
 * int dma_xfer_done(struct dma *ctx);
 */

#endif

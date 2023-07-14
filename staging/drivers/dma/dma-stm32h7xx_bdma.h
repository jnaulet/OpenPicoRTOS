#ifndef DMA_STM32H7XX_BDMA_H
#define DMA_STM32H7XX_BDMA_H

#include "dma.h"

#define DMA_STM32H7XX_BDMA_CHANNEL_COUNT 8

struct DMA_STM32H7XX_BDMA;

struct dma {
    /*@temp@*/ struct DMA_STM32H7XX_BDMA *base;
    size_t channel;
};

int dma_stm32h7xx_bdma_init(/*@out@*/ struct dma *ctx, int base, size_t channel);

/* Runtime calls:
 * int dma_setup(struct dma *ctx, struct dma_xfer *xfer);
 * int dma_xfer(struct dma *ctx, struct dma_xfer *xfer);
 * int dma_xfer_done(struct dma *ctx);
 */

#endif

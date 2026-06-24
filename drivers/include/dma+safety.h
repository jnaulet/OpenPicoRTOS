#ifndef HAL_DMA_SAFETY_H
#define HAL_DMA_SAFETY_H

#include "dma.h"

/**
 * struct dma \***dma_claim**(**struct dma** \*<ins>ctx</ins>);
 * > Claims the dma for this process/thread (opt. MPU-related feature)
 */
struct dma *dma_claim(/*@returned@*/ struct dma *ctx);

#endif

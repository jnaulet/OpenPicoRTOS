#ifndef HAL_DMA_SAFETY_H
#define HAL_DMA_SAFETY_H

#include "dma.h"

/* Function: dma_claim
 * Claims the dma for this process/thread (opt. MPU-related feature)
 *
 * Parameters:
 *  dma - A dma instance
 */
struct dma *dma_claim(/*@returned@*/ struct dma *ctx);

#endif

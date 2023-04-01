#ifndef HAL_DMA_H
#define HAL_DMA_H

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct dma;

struct dma_xfer {
    intptr_t saddr;     /* source address */
    intptr_t daddr;     /* destination address */
    /* attributes */
    bool incr_read;     /* increment read address after bus xfer */
    bool incr_write;    /* increment write address after bus xfer */
    size_t size;        /* size of bus xfer (0-4 usually) */
    size_t byte_count;  /* byte count */
};

/* Function: dma_xfer
 * Starts a DMA transfer
 *
 * Parameters:
 *  ctx - The DMA channel to operate
 *  xfer - The transfer parameters
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int dma_xfer(struct dma *ctx, struct dma_xfer *xfer);

/* Function: dma_xfer_done
 * Polls a DMA for end of transfer
 *
 * Parameters:
 *  ctx - The DMA to poll
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int dma_xfer_done(struct dma *ctx);

#endif

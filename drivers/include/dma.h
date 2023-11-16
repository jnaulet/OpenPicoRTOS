#ifndef HAL_DMA_H
#define HAL_DMA_H

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct dma;

typedef enum {
    DMA_XFER_INCREMENT_OFF,
    DMA_XFER_INCREMENT_ON,
    DMA_XFER_INCREMENT_COUNT
} dma_xfer_increment_t;

struct dma_xfer {
    intptr_t saddr;                     /* source address */
    intptr_t daddr;                     /* destination address */
    /* attributes */
    dma_xfer_increment_t incr_read;     /* increment read address after bus xfer */
    dma_xfer_increment_t incr_write;    /* increment write address after bus xfer */
    size_t size;                        /* size of bus xfer (0-4 usually) */
    size_t byte_count;                  /* byte count */
};

/* Function: dma_setup
 * Configures a DMA for external trigger
 *
 * Parameters:
 *  ctx - The DMA channel to configure
 *  xfer - The transfer parameters
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int dma_setup(struct dma *ctx, struct dma_xfer *xfer);

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

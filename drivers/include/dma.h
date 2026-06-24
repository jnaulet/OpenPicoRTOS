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

/**
 * int **dma_setup**(**struct dma** \*<ins>ctx</ins>, const **struct dma_xfer** \*<ins>xfer</ins>);
 * > Configures a DMA channel <ins>ctx</ins> for external trigger
 *
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int dma_setup(struct dma *ctx, const struct dma_xfer *xfer);

/**
 * int **dma_xfer**(**struct dma** \*<ins>ctx</ins>, const **struct dma_xfer** \*<ins>xfer</ins>);
 * > Manually starts a DMA transfer <ins>xfer</ins>
 *
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int dma_xfer(struct dma *ctx, const struct dma_xfer *xfer);

/**
 * int **dma_xfer_done**(**struct dma** \*<ins>ctx</ins>);
 * > Polls a DMA channel for end of transfer
 *
 * ### RETURN
 * Returns 0 in case of xfer success, -errno otherwise
 */
int dma_xfer_done(struct dma *ctx);

#endif

#ifndef DMA_H
#define DMA_H

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

struct dma_channel;

struct dma_channel_xfer {
    intptr_t saddr;     /* source address */
    intptr_t daddr;     /* destination address */
    /* attributes */
    bool incr_read;     /* increment read address after bus xfer */
    bool incr_write;    /* increment write address after bus xfer */
    size_t size;        /* size of bus xfer (0-4 usually) */
    size_t byte_count;  /* byte count */
};

int dma_channel_xfer(struct dma_channel *ctx, struct dma_channel_xfer *xfer);
int dma_channel_xfer_done(struct dma_channel *ctx);

#endif

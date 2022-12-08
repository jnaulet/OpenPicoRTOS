#ifndef DMA_SG_H
#define DMA_SG_H

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

struct dma_sg_channel;

struct dma_sg_channel_xfer {
    /* source address */
    intptr_t saddr;
    /* attributes */
    size_t smod;        /* source address modulo */
    size_t ssize;       /* source data size in bytes */
    size_t dmod;        /* destination address modulo */
    size_t dsize;       /* destination data size in bytes */
    int soff;           /* signed source address offset */
    size_t byte_count;  /* byte count */
    int slast;          /* last source adjustment */
    intptr_t daddr;     /* destination address */
    int doff;           /* signed destination address offset */
    int dlastsga;       /* last destination adjustment scatter-gather */
};

int dma_sg_channel_xfer(struct dma_sg_channel *ctx, struct dma_sg_channel_xfer *xfer);
int dma_sg_channel_xfer_done(struct dma_sg_channel *ctx);

#endif

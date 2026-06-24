#ifndef HAL_DMASG_H
#define HAL_DMASG_H

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

struct dmasg;

struct dmasg_xfer {
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

/**
 * int **dmasg_xfer**(**struct dmasg** \*<ins>ctx</ins>, 
 * const **struct dmasg_xfer** \*<ins>xfer<ins>);
 * > Starts a DMA scatter-gather transfer <ins>xfer</ins>
 *
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int dmasg_xfer(struct dmasg *ctx, const struct dmasg_xfer *xfer);

/**
 * int **dmasg_xfer_done**(**struct dmasg** \*<ins>ctx</ins>);
 * > Polls a DMA-SG for end of transfer
 *
 * ### RETURN
 * > Returns 0 in case of successful xfer, -errno otherwise
 */
int dmasg_xfer_done(struct dmasg *ctx);

#endif

#ifndef DMASG_NXP_EDMA_H
#define DMASG_NXP_EDMA_H

#include "dmasg.h"

#define DMASG_NXP_EDMA_CHANNEL_COUNT 64

struct DMASG_NXP_EDMA;

struct dmasg_nxp_edma {
    /*@temp@*/ struct DMASG_NXP_EDMA *base;
};

int dmasg_nxp_edma_init(struct dmasg_nxp_edma *ctx, int base);

struct dmasg {
    /*@temp@*/ struct dmasg_nxp_edma *parent;
    size_t channel;
};

int dmasg_nxp_edma_dmasg_init(struct dmasg *ctx, struct dmasg_nxp_edma *parent, size_t channel);
int dmasg_nxp_edma_dmasg_setup(struct dmasg *ctx, struct dmasg_xfer *xfer);

/* Runtime calls:
 * int dmasg_xfer(struct dmasg *ctx, struct dmasg_xfer *xfer);
 * int dmasg_xfer_done(struct dmasg *ctx);
 */

#endif

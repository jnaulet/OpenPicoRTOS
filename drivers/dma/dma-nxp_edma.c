#include "dma-nxp_edma.h"
#include "picoRTOS.h"

struct DMA_NXP_EDMA {
    volatile uint32_t CR;
    volatile uint32_t ES;
    volatile uint32_t ERQH;
    volatile uint32_t ERQL;
    volatile uint32_t EEIH;
    volatile uint32_t EEIL;
    volatile uint8_t SERQ;
    volatile uint8_t CERQ;
    volatile uint8_t SEEI;
    volatile uint8_t CEEI;
    volatile uint8_t CINT;
    volatile uint8_t CERR;
    volatile uint8_t SSRT;
    volatile uint8_t CDNE;
    volatile uint32_t INTH;
    volatile uint32_t INTL;
    volatile uint32_t ERRH;
    volatile uint32_t ERRL;
    volatile uint32_t HRSH;
    volatile uint32_t HRSL;
    volatile uint32_t GPOR[2];
    uint32_t RESERVED0[48];
    volatile uint8_t DCHPRI[DMA_NXP_EDMA_CHANNEL_COUNT];
    uint32_t RESERVED1[944];
    struct {
        volatile uint32_t SADDR;
        volatile uint16_t ATTR;
        volatile uint16_t SOFF;
        volatile uint32_t NBYTES;
        volatile uint32_t SLAST;
        volatile uint32_t DADDR;
        volatile uint16_t CITER;
        volatile uint16_t DOFF;
        volatile uint32_t DLASTSGA;
        volatile uint16_t BITER;
        volatile uint16_t CSR;
    } TCD[DMA_NXP_EDMA_CHANNEL_COUNT];
};

#define CR_EMLM (1 << 7)
#define CR_ERGA (1 << 3)
#define CR_ERCA (1 << 2)

#define SERQ_SERQ_M  0x3fu
#define SERQ_SERQ(x) ((x) & SERQ_SERQ_M)

#define CERQ_CERQ_M  0x3fu
#define CERQ_CERQ(x) ((x) & CERQ_CERQ_M)

#define SSRT_SSRT_M  0x3fu
#define SSRT_SSRT(x) ((x) & SSRT_SSRT_M)

#define CDNE_CDNE_M  0x3fu
#define CDNE_CDNE(x) ((x) & CDNE_CDNE_M)

#define TCD_CSR_DONE   (1 << 7)
#define TCD_CSR_ACTIVE (1 << 6)

#define TCD_ATTR_SMOD_M   0x1fu
#define TCD_ATTR_SMOD(x)  (((x) & TCD_ATTR_SMOD_M) << 11)
#define TCD_ATTR_SSIZE_M  0x7u
#define TCD_ATTR_SSIZE(x) (((x) & TCD_ATTR_SSIZE_M) << 8)
#define TCD_ATTR_DMOD_M   0x1fu
#define TCD_ATTR_DMOD(x)  (((x) & TCD_ATTR_DMOD_M) << 3)
#define TCD_ATTR_DSIZE_M  0x7u
#define TCD_ATTR_DSIZE(x) ((x) & TCD_ATTR_DSIZE_M)

/* Function: dma_nxp_edma_init
 * Initialiases a NXP eDMA controller block
 *
 * Parameters:
 *  ctx - The eDMA to init
 *  base - The eDMA base address
 *
 * Returns:
 * always 0
 */
int dma_nxp_edma_init(struct dma_nxp_edma *ctx, int base)
{
    ctx->base = (struct DMA_NXP_EDMA*)base;

    /* round-robin */
    ctx->base->CR = (uint32_t)CR_ERCA;
    ctx->base->CR |= (uint32_t)CR_ERGA;

    /* minor loop mapping */
    ctx->base->CR |= (uint32_t)CR_EMLM;

    return 0;
}

/* Function: dma_nxp_edma_dma_init
 * Initialiases a NXP eDMA controller channel
 *
 * Parameters:
 *  ctx - The DMA channel to init
 *  parent - The eDMA channel parent controller
 *  channel - The channel number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int dma_nxp_edma_dma_init(struct dma *ctx, struct dma_nxp_edma *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)DMA_NXP_EDMA_CHANNEL_COUNT,
                    return -EINVAL);

    ctx->parent = parent;
    ctx->channel = channel;

    return 0;
}

static uint32_t nxp_edma_data_size(size_t size)
{
    switch (size) {
    case sizeof(uint8_t): return (uint32_t)0;
    case sizeof(uint16_t): return (uint32_t)1;
    case sizeof(uint32_t): return (uint32_t)2;
    case sizeof(uint64_t): return (uint32_t)3;
    default:
        picoRTOS_break();
        /*@notreached@*/
        break;
    }

    return (uint32_t)0;
}

static int setup_tcd_attr(struct dma *ctx, const struct dma_xfer *xfer)
{
    picoRTOS_assert(xfer->size <= sizeof(uint64_t), return -EINVAL);

    struct DMA_NXP_EDMA *base = ctx->parent->base;

    base->TCD[ctx->channel].ATTR = (uint32_t)(TCD_ATTR_SSIZE(nxp_edma_data_size(xfer->size)) |
                                              TCD_ATTR_DSIZE(nxp_edma_data_size(xfer->size)));

    return 0;
}

static int setup_tcd(struct dma *ctx, const struct dma_xfer *xfer)
{
    int res;
    struct DMA_NXP_EDMA *base = ctx->parent->base;

    /* start address */
    base->TCD[ctx->channel].SADDR = (uint32_t)xfer->saddr;
    /* attributes */
    if ((res = setup_tcd_attr(ctx, xfer)) < 0)
        return res;
    /* source offset & last source adjustment */
    if (xfer->incr_read != DMA_XFER_INCREMENT_OFF) {
        base->TCD[ctx->channel].SOFF = (uint16_t)xfer->size;
        base->TCD[ctx->channel].SLAST = (uint32_t)-(int)xfer->byte_count;
    }else{
        base->TCD[ctx->channel].SOFF = (uint16_t)0;
        base->TCD[ctx->channel].SLAST = (uint32_t)0;
    }
    /* minor byte count */
    base->TCD[ctx->channel].NBYTES = (uint32_t)xfer->byte_count;
    base->TCD[ctx->channel].CITER = (uint32_t)0x1;
    /* destination address */
    base->TCD[ctx->channel].DADDR = (uint32_t)xfer->daddr;
    /* signed destination address offset & last destination adjustment sga */
    if (xfer->incr_write != DMA_XFER_INCREMENT_OFF) {
        base->TCD[ctx->channel].DOFF = (uint16_t)xfer->size;
        base->TCD[ctx->channel].DLASTSGA = (uint32_t)-(int)xfer->byte_count;
    }else{
        base->TCD[ctx->channel].DOFF = (uint16_t)0;
        base->TCD[ctx->channel].DLASTSGA = (uint32_t)0;
    }
    /* last destination adjustment sga */
    base->TCD[ctx->channel].BITER = (uint32_t)0x1;

    return 0;
}

/* hooks */

int dma_setup(struct dma *ctx, const struct dma_xfer *xfer)
{
    int res;
    struct DMA_NXP_EDMA *base = ctx->parent->base;

    if ((res = setup_tcd(ctx, xfer)) < 0)
        return res;

    /* enable request register */
    base->SERQ = (uint32_t)SERQ_SERQ(ctx->channel);

    return 0;
}

int dma_xfer(struct dma *ctx, const struct dma_xfer *xfer)
{
    int res;
    struct DMA_NXP_EDMA *base = ctx->parent->base;

    if ((base->TCD[ctx->channel].CSR & TCD_CSR_ACTIVE) != 0)
        return -EAGAIN;

    /* clear done status bit */
    base->CDNE = (uint32_t)CDNE_CDNE(ctx->channel);

    if ((res = setup_tcd(ctx, xfer)) < 0)
        return res;

    /* start */
    base->SSRT = (uint32_t)SSRT_SSRT(ctx->channel);

    return 0;
}

int dma_xfer_done(struct dma *ctx)
{
    const struct DMA_NXP_EDMA *base = ctx->parent->base;

    if ((base->TCD[ctx->channel].CSR & TCD_CSR_ACTIVE) != 0)
        return -EAGAIN;

    return 0;
}

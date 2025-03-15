#include "dma-rp2040.h"
#include "picoRTOS.h"

struct DMA_RP2040 {
    struct DMA_RP2040_CH {
        volatile uint32_t READ_ADDR;
        volatile uint32_t WRITE_ADDR;
        volatile uint32_t TRANS_COUNT;
        volatile uint32_t CTRL_TRIG;
        volatile uint32_t AL1_CTRL;
        volatile uint32_t AL1_READ_ADDR;
        volatile uint32_t AL1_WRITE_ADDR;
        volatile uint32_t AL1_TRANS_COUNT_TRIG;
        volatile uint32_t AL2_CTRL;
        volatile uint32_t AL2_TRANS_COUNT;
        volatile uint32_t AL2_READ_ADDR;
        volatile uint32_t AL2_WRITE_ADDR_TRIG;
        volatile uint32_t AL3_CTRL;
        volatile uint32_t AL3_WRITE_ADDR;
        volatile uint32_t AL3_TRANS_COUNT;
        volatile uint32_t AL3_READ_ADDR_TRIG;
    } CH[DMA_RP2040_CHANNEL_COUNT];
    uint32_t RESERVED0[64];
    volatile uint32_t INTR;
    volatile uint32_t INTE0;
    volatile uint32_t INTF0;
    volatile uint32_t INTS0;
    volatile uint32_t INTE1;
    volatile uint32_t INTF1;
    volatile uint32_t INTS1;
    volatile uint32_t TIMER0;
    volatile uint32_t TIMER1;
    volatile uint32_t TIMER2;
    volatile uint32_t TIMER3;
    volatile uint32_t MULTI_CHAN_TRIGGER;
    volatile uint32_t SNIFF_CTRL;
    volatile uint32_t SNIFF_DATA;
    volatile uint32_t FIFO_LEVELS;
    volatile uint32_t CHAN_ABORT;
    volatile uint32_t N_CHANNELS;
    struct {
        volatile uint32_t CTDREQ;
        volatile uint32_t TCR;
    } CHn_DBG[DMA_RP2040_CHANNEL_COUNT];
};

#define CTRL_TRIG_BUSY         (1 << 24)
#define CTRL_TRIG_TREQ_SEL_M   0x3fu
#define CTRL_TRIG_TREQ_SEL(x)  (((x) & CTRL_TRIG_TREQ_SEL_M) << 15)
#define CTRL_TRIG_CHAIN_TO_M   0xfu
#define CTRL_TRIG_CHAIN_TO(x)  (((x) & CTRL_TRIG_CHAIN_TO_M) << 11)
#define CTRL_TRIG_INCR_WRITE   (1 << 5)
#define CTRL_TRIG_INCR_READ    (1 << 4)
#define CTRL_TRIG_DATA_SIZE_M  0x3u
#define CTRL_TRIG_DATA_SIZE(x) (((x) & CTRL_TRIG_DATA_SIZE_M) << 2)
#define CTRL_TRIG_EN           (1 << 0)

/* Function: dma_r2040_init
 * Initializes a DMA channel
 *
 * Parameters:
 *  ctx - The DMA channel to init
 *  base - The DMA block base address
 *  channel - The DMA channel number
 *  treq_sel - The TREQ_SEL / DREQ associated with this channel
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int dma_r2040_init(struct dma *ctx, int base, size_t channel,
                   dma_rp2040_treq_sel_t treq_sel)
{
    picoRTOS_assert(channel < (size_t)DMA_RP2040_CHANNEL_COUNT, return -EINVAL);
    picoRTOS_assert(treq_sel < DMA_RP2040_TREQ_SEL_COUNT, return -EINVAL);

    ctx->base = (struct DMA_RP2040*)base;
    ctx->ch = &ctx->base->CH[channel];
    ctx->treq_sel = treq_sel;
    ctx->CTRL_TRIG = 0;

    /* disable channel */
    ctx->ch->CTRL_TRIG &= ~CTRL_TRIG_EN;

    /* data req */
    ctx->ch->CTRL_TRIG &= ~CTRL_TRIG_TREQ_SEL(CTRL_TRIG_TREQ_SEL_M);
    ctx->ch->CTRL_TRIG |= CTRL_TRIG_TREQ_SEL(treq_sel);

    /* disable chain to */
    ctx->ch->CTRL_TRIG &= ~CTRL_TRIG_CHAIN_TO(CTRL_TRIG_CHAIN_TO_M);
    ctx->ch->CTRL_TRIG |= CTRL_TRIG_CHAIN_TO(channel);

    return 0;
}

/* hooks */

int dma_setup(struct dma *ctx, const struct dma_xfer *xfer)
{
    picoRTOS_assert(xfer->size > 0, return -EINVAL);
    picoRTOS_assert(xfer->size != (size_t)3, return -EINVAL);
    picoRTOS_assert(xfer->size < (size_t)5, return -EINVAL);
    picoRTOS_assert(xfer->incr_read <= DMA_XFER_INCREMENT_COUNT, return -EINVAL);
    picoRTOS_assert(xfer->incr_write <= DMA_XFER_INCREMENT_COUNT, return -EINVAL);

    ctx->ch->READ_ADDR = (uint32_t)xfer->saddr;
    ctx->ch->WRITE_ADDR = (uint32_t)xfer->daddr;
    ctx->ch->TRANS_COUNT = (uint32_t)(xfer->byte_count / xfer->size);

    if (xfer->incr_read != DMA_XFER_INCREMENT_OFF) ctx->ch->CTRL_TRIG |= CTRL_TRIG_INCR_READ;
    else ctx->ch->CTRL_TRIG &= ~CTRL_TRIG_INCR_READ;

    if (xfer->incr_write != DMA_XFER_INCREMENT_OFF) ctx->ch->CTRL_TRIG |= CTRL_TRIG_INCR_WRITE;
    else ctx->ch->CTRL_TRIG &= ~CTRL_TRIG_INCR_WRITE;

    /* xfer size, default to byte */
    ctx->ch->CTRL_TRIG &= ~CTRL_TRIG_DATA_SIZE(CTRL_TRIG_DATA_SIZE_M);
    if (xfer->size == (size_t)2) ctx->ch->CTRL_TRIG |= CTRL_TRIG_DATA_SIZE(0x1);
    if (xfer->size == (size_t)4) ctx->ch->CTRL_TRIG |= CTRL_TRIG_DATA_SIZE(0x2);

    ctx->ch->CTRL_TRIG |= CTRL_TRIG_EN;
    return 0;
}

int dma_xfer(struct dma *ctx, const struct dma_xfer *xfer)
{
    picoRTOS_assert(xfer->size > 0, return -EINVAL);
    picoRTOS_assert(xfer->size != (size_t)3, return -EINVAL);
    picoRTOS_assert(xfer->size < (size_t)5, return -EINVAL);

    int res;

    if ((res = dma_setup(ctx, xfer)) < 0)
        return res;

    /* trig permanently */
    ctx->ch->CTRL_TRIG |= CTRL_TRIG_TREQ_SEL(0x3f);

    return 0;
}

int dma_xfer_done(struct dma *ctx)
{
    if ((ctx->ch->CTRL_TRIG & CTRL_TRIG_BUSY) != 0)
        return -EAGAIN;

    /* disable channel */
    ctx->ch->CTRL_TRIG &= ~CTRL_TRIG_EN;
    return 0;
}

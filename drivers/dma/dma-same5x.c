#include "dma-same5x.h"
#include "picoRTOS.h"

struct DMA_SAME5X {
    volatile uint16_t CTRL;
    volatile uint16_t CRCCTRL;
    volatile uint32_t CRCDATAIN;
    volatile uint32_t CRCCHKSUM;
    volatile uint8_t CRCSTATUS;
    volatile uint8_t DBGCTRL;
    uint16_t RESERVED0;
    volatile uint32_t SWTRIGCTRL;
    volatile uint32_t PRICTRL0;
    uint32_t RESERVED1[2];
    volatile uint16_t INTPEND;
    uint16_t RESERVED2;
    volatile uint32_t INTSTATUS;
    volatile uint32_t BUSYCH;
    volatile uint32_t PENDCH;
    volatile uint32_t ACTIVE;
    volatile uint32_t BASEADDR;
    volatile uint32_t WRBADDR;
    uint32_t RESERVED3;
    struct DMA_SAME5X_CH {
        volatile uint32_t CHCTRLA;
        volatile uint8_t CHCTRLB;
        volatile uint8_t CHPRILVL;
        volatile uint8_t CHEVCTRL;
        uint8_t RESERVED[5];
        volatile uint8_t CHINTENCLR;
        volatile uint8_t CHINTENSET;
        volatile uint8_t CHINTFLAG;
        volatile uint8_t CHSTATUS;
    } CH[DMA_SAME5X_CHANNEL_COUNT];
};

#define CTRL_LVLEN3    (1 << 11)
#define CTRL_LVLEN2    (1 << 10)
#define CTRL_LVLEN1    (1 << 9)
#define CTRL_LVLEN0    (1 << 8)
#define CTRL_DMAENABLE (1 << 1)
#define CTRL_SWRST     (1 << 0)

/* TODO: CRC */

#define PRICTRL0_RRLVLEN3   (1u << 31)
#define PRICTRL0_QOS3_M     0x3u
#define PRICTRL0_QOS3(x)    (((x) & PRICTRL0_QOS3_M) << 29)
#define PRICTRL0_LVLPRI3_M  0x1fu
#define PRICTRL0_LVLPRI3(x) (((x) & PRICTRL0_LVLPRI3_M) << 24)
#define PRICTRL0_RRLVLEN2   (1u << 31)
#define PRICTRL0_QOS2_M     0x3u
#define PRICTRL0_QOS2(x)    (((x) & PRICTRL0_QOS2_M) << 21)
#define PRICTRL0_LVLPRI2_M  0x1fu
#define PRICTRL0_LVLPRI2(x) (((x) & PRICTRL0_LVLPRI2_M) << 16)
#define PRICTRL0_RRLVLEN1   (1u << 31)
#define PRICTRL0_QOS1_M     0x3u
#define PRICTRL0_QOS1(x)    (((x) & PRICTRL0_QOS1_M) << 13)
#define PRICTRL0_LVLPRI1_M  0x1fu
#define PRICTRL0_LVLPRI1(x) (((x) & PRICTRL0_LVLPRI1_M) << 8)
#define PRICTRL0_RRLVLEN0   (1u << 31)
#define PRICTRL0_QOS0_M     0x3u
#define PRICTRL0_QOS0(x)    (((x) & PRICTRL0_QOS0_M) << 5)
#define PRICTRL0_LVLPRI0_M  0x1fu
#define PRICTRL0_LVLPRI0(x) ((x) & PRICTRL0_LVLPRI0_M)

#define INTPEND_PEND   (1 << 15)
#define INTPEND_BUSY   (1 << 14)
#define INTPEND_FERR   (1 << 13)
#define INTPEND_CRCERR (1 << 12)
#define INTPEND_SUSP   (1 << 10)
#define INTPEND_TCMPL  (1 << 9)
#define INTPEND_TERR   (1 << 8)
#define INTPEND_ID_M   0x1fu
#define INTPEND_ID(x)  ((x) & INTPEND_ID_M)

#define ACTIVE_BTCNT_M  0xffffu
#define ACTIVE_BTCNT(x) (((x) & ACTIVE_BTCNT_M) << 16)
#define ACTIVE_ABUSY    (1 << 15)
#define ACTIVE_ID_M     0x1fu
#define ACTIVE_ID(x)    (((x) & ACTIVE_ID_M) << 8)
#define ACTIVE_LVLEX3   (1 << 3)
#define ACTIVE_LVLEX2   (1 << 2)
#define ACTIVE_LVLEX1   (1 << 1)
#define ACTIVE_LVLEX0   (1 << 0)

/* CHANNELS */

#define CHCTRLA_THRESHOLD_M  0x3u
#define CHCTRLA_THRESHOLD(x) (((x) & CHCTRLA_THRESHOLD_M) << 28)
#define CHCTRLA_BURSTLEN_M   0xfu
#define CHCTRLA_BURSTLEN(x)  (((x) & CHCTRLA_BURSTLEN_M) << 24)
#define CHCTRLA_TRIGACT_M    0x3u
#define CHCTRLA_TRIGACT(x)   (((x) & CHCTRLA_TRIGACT_M) << 20)
#define CHCTRLA_TRIGSRC_M    0x7fu
#define CHCTRLA_TRIGSRC(x)   (((x) & CHCTRLA_TRIGSRC_M) << 8)
#define CHCTRLA_RUNSTDBY     (1 << 6)
#define CHCTRLA_ENABLE       (1 << 1)
#define CHCTRLA_SWRST        (1 << 0)

#define CHCTRLB_CMD_M  0x3u
#define CHCTRLB_CMD(x) ((x) & CHCTRLB_CMD_M)

#define CHPRILVL_PRILVL_M  0x3u
#define CHPRILVL_PRILVL(x) ((x) & CHPRILVL_PRILVL_M)

#define CHEVCTRL_EVOE       (1 << 7)
#define CHEVCTRL_EVIE       (1 << 6)
#define CHEVCTRL_EVOMODE_M  0x3u
#define CHEVCTRL_EVOMODE(x) (((x) & CHEVCTRL_EVOMODE_M) << 4)
#define CHEVCTRL_EVACT_M    0x7u
#define CHEVCTRL_EVACT(x)   ((x) & CHEVCTRL_EVACT_M)

#define CHINTFLAG_SUSP  (1 << 2)
#define CHINTFLAG_TCMPL (1 << 1)
#define CHINTFLAG_TERR  (1 << 0)

#define CHSTATUS_CRCERR (1 << 3)
#define CHSTATUS_FERR   (1 << 2)
#define CHSTATUS_BUSY   (1 << 1)
#define CHSTATUS_PEND   (1 << 0)

/* DMA_SAME5x_SRAM */

#define BTCTRL_STEPSIZE_M  0x7u
#define BTCTRL_STEPSIZE(x) (((x) & BTCTRL_STEPSIZE_M) << 13)
#define BTCTRL_STEPSEL     (1 << 12)
#define BTCTRL_DSTINC      (1 << 11)
#define BTCTRL_SRCINC      (1 << 10)
#define BTCTRL_BEATSIZE_M  0x3u
#define BTCTRL_BEATSIZE(x) (((x) & BTCTRL_BEATSIZE_M) << 8)
#define BTCTRL_BLOCKACT_M  0x3u
#define BTCTRL_BLOCKACT(x) (((x) & BTCTRL_BLOCKACT_M) << 3)
#define BTCTRL_EVOSEL_M    0x3u
#define BTCTRL_EVOSEL(x)   (((x) & BTCTRL_EVOSEL_M) << 1)
#define BTCTRL_VALID       (1 << 0)

/* Function: dma_same5x_init
 * Initialiases a SAME5x DMAC controller
 *
 * Parameters:
 *  ctx - The DMA channel to init
 *  base - The DMAC base address
 *
 * Returns:
 * always 0
 */
int dma_same5x_init(struct dma_same5x *ctx, int base)
{
    ctx->base = (struct DMA_SAME5X*)base;

    /* config TDs & turn on */
    ctx->base->BASEADDR = (uint32_t)ctx->TD;
    ctx->base->WRBADDR = (uint32_t)ctx->WB;

    ctx->base->CTRL = (uint16_t)(CTRL_LVLEN3 | CTRL_LVLEN2 |
                                 CTRL_LVLEN1 | CTRL_LVLEN0 |
                                 CTRL_DMAENABLE);

    return 0;
}

/* Function: dma_same5x_dma_init
 * Initialiases a SAME5x DMAC controller channel
 *
 * Parameters:
 *  ctx - The DMA channel to init
 *  parent - The DMAC channel parent controller
 *  channel - The channel number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int dma_same5x_dma_init(/*@out@*/ struct dma *ctx, struct dma_same5x *parent,
                        size_t channel, dma_same5x_trigsrc_t trigsrc)
{
    if (!picoRTOS_assert(channel < (size_t)DMA_SAME5X_CHANNEL_COUNT)) return -EINVAL;

    ctx->parent = parent;
    ctx->channel = channel;
    ctx->ch = &parent->base->CH[channel];

    /* set trigsrc */
    ctx->ch->CHCTRLA = (uint32_t)(CHCTRLA_TRIGACT(0x2) |
                                  CHCTRLA_TRIGSRC(trigsrc));
    /* priority level */
    ctx->ch->CHPRILVL = (uint32_t)CHPRILVL_PRILVL(0);

    return 0;
}

int dma_setup(struct dma *ctx, struct dma_xfer *xfer)
{
    if (!picoRTOS_assert(xfer->size > 0)) return -EINVAL;
    if (!picoRTOS_assert(xfer->size <= sizeof(uint32_t))) return -EINVAL;

    struct DMA_SAME5X_TD *TD = &ctx->parent->TD[ctx->channel];

    TD->BTCTRL = (uint16_t)(BTCTRL_STEPSIZE(0) |
                            (xfer->incr_write ? BTCTRL_DSTINC : 0) |
                            (xfer->incr_read ? BTCTRL_SRCINC : 0) |
                            BTCTRL_BEATSIZE(xfer->size >> 1) |
                            BTCTRL_BLOCKACT(0) |
                            BTCTRL_EVOSEL(0) |
                            BTCTRL_VALID);

    TD->BTCNT = (uint32_t)(xfer->byte_count / xfer->size);
    /* SCRADDR computation */
    if (!xfer->incr_read) TD->SRCADDR = (uint32_t)xfer->saddr;
    else TD->SRCADDR = (uint32_t)xfer->saddr + xfer->byte_count;
    /* DSTADDR computation */
    if (!xfer->incr_write) TD->DSTADDR = (uint32_t)xfer->daddr;
    else TD->DSTADDR = (uint32_t)xfer->daddr + xfer->byte_count;
    /* no linked xfer */
    TD->DESCADDR = (uint32_t)0;

    /* clear complete flag & start */
    ctx->ch->CHINTFLAG |= CHINTFLAG_TCMPL;
    ctx->ch->CHCTRLA |= CHCTRLA_ENABLE;

    return 0;
}

int dma_xfer(struct dma *ctx, struct dma_xfer *xfer)
{
    int res;
    struct DMA_SAME5X *base = ctx->parent->base;

    if ((res = dma_setup(ctx, xfer)) < 0)
        return res;

    /* soft trigger */
    base->SWTRIGCTRL = (uint32_t)1 << ctx->channel;
    return 0;
}

int dma_xfer_done(struct dma *ctx)
{
    if ((ctx->ch->CHINTFLAG & CHINTFLAG_TCMPL) == 0)
        return -EAGAIN;

    return 0;
}

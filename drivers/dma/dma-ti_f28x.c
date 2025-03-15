#include "dma-ti_f28x.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct DMA_REGS {
    volatile uint16_t DMACTRL;
    volatile uint16_t DEBUGCTRL;
    uint16_t RESERVED0[2];
    volatile uint16_t PRIORITYCTRL1;
    uint16_t RESERVED1;
    volatile uint16_t PRIORITYSTAT;
    uint16_t RESERVED2[25];
    struct DMA_CH_REGS {
        volatile uint16_t MODE;
        volatile uint16_t CONTROL;
        volatile uint16_t BURST_SIZE;
        volatile uint16_t BURST_COUNT;
        volatile uint16_t SRC_BURST_STEP;
        volatile uint16_t DST_BURST_STEP;
        volatile uint16_t TRANSFER_SIZE;
        volatile uint16_t TRANSFER_COUNT;
        volatile uint16_t SRC_TRANSFER_STEP;
        volatile uint16_t DST_TRANSFER_STEP;
        volatile uint16_t SRC_WRAP_SIZE;
        volatile uint16_t SRC_WRAP_COUNT;
        volatile uint16_t SRC_WRAP_STEP;
        volatile uint16_t DST_WRAP_SIZE;
        volatile uint16_t DST_WRAP_COUNT;
        volatile uint16_t DST_WRAP_STEP;
        volatile uint32_t SRC_BEG_ADDR_SHADOW;
        volatile uint32_t SRC_ADDR_SHADOW;
        volatile uint32_t SRC_BEG_ADDR_ACTIVE;
        volatile uint32_t SRC_ADDR_ACTIVE;
        volatile uint32_t DST_BEG_ADDR_SHADOW;
        volatile uint32_t DST_ADDR_SHADOW;
        volatile uint32_t DST_BEG_ADDR_ACTIVE;
        volatile uint32_t DST_ADDR_ACTIVE;
    } CH[DMA_TI_F28X_CHANNEL_COUNT];
};

#define DMACTRL_PRIORITYRESET (1 << 1)
#define DMACTRL_HARDRESET     (1 << 0)

#define DEBUGCTRL_FREE (1 << 15)

#define PRIORITYCTRL1_CH1PRIORITY (1 << 0)

#define PRIORITYSTAT_ACTIVESTS_SHADOW_M  0x7u
#define PRIORITYSTAT_ACTIVESTS_SHADOW(x) (((x) & PRIORITYSTAT_ACTIVESTS_SHADOW_M) << 4)
#define PRIORITYSTAT_ACTIVSTS_M          0x7u
#define PRIORITYSTAT_ACTIVSTS(x)         ((x) & PRIORITYSTAT_ACTIVSTS_M)

#define MODE_CHINTE       (1 << 15)
#define MODE_DATASIZE     (1 << 14)
#define MODE_CONTINUOUS   (1 << 11)
#define MODE_ONESHOT      (1 << 10)
#define MODE_CHINTMODE    (1 << 9)
#define MODE_PERINTE      (1 << 8)
#define MODE_OVRINTE      (1 << 7)
#define MODE_PERINTSEL_M  0x1fu
#define MODE_PERINTSEL(x) ((x) & MODE_PERINTSEL_M)

#define CONTROL_OVRFLG      (1 << 14)
#define CONTROL_RUNSTS      (1 << 13)
#define CONTROL_BURSTSTS    (1 << 12)
#define CONTROL_TRANSFERSTS (1 << 11)
#define CONTROL_PERINTFLG   (1 << 8)
#define CONTROL_ERRCLR      (1 << 7)
#define CONTROL_PERINTCLR   (1 << 4)
#define CONTROL_PERINTFRC   (1 << 3)
#define CONTROL_SOFTRESET   (1 << 2)
#define CONTROL_HALT        (1 << 1)
#define CONTROL_RUN         (1 << 0)

#define BURST_SIZE_BURSTSIZE_M  0x1fu
#define BURST_SIZE_BURSTSIZE(x) ((x) & BURST_SIZE_BURSTSIZE_M)

#define BURST_COUNT_BURSTCOUNT_M  0x1fu
#define BURST_COUNT_BURSTCOUNT(x) ((x) & BURST_COUNT_BURSTCOUNT_M)

struct DMA_CLA_SRC_SEL_REGS {
    volatile uint32_t CLA1TASKSRCSELLOCK;
    uint32_t RESERVED0;
    volatile uint32_t DMACHSRCSELLOCK;
    volatile uint32_t CLA1TASKSRCSELn[2];
    uint16_t RESERVED1[11];
    volatile uint32_t DMACHSRCSELn[2];
};

#define CLA1TASKSRCSELLOCK_CLA1TASKSRCSELLOCK (1 << 1)
#define CLA1TASKSRCSELLOCK_CLA1TASKSRCSEL1    (1 << 0)

#define DMACHSRCSELLOCK_DMACHSRCSEL2 (1 << 1)
#define DMACHSRCSELLOCK_DMACHSRCSEL1 (1 << 0)

#define CLA1TASKSRCSEL1_TASK1_M  0xffu
#define CLA1TASKSRCSEL1_TASK1(x) ((x) & CLA1TASKSRCSEL1_TASK1_M)

#define DMACHSRCSEL1_CH1_M  0xffu
#define DMACHSRCSEL1_CH1(x) ((x) & DMACHSRCSEL1_CH1_M)

static struct DMA_CLA_SRC_SEL_REGS *DMA_CLA_SRC_SEL =
    (struct DMA_CLA_SRC_SEL_REGS*)ADDR_DMA_CLA_SRC_SEL;

/* Function: dma_ti_f28x_init
 * Inits a DMA channel
 *
 * Parameters:
 *  ctx - The DMA channel to init
 *  base - The DMA controller address
 *  channel - The DMA channel number (-1)
 *  chsrsel - The DMA channel source trigger selection
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int dma_ti_f28x_init(struct dma *ctx, int base, size_t channel, dma_ti_f28x_chsrcsel_t chsrcsel)
{
    picoRTOS_assert(channel < (size_t)DMA_TI_F28X_CHANNEL_COUNT, return -EINVAL);
    picoRTOS_assert(chsrcsel < DMA_TI_F28X_CHSRCSEL_COUNT, return -EINVAL);

    size_t index = channel >> 2;
    size_t lshift = (channel & 0x3) * 8;

    ctx->channel = channel;
    ctx->base = (struct DMA_REGS*)base;
    ctx->ch = &ctx->base->CH[ctx->channel];

    ASM(" eallow");

    /* legacy bits */
    ctx->ch->MODE &= ~MODE_PERINTSEL(MODE_PERINTSEL_M);
    ctx->ch->MODE |= MODE_PERINTE | MODE_PERINTSEL(channel + 1);

    /* clear flags */
    ctx->ch->CONTROL |= (CONTROL_PERINTCLR | CONTROL_ERRCLR);

    /* CLA src */
    DMA_CLA_SRC_SEL->DMACHSRCSELn[index] &= ~(DMACHSRCSEL1_CH1(DMACHSRCSEL1_CH1_M) << lshift);
    DMA_CLA_SRC_SEL->DMACHSRCSELn[index] |= DMACHSRCSEL1_CH1(chsrcsel) << lshift;

    ASM(" edis");
    return 0;
}

int dma_setup(struct dma *ctx, const struct dma_xfer *xfer)
{
    picoRTOS_assert(xfer->size >= sizeof(uint16_t), return -EINVAL);
    picoRTOS_assert(xfer->size <= sizeof(uint32_t), return -EINVAL);
    picoRTOS_assert(xfer->incr_read <= DMA_XFER_INCREMENT_COUNT, return -EINVAL);
    picoRTOS_assert(xfer->incr_write <= DMA_XFER_INCREMENT_COUNT, return -EINVAL);

    ASM(" eallow");

    /* 16/32 */
    if (xfer->size == sizeof(uint32_t)) ctx->ch->MODE |= MODE_DATASIZE;
    else ctx->ch->MODE &= ~MODE_DATASIZE;
    /* burst */
    ctx->ch->BURST_SIZE = (uint16_t)0;
    ctx->ch->SRC_BURST_STEP = (uint16_t)(xfer->incr_read != DMA_XFER_INCREMENT_OFF ? xfer->size : 0);
    ctx->ch->DST_BURST_STEP = (uint16_t)(xfer->incr_write != DMA_XFER_INCREMENT_OFF ? xfer->size : 0);
    /* transfer */
    ctx->ch->TRANSFER_SIZE = (uint16_t)(xfer->byte_count / xfer->size) - 1;
    ctx->ch->SRC_TRANSFER_STEP = (uint16_t)(xfer->incr_read != DMA_XFER_INCREMENT_OFF ? xfer->size : 0);
    ctx->ch->DST_TRANSFER_STEP = (uint16_t)(xfer->incr_write != DMA_XFER_INCREMENT_OFF ? xfer->size : 0);
    /* addresses */
    ctx->ch->SRC_ADDR_SHADOW = (uint32_t)xfer->saddr;
    ctx->ch->SRC_BEG_ADDR_SHADOW = (uint32_t)xfer->saddr;
    ctx->ch->DST_ADDR_SHADOW = (uint32_t)xfer->daddr;
    ctx->ch->DST_BEG_ADDR_SHADOW = (uint32_t)xfer->daddr;

    /* run */
    ctx->ch->CONTROL |= CONTROL_RUN;

    ASM(" edis");
    return 0;
}

int dma_xfer(struct dma *ctx, const struct dma_xfer *xfer)
{
    int res;

    if ((res = dma_setup(ctx, xfer)) < 0)
        return res;

    /* TODO: soft trigger ? */
    return 0;
}

int dma_xfer_done(struct dma *ctx)
{
    if ((ctx->ch->CONTROL & CONTROL_TRANSFERSTS) != 0)
        return -EAGAIN;

    ASM(" eallow");
    ctx->ch->CONTROL |= (CONTROL_HALT | CONTROL_PERINTCLR | CONTROL_ERRCLR);
    ASM(" edis");

    return 0;
}

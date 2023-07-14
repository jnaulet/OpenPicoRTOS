#include "dma-stm32h7xx.h"
#include "picoRTOS.h"

#include <stdint.h>

struct DMA_STM32H7XX {
    volatile uint32_t LISR;
    volatile uint32_t HISR;
    volatile uint32_t LIFCR;
    volatile uint32_t HIFCR;
    struct DMA_STM32H7XX_CH {
        volatile uint32_t SxCR;
        volatile uint32_t SxNDTR;
        volatile uint32_t SxPAR;
        volatile uint32_t SxM0AR;
        volatile uint32_t SxM1AR;
        volatile uint32_t SxFCR;
    } CH[DMA_STM32H7XX_CHANNEL_COUNT];
};

#define LISR_TCIF3  (1 << 27)
#define LISR_HTIF3  (1 << 26)
#define LISR_TEIF3  (1 << 25)
#define LISR_DMEIF3 (1 << 24)
#define LISR_FEIF3  (1 << 22)
#define LISR_TCIF2  (1 << 21)
#define LISR_HTIF2  (1 << 20)
#define LISR_TEIF2  (1 << 19)
#define LISR_DMEIF2 (1 << 18)
#define LISR_FEIF2  (1 << 16)
#define LISR_TCIF1  (1 << 11)
#define LISR_HTIF1  (1 << 10)
#define LISR_TEIF1  (1 << 9)
#define LISR_DMEIF1 (1 << 8)
#define LISR_FEIF1  (1 << 6)
#define LISR_TCIF0  (1 << 5)
#define LISR_HTIF0  (1 << 4)
#define LISR_TEIF0  (1 << 3)
#define LISR_DMEIF0 (1 << 2)
#define LISR_FEIF0  (1 << 0)

#define HISR_TCIF7  (1 << 27)
#define HISR_HITF7  (1 << 26)
#define HISR_TEIF7  (1 << 25)
#define HISR_DMEIF7 (1 << 24)
#define HISR_FEIF7  (1 << 22)
#define HISR_TCIF6  (1 << 21)
#define HISR_HITF6  (1 << 20)
#define HISR_TEIF6  (1 << 19)
#define HISR_DMEIF6 (1 << 18)
#define HISR_FEIF6  (1 << 16)
#define HISR_TCIF5  (1 << 11)
#define HISR_HITF5  (1 << 10)
#define HISR_TEIF5  (1 << 9)
#define HISR_DMEIF5 (1 << 8)
#define HISR_FEIF5  (1 << 6)
#define HISR_TCIF4  (1 << 5)
#define HISR_HITF4  (1 << 4)
#define HISR_TEIF4  (1 << 3)
#define HISR_DMEIF4 (1 << 2)
#define HISR_FEIF4  (1 << 0)

#define SxCR_MBURST_M  0x3u
#define SxCR_MBURST(x) (((x) & SxCR_MBURST_M) << 23)
#define SxCR_PBURST_M  0x3u
#define SxCR_PBURST(x) (((x) & SxCR_PBURST_M) << 21)
#define SxCR_TRBUF     (1 << 20)
#define SxCR_CT        (1 << 19)
#define SxCR_DBM       (1 << 18)
#define SxCR_PL_M      0x3u
#define SxCR_PL(x)     (((x) & SxCR_PL_M) << 16)
#define SxCR_PINCOS    (1 << 15)
#define SxCR_MSIZE_M   0x3u
#define SxCR_MSIZE(x)  (((x) & SxCR_MSIZE_M) << 13)
#define SxCR_PSIZE_M   0x3u
#define SxCR_PSIZE(x)  (((x) & SxCR_PSIZE_M) << 11)
#define SxCR_MINC      (1 << 10)
#define SxCR_PINC      (1 << 9)
#define SxCR_CIRC      (1 << 8)
#define SxCR_DIR_M     0x3u
#define SxCR_DIR(x)    (((x) & SxCR_DIR_M) << 6)
#define SxCR_PFCTRL    (1 << 5)
#define SxCR_TCIE      (1 << 4)
#define SxCR_HTIE      (1 << 3)
#define SxCR_TEIE      (1 << 2)
#define SxCR_DMEIE     (1 << 1)
#define SxCR_EN        (1 << 0)

#define SxNDTR_NDT_M  0xffffu
#define SxNDTR_NDT(x) ((x) & SxNDTR_NDT_M)

#define SxFCR_FEIE   (1 << 7)
#define SxFCR_FS_M   0x7u
#define SxFCR_FS(x)  (((x) & SxFCR_FS_M) << 3)
#define SxFCR_DMDIS  (1 << 2)
#define SxFCR_FTH_M  0x3u
#define SxFCR_FTH(x) ((x) & SxFCR_FTH_M)

int dma_stm32h7xx_init(struct dma *ctx, int base, size_t channel)
{
    ctx->base = (struct DMA_STM32H7XX*)base;
    ctx->ch = &ctx->base->CH[channel];
    ctx->channel = channel;

    return 0;
}

int dma_setup(struct dma *ctx, struct dma_xfer *xfer)
{
    uint32_t SxCR = (uint32_t)SxCR_EN;

    /* disable */
    ctx->ch->SxCR = 0;

    /* memory & periphral sizes */
    SxCR |= SxCR_MSIZE(xfer->size >> 1);
    SxCR |= SxCR_PSIZE(xfer->size >> 1);

    /* count */
    ctx->ch->SxNDTR = (uint32_t)(xfer->byte_count / xfer->size);

    if (xfer->saddr >= (intptr_t)DMA_PADDR_BASE) {
        /* peripheral to memory */
        ctx->ch->SxPAR = (uint32_t)xfer->saddr;
        ctx->ch->SxM0AR = (uint32_t)xfer->daddr;

        if (xfer->incr_write) SxCR |= SxCR_MINC;                    /* dst */
        if (xfer->incr_read) SxCR |= (SxCR_PINC | SxCR_PINCOS);     /* src */

    }else{
        /* memory to peripheral */
        ctx->ch->SxPAR = (uint32_t)xfer->daddr;
        ctx->ch->SxM0AR = (uint32_t)xfer->saddr;

        SxCR |= SxCR_DIR(0x1);
        if (xfer->incr_read) SxCR |= SxCR_MINC;                     /* src */
        if (xfer->incr_write) SxCR |= (SxCR_PINC | SxCR_PINCOS);    /* dst */
    }

    /* enable & run */
    ctx->ch->SxCR = SxCR;
    return 0;
}

int dma_xfer(struct dma *ctx, struct dma_xfer *xfer)
{
    return dma_setup(ctx, xfer);
}

int dma_xfer_done(struct dma *ctx)
{
    uint32_t xISR_TCIFn = 0;
    volatile uint32_t *xISR;
    volatile uint32_t *xIFCR;

    if (ctx->channel > (size_t)3) {
        xISR = &ctx->base->HISR;
        xIFCR = &ctx->base->HIFCR;
    }else{
        xISR = &ctx->base->LISR;
        xIFCR = &ctx->base->LIFCR;
    }

    switch (ctx->channel) {
    case 7: xISR_TCIFn = (uint32_t)HISR_TCIF7; break;
    case 6: xISR_TCIFn = (uint32_t)HISR_TCIF6; break;
    case 5: xISR_TCIFn = (uint32_t)HISR_TCIF5; break;
    case 4: xISR_TCIFn = (uint32_t)HISR_TCIF4; break;
    case 3: xISR_TCIFn = (uint32_t)LISR_TCIF3; break;
    case 2: xISR_TCIFn = (uint32_t)LISR_TCIF2; break;
    case 1: xISR_TCIFn = (uint32_t)LISR_TCIF1; break;
    case 0: xISR_TCIFn = (uint32_t)LISR_TCIF0; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    if ((*xISR & xISR_TCIFn) != 0)
        *xIFCR = xISR_TCIFn; /* clear flag */

    if ((ctx->ch->SxCR & SxCR_EN) == 0)
        return 0;

    return -EAGAIN;
}

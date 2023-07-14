#include "dma-stm32h7xx_bdma.h"
#include "picoRTOS.h"

#include <stdint.h>

struct DMA_STM32H7XX_BDMA {
    volatile uint32_t ISR;
    volatile uint32_t IFCR;
    struct {
        volatile uint32_t CCR;
        volatile uint32_t CNDTR;
        volatile uint32_t CPAR;
        volatile uint32_t CM0AR;
        volatile uint32_t CM1AR;
    } CH[DMA_STM32H7XX_BDMA_CHANNEL_COUNT];
};

#define CCR_CT       (1 << 16)
#define CCR_DBM      (1 << 15)
#define CCR_MEM2MEM  (1 << 14)
#define CCR_PL_M     0x3u
#define CCR_PL(x)    (((x) & CCR_PL_M) << 12)
#define CCR_MSIZE_M  0x3u
#define CCR_MSIZE(x) (((x) & CCR_MSIZE_M) << 10)
#define CCR_PSIZE_M  0x3u
#define CCR_PSIZE(x) (((x) & CCR_PSIZE_M) << 8)
#define CCR_MINC     (1 << 7)
#define CCR_PINC     (1 << 6)
#define CCR_CIRC     (1 << 5)
#define CCR_DIR      (1 << 4)
#define CCR_TEIE     (1 << 3)
#define CCR_HTIE     (1 << 2)
#define CCR_TCIE     (1 << 1)
#define CCR_EN       (1 << 0)

#define CNDTR_NDT_M  0xffffu
#define CNDTR_NDT(x) ((x) & CNDTR_NDT_M)

int dma_stm32h7xx_bdma_init(struct dma *ctx, int base, size_t channel)
{
    if (!picoRTOS_assert(channel < (size_t)DMA_STM32H7XX_BDMA_CHANNEL_COUNT)) return -EINVAL;

    ctx->base = (struct DMA_STM32H7XX_BDMA*)base;
    ctx->channel = channel;

    return 0;
}

int dma_setup(struct dma *ctx, struct dma_xfer *xfer)
{

}

int dma_xfer(struct dma *ctx, struct dma_xfer *xfer)
{
}

int dma_xfer_done(struct dma *ctx)
{
}

#ifndef DMA_SAME5X_H
#define DMA_SAME5X_H

#include "dma.h"

#define DMA_SAME5X_CHANNEL_COUNT 32

/* DMAC */

struct DMA_SAME5X;
struct DMA_SAME5X_CH;

/* transfer descriptors */
struct DMA_SAME5X_TD {
    volatile uint16_t BTCTRL;
    volatile uint16_t BTCNT;
    volatile uint32_t SRCADDR;
    volatile uint32_t DSTADDR;
    volatile uint32_t DESCADDR;
};

struct dma_same5x {
    /*@temp@*/ struct DMA_SAME5X *base;
    struct DMA_SAME5X_TD TD[DMA_SAME5X_CHANNEL_COUNT] __attribute__ ((aligned(8)));
    struct DMA_SAME5X_TD WB[DMA_SAME5X_CHANNEL_COUNT] __attribute__ ((aligned(8)));
};

int dma_same5x_init(/*@out@*/ struct dma_same5x *ctx, int base);

/* CHANNELS */

typedef enum {
    DMA_SAME5X_TRIGSRC_DISABLE      = 0x0,
    DMA_SAME5X_TRIGSRC_RTC          = 0x1,
    DMA_SAME5X_TRIGSRC_DSU_DCC0     = 0x2,
    DMA_SAME5X_TRIGSRC_DSU_DCC1     = 0x3,
    DMA_SAME5X_TRIGSRC_SERCOM0_RX   = 0x4,
    DMA_SAME5X_TRIGSRC_SERCOM0_TX   = 0x5,
    DMA_SAME5X_TRIGSRC_SERCOM1_RX   = 0x6,
    DMA_SAME5X_TRIGSRC_SERCOM1_TX   = 0x7,
    DMA_SAME5X_TRIGSRC_SERCOM2_RX   = 0x8,
    DMA_SAME5X_TRIGSRC_SERCOM2_TX   = 0x9,
    DMA_SAME5X_TRIGSRC_SERCOM3_RX   = 0xa,
    DMA_SAME5X_TRIGSRC_SERCOM3_TX   = 0xb,
    DMA_SAME5X_TRIGSRC_SERCOM4_RX   = 0xc,
    DMA_SAME5X_TRIGSRC_SERCOM4_TX   = 0xd,
    DMA_SAME5X_TRIGSRC_SERCOM5_RX   = 0xe,
    DMA_SAME5X_TRIGSRC_SERCOM5_TX   = 0xf,
    DMA_SAME5X_TRIGSRC_SERCOM6_RX   = 0x10,
    DMA_SAME5X_TRIGSRC_SERCOM6_TX   = 0x11,
    DMA_SAME5X_TRIGSRC_SERCOM7_RX   = 0x12,
    DMA_SAME5X_TRIGSRC_SERCOM7_TX   = 0x13,
    DMA_SAME5X_TRIGSRC_CAN0_DEBUG   = 0x14,
    DMA_SAME5X_TRIGSRC_CAN1_DEBUG   = 0x15,
    DMA_SAME5X_TRIGSRC_TCC0_OVF     = 0x16,
    DMA_SAME5X_TRIGSRC_TCC0_MC      = 0x17,
    DMA_SAME5X_TRIGSRC_TCC1_OVF     = 0x1d,
    DMA_SAME5X_TRIGSRC_TCC1_MC      = 0x1e,
    DMA_SAME5X_TRIGSRC_TCC2_OVF     = 0x22,
    DMA_SAME5X_TRIGSRC_TCC2_MC      = 0x23,
    DMA_SAME5X_TRIGSRC_TCC3_OVF     = 0x26,
    DMA_SAME5X_TRIGSRC_TCC3_MC      = 0x27,
    DMA_SAME5X_TRIGSRC_TCC4_OVF     = 0x29,
    DMA_SAME5X_TRIGSRC_TCC4_MC      = 0x2a,
    DMA_SAME5X_TRIGSRC_TC0_OVF      = 0x2c,
    DMA_SAME5X_TRIGSRC_TC0_MC       = 0x2d,
    DMA_SAME5X_TRIGSRC_TC1_OVF      = 0x2f,
    DMA_SAME5X_TRIGSRC_TC1_MC       = 0x30,
    DMA_SAME5X_TRIGSRC_TC2_OVF      = 0x32,
    DMA_SAME5X_TRIGSRC_TC2_MC       = 0x33,
    DMA_SAME5X_TRIGSRC_TC3_OVF      = 0x35,
    DMA_SAME5X_TRIGSRC_TC3_MC       = 0x36,
    DMA_SAME5X_TRIGSRC_TC4_OVF      = 0x38,
    DMA_SAME5X_TRIGSRC_TC4_MC       = 0x39,
    DMA_SAME5X_TRIGSRC_TC5_OVF      = 0x3b,
    DMA_SAME5X_TRIGSRC_TC5_MC       = 0x3c,
    DMA_SAME5X_TRIGSRC_TC6_OVF      = 0x3e,
    DMA_SAME5X_TRIGSRC_TC6_MC       = 0x3f,
    DMA_SAME5X_TRIGSRC_TC7_OVF      = 0x41,
    DMA_SAME5X_TRIGSRC_TC7_MC       = 0x43,
    DMA_SAME5X_TRIGSRC_ADC0_RESRDY  = 0x44,
    DMA_SAME5X_TRIGSRC_ADC0_SEQ     = 0x45,
    DMA_SAME5X_TRIGSRC_ADC1_RESRDY  = 0x46,
    DMA_SAME5X_TRIGSRC_ADC1_SEQ     = 0x47,
    DMA_SAME5X_TRIGSRC_DAC_EMPTY    = 0x48,
    DMA_SAME5X_TRIGSRC_DAC_RESRDY   = 0x4a,
    DMA_SAME5X_TRIGSRC_I2S_RX       = 0x4c,
    DMA_SAME5X_TRIGSRC_I2S_TX       = 0x4e,
    DMA_SAME5X_TRIGSRC_PCC_RX       = 0x50,
    DMA_SAME5X_TRIGSRC_AES_WR       = 0x51,
    DMA_SAME5X_TRIGSRC_AES_RD       = 0x52,
    DMA_SAME5X_TRIGSRC_QSPI_RX      = 0x53,
    DMA_SAME5X_TRIGSRC_QSPI_TX      = 0x54,
    DMA_SAME5X_TRIGSRC_COUNT
} dma_same5x_trigsrc_t;

struct dma {
    /*@temp@*/ struct dma_same5x *parent;
    /*@temp@*/ struct DMA_SAME5X_CH *ch;
    size_t channel;
};

int dma_same5x_dma_init(/*@out@*/ struct dma *ctx, struct dma_same5x *parent,
                        size_t channel, dma_same5x_trigsrc_t trigsrc);

/* Runtime calls:
 * int dma_setup(struct dma *ctx, struct dma_xfer *xfer);
 * int dma_xfer(struct dma *ctx, struct dma_xfer *xfer);
 * int dma_xfer_done(struct dma *ctx);
 */

#endif

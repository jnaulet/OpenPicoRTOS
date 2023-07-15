#ifndef SPI_GD32VF103_H
#define SPI_GD32VF103_H

#include "dma.h"
#include "spi.h"
#include "clock.h"

struct SPI_GD32VF103;

typedef enum {
    SPI_GD32VF103_STATE_DMA_START,
    SPI_GD32VF103_STATE_DMA_WAIT,
    SPI_GD32VF103_STATE_DMA_COUNT
} spi_gd32vf103_state_t;

struct spi_gd32vf103_settings {
    /*@temp@*/ struct dma *fill;
    /*@temp@*/ struct dma *drain;
    size_t threshold;
};

struct spi {
    /*@temp@*/ struct SPI_GD32VF103 *base;
    clock_id_t clkid;
    int balance;
    size_t frame_size;
    size_t frame_width;
    /* dma opt. */
    spi_gd32vf103_state_t state;
    /*@temp@*/ /*@null@*/ struct dma *fill;
    /*@temp@*/ /*@null@*/ struct dma *drain;
    size_t threshold;
};

int spi_gd32vf103_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);
int spi_gd32vf103_setup(struct spi *ctx, struct spi_gd32vf103_settings *settings);

/* Implements:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

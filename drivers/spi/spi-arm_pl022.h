#ifndef SPI_ARM_PL022_H
#define SPI_ARM_PL022_H

#include "spi.h"
#include "dma.h"
#include "clock.h"

#include <stdbool.h>

struct SPI_ARM_PL022;

#define SPI_ARM_PL022_FRAME_SIZE_MIN 4
#define SPI_ARM_PL022_FRAME_SIZE_MAX 16

typedef enum {
    SPI_ARM_PL022_STATE_DMA_START,
    SPI_ARM_PL022_STATE_DMA_WAIT,
    SPI_ARM_PL022_STATE_DMA_COUNT
} spi_arm_pl022_state_t;

struct spi_arm_pl022_settings {
    /* loopback */
    bool loopback;
};

struct spi_arm_pl022_dma_settings {
    /*@temp@*/ struct dma *fill;
    /*@temp@*/ struct dma *drain;
    size_t threshold;
};

struct spi {
    /*@temp@*/ struct SPI_ARM_PL022 *base;
    clock_id_t clkid;
    size_t frame_size;
    size_t frame_width;
    int balance;
    /* dma opt. */
    spi_arm_pl022_state_t state;
    /*@temp@*/ /*@null@*/ struct dma *fill;
    /*@temp@*/ /*@null@*/ struct dma *drain;
    size_t threshold;
};

int spi_arm_pl022_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);
int spi_arm_pl022_setup(struct spi *ctx, struct spi_arm_pl022_settings *settings);
int spi_arm_pl022_setup_dma(struct spi *ctx, struct spi_arm_pl022_dma_settings *settings);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

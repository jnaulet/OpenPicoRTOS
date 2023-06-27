#ifndef SPI_TI_F28X_H
#define SPI_TI_F28X_H

/* spi driver for TI F28x family */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "dma.h"
#include "spi.h"
#include "clock.h"

struct SPI_REGS;

typedef enum {
    SPI_TI_F28X_STATE_DMA_START,
    SPI_TI_F28X_STATE_DMA_WAIT,
    SPI_TI_F28X_STATE_DMA_COUNT
} spi_ti_f28x_state_t;

struct spi_ti_f28x_settings {
    /* DMA */
    /*@temp@*/ /*@null@*/ struct dma *fill;
    /*@temp@*/ /*@null@*/ struct dma *drain;
    size_t threshold;
    /* loopback */
    bool loopback;
};

struct spi {
    /*@temp@*/ struct SPI_REGS *base;
    clock_id_t clkid;
    size_t balance;
    size_t lshift;
    size_t frame_size;
    /* dma opt */
    spi_ti_f28x_state_t state;
    /*@temp@*/ /*@null@*/ struct dma *fill;
    /*@temp@*/ /*@null@*/ struct dma *drain;
    size_t threshold;
};

int spi_ti_f28x_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);
int spi_ti_f28x_setup(struct spi *ctx, struct spi_ti_f28x_settings *settings);

/*
 * Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

#ifndef SPI_ATMEL_SERCOM_H
#define SPI_ATMEL_SERCOM_H

#include "dma.h"
#include "spi.h"
#include "clock.h"

#define SPI_ATMEL_SERCOM_FRAME_SIZE_MIN 8
#define SPI_ATMEL_SERCOM_FRAME_SIZE_MAX 32

struct SPI_ATMEL_SERCOM;

typedef enum {
    SPI_ATMEL_SERCOM_STATE_DMA_START,
    SPI_ATMEL_SERCOM_STATE_DMA_WAIT,
    SPI_ATMEL_SERCOM_STATE_DMA_COUNT
} spi_atmel_sercom_state_t;

struct spi_atmel_sercom_dma_settings {
    /*@temp@*/ struct dma *fill;
    /*@temp@*/ struct dma *drain;
    size_t threshold;
};

struct spi {
    /*@temp@*/ struct SPI_ATMEL_SERCOM *base;
    clock_id_t clkid;
    size_t frame_size;
    size_t frame_width;
    int balance;
    /* dma opt. */
    spi_atmel_sercom_state_t state;
    /*@temp@*/ /*@null@*/ struct dma *fill;
    /*@temp@*/ /*@null@*/ struct dma *drain;
    size_t threshold;
};

int spi_atmel_sercom_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);
int spi_atmel_sercom_setup_dma(struct spi *ctx, struct spi_atmel_sercom_dma_settings *settings);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

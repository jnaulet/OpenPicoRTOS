#ifndef SPI_STM32H7XX_H
#define SPI_STM32H7XX_H

#include "dma.h"
#include "spi.h"
#include "clock.h"

struct SPI_STM32H7XX;

#define SPI_STM32H7XX_DSIZE_MIN 4
#define SPI_STM32H7XX_DSIZE_MAX 32

typedef enum {
    SPI_STM32H7XX_STATE_IDLE,
    SPI_STM32H7XX_STATE_XFER,
    SPI_STM32H7XX_STATE_COUNT
} spi_stm32h7xx_state_t;

struct spi_stm32h7xx_dma_settings {
    /*@temp@*/ struct dma *fill;
    /*@temp@*/ struct dma *drain;
    size_t threshold;
};

struct spi {
    /*@temp@*/ struct SPI_STM32H7XX *base;
    clock_id_t clkid;
    size_t frame_size;
    size_t frame_width;
    int balance;
    spi_stm32h7xx_state_t state;
    size_t fifo_depth;
    /* dma opt. */
    /*@temp@*/ /*@null@*/ struct dma *fill;
    /*@temp@*/ /*@null@*/ struct dma *drain;
    size_t threshold;
};

int spi_stm32h7xx_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);
int spi_stm32h7xx_setup_dma(struct spi *ctx, struct spi_stm32h7xx_dma_settings *settings);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

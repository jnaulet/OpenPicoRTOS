#ifndef SPI_TINYAVR_H
#define SPI_TINYAVR_H

#include "spi.h"
#include "clock.h"

struct SPI_TINYAVR;

struct spi {
    /*@temp@*/ struct SPI_TINYAVR *base;
    clock_id_t clkid;
    int balance;
};

int spi_tinyavr_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

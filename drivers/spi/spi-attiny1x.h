#ifndef SPI_ATTINY1X_H
#define SPI_ATTINY1X_H

#include "spi.h"
#include "clock.h"

struct SPI_ATTINY1X;

struct spi {
    /*@temp@*/ struct SPI_ATTINY1X *base;
    clock_id_t clkid;
    int balance;
};

int spi_attiny1x_init(/*@out@*/ struct spi *ctx, struct SPI_ATTINY1X *base, clock_id_t clkid);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

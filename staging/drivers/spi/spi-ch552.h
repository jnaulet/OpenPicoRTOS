#ifndef SPI_CH552_H
#define SPI_CH552_H

#include "spi.h"
#include "clock.h"

struct spi {
    clock_id_t clkid;
};

int spi_ch552_init(/*@out@*/ struct spi *ctx, clock_id_t clkid);
int spi_ch552_set_half_duplex(struct spi *ctx);

/* Implements:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

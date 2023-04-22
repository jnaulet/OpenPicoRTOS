#ifndef SPI_GD32VF103_H
#define SPI_GD32VF103_H

#include "spi.h"
#include "clock.h"

struct SPI_GD32VF103;

struct spi {
    /*@temp@*/ struct SPI_GD32VF103 *base;
    clock_id_t clkid;
    int balance;
    size_t frame_size;
};

int spi_gd32vf103_init(/*@out@*/ struct spi *ctx, struct SPI_GD32VF103 *base, clock_id_t clkid);

/* Implements:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

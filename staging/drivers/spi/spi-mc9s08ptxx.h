#ifndef SPI_MC9S08PTXX_H
#define SPI_MC9S08PTXX_H

#include "spi.h"
#include "clock.h"

#include <stddef.h>

struct SPI_MC9S08PTXX;

#define SPI_MC9S08PTXX_FRAME_SIZE_MIN 8
#define SPI_MC9S08PTXX_FRAME_SIZE_MAX 16 /* only SPI1 */

struct spi {
    /*@temp@*/ struct SPI_MC9S08PTXX *base;
    clock_id_t clkid;
    size_t frame_width;
};

int spi_mc9s08ptxx_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

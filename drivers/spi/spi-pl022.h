#ifndef SPI_PL022_H
#define SPI_PL022_H

#include "spi.h"
#include "clock.h"

#include <stdbool.h>

struct SPI_PL022;

#define SPI_PL022_FRAME_SIZE_MIN 4
#define SPI_PL022_FRAME_SIZE_MAX 16

struct spi {
    /*@temp@*/ struct SPI_PL022 *base;
    clock_id_t clkid;
    size_t frame_size;
    int balance;
};

int spi_pl022_init(/*@out@*/ struct spi *ctx, struct SPI_PL022 *base, clock_id_t clkid);

/* for tests */
int spi_pl022_set_loopback(struct spi *ctx, bool loopback);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

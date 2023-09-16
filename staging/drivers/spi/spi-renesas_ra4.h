#ifndef SPI_RENESAS_RA4_H
#define SPI_RENESAS_RA4_H

#include "spi.h"
#include "clock.h"

#include <stddef.h>
#include <stdbool.h>

#define SPI_RENESAS_RA4_SSL_COUNT 4
#define SPI_RENESAS_RA4_FRAME_SIZE_MIN 8
#define SPI_RENESAS_RA4_FRAME_SIZE_MAX 32

struct SPI_RENESAS_RA4;

struct spi_renesas_ra4_settings {
    /* loopback */
    bool loopback;
};

struct spi {
    /*@temp@*/ struct SPI_RENESAS_RA4 *base;
    clock_id_t clkid;
    size_t frame_size;
    size_t frame_width;
    int balance;
    size_t ssl;
};

int spi_renesas_ra4_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);
int spi_renesas_ra4_setup(struct spi *ctx, struct spi_renesas_ra4_settings *settings);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

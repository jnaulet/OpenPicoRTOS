#ifndef SPI_ATSAMX7X_H
#define SPI_ATSAMX7X_H

#include "spi.h"
#include "clock.h"

#include <stdint.h>
#include <stdbool.h>

struct SPI_ATSAMX7X;

#define SPI_ATSAMX7X_CS_COUNT 4 /* TODO: move to device ? */
#define SPI_ATSAMX7X_FRAME_SIZE_MIN 8
#define SPI_ATSAMX7X_FRAME_SIZE_MAX 16

struct spi {
    /*@temp@*/ struct SPI_ATSAMX7X *base;
    clock_id_t clkid;
    int balance;
    size_t frame_size;
    size_t frame_width;
    uint32_t pcs;
};

int spi_atsamx7x_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);

/* for tests */
int spi_atsamx7x_set_loopback(struct spi *ctx, bool loopback);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

#ifndef SPI_ATMEL_SERCOM_H
#define SPI_ATMEL_SERCOM_H

#include "spi.h"
#include "clock.h"

#define SPI_ATMEL_SERCOM_FRAME_SIZE_MIN 8
#define SPI_ATMEL_SERCOM_FRAME_SIZE_MAX 32

struct SPI_ATMEL_SERCOM;

struct spi {
    /*@temp@*/ struct SPI_ATMEL_SERCOM *base;
    clock_id_t clkid;
    size_t frame_size;
    int balance;
};

int spi_atmel_sercom_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

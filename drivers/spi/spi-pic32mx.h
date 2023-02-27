#ifndef SPI_PIC32MX_H
#define SPI_PIC32MX_H

#include "spi.h"
#include "clock.h"

struct SPI_PIC32MX;

struct spi {
    /*@temp@*/ struct SPI_PIC32MX *base;
    clock_id_t clkid;
    size_t frame_size;
    int balance;
};

int spi_pic32mx_init(/*@out@*/ struct spi *ctx, struct SPI_PIC32MX *base, clock_id_t clkid);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

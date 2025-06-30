#ifndef SPI_LGT8FX8P_H
#define SPI_LGT8FX8P_H

#include "spi.h"
#include "clock.h"

struct SPI_LGT8FX8P;

struct spi {
    /*@temp@*/ struct SPI_LGT8FX8P *base;
    clock_id_t clkid;
    int balance;
};

int spi_lgt8fx8p_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

#ifndef SPI_TI_F28X_H
#define SPI_TI_F28X_H

/* spi driver for TI F28x family */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "spi.h"
#include "clock.h"

struct SPI_REGS;

struct spi {
    /*@temp@*/ struct SPI_REGS *base;
    clock_id_t clkid;
    size_t balance;
    size_t lshift;
};

int spi_ti_f28x_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);

/* options */
int spi_ti_f28x_set_loopback(struct spi *ctx, bool loopback);

/*
 * Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

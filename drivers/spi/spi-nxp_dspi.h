#ifndef SPI_NXP_DSPI_H
#define SPI_NXP_DSPI_H

/* NXP_DSPI / SPI driver for NXP MPC5x series */

#include "spi.h"
#include "clock.h"

#include <stddef.h>

struct SPI_NXP_DSPI;

struct spi {
    /*@temp@*/ struct SPI_NXP_DSPI *base;
    clock_id_t clkid;
    size_t cs;
    size_t frame_size;
    size_t frame_width;
    int balance;
};

int spi_nxp_dspi_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);

#endif

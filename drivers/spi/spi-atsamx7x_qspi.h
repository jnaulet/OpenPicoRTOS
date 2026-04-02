#ifndef SPI_ATSAMX7X_QPI_H
#define SPI_ATSAMX7X_QPI_H

#include "spi.h"
#include "clock.h"

#include <stdint.h>
#include <stdbool.h>

#define SPI_ATSAMX7X_QSPI_FRAME_NBITS_MIN 8
#define SPI_ATSAMX7X_QSPI_FRAME_NBITS_MAX 16

struct SPI_ATSAMX7X_QSPI;

struct spi {
    /*@temp@*/ struct SPI_ATSAMX7X_QSPI *base;
    clock_id_t clkid;
    int balance;
    size_t frame_nbits;
    size_t frame_width;
};

int spi_atsamx7x_qspi_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);

struct spi_atsamx7x_qspi_smm_settings {
    uint32_t addr;
    uint8_t inst;
    uint8_t opt;
    size_t dummy_cycles;
};

int spi_atsamx7x_qspi_smm(struct spi *ctx, const struct spi_atsamx7x_qspi_smm_settings *settings);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

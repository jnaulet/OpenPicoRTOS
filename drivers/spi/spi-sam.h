#ifndef SPI_SAM_H
#define SPI_SAM_H

#include "spi.h"
#include "clock.h"

#include <stdint.h>
#include <stdbool.h>

struct SPI_SAM;

#define SPI_SAM_CS_COUNT 4 /* TODO: move to device ? */
#define SPI_SAM_FRAME_SIZE_MIN 8
#define SPI_SAM_FRAME_SIZE_MAX 16

struct spi {
    /*@temp@*/ struct SPI_SAM *base;
    clock_id_t clkid;
    int balance;
    size_t frame_size;
    uint32_t pcs;
};

int spi_sam_init(/*@out@*/ struct spi *ctx, struct SPI_SAM *base, clock_id_t clkid);

/* for tests */
int spi_sam_set_loopback(struct spi *ctx, bool loopback);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

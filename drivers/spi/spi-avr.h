#ifndef SPI_AVR_H
#define SPI_AVR_H

#include "spi.h"

struct SPI_AVR;

typedef enum {
    SPI_AVR_SPEED_SPI1X,
    SPI_AVR_SPEED_SPI2X,
    SPI_AVR_SPEED_COUNT
} spi_avr_speed_t;

typedef enum {
    SPI_AVR_STATE_START,
    SPI_AVR_STATE_XFER,
    SPI_AVR_STATE_COUNT
} spi_avr_state_t;

struct spi {
    /*@temp@*/ struct SPI_AVR *base;
    spi_avr_state_t state;
};

int spi_avr_init(/*@out@*/ struct spi *ctx, struct SPI_AVR *base, spi_avr_speed_t speed);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

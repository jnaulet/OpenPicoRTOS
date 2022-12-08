#ifndef HAL_SPI_H
#define HAL_SPI_H

#include <errno.h>
#include <stddef.h>

struct spi;

/* Enum: SPI mode
 *
 * SPI_MODE_IGNORE - Ignore mode
 * SPI_MODE_MASTER - SPI master mode
 * SPI_MODE_SLAVE - SPI slave mode
 */
typedef enum {
    SPI_MODE_IGNORE,
    SPI_MODE_MASTER,
    SPI_MODE_SLAVE,
    SPI_MODE_COUNT
} spi_mode_t;

/* Enum: SPI clock mode
 *
 * SPI_CLOCK_MODE_0 - Clock polarity 0, clock phase 0
 * SPI_CLOCK_MODE_1 - Clock polarity 0, clock phase 1
 * SPI_CLOCK_MODE_2 - Clock polarity 1, clock phase 0
 * SPI_CLOCK_MODE_2 - Clock polarity 1, clock phase 1
 */
typedef enum {
    SPI_CLOCK_MODE_IGNORE,
    SPI_CLOCK_MODE_0,   /* CPOL 0 CPHA 0 */
    SPI_CLOCK_MODE_1,   /* CPOL 0 CPHA 1 */
    SPI_CLOCK_MODE_2,   /* CPOL 1 CPHA 0 */
    SPI_CLOCK_MODE_3,   /* CPOL 1 CPHA 1 */
    SPI_CLOCK_MODE_COUNT
} spi_clock_mode_t;

/* Enum: SPI Slave Select polarity
 *
 * SPI_CS_POL_IGNORE - Ignore polarity
 * SPI_CS_POL_ACTIVE_LOW - SS is active low
 * SPI_CS_POL_ACTIVE_HIGH - SS is active high
 */
typedef enum {
    SPI_CS_POL_IGNORE,
    SPI_CS_POL_ACTIVE_LOW,
    SPI_CS_POL_ACTIVE_HIGH,
    SPI_CS_POL_COUNT
} spi_cs_pol_t;

/* Constants: Bitrate configuration
 *
 * SPI_BITRATE_IGNORE - Ignore SPI bitrate
 */
#define SPI_BITRATE_IGNORE 0

struct spi_settings {
    unsigned long bitrate; /* 0 = ignore */
    spi_mode_t mode;
    spi_clock_mode_t clkmode;
    size_t frame_size; /* 0 = ignore */
    spi_cs_pol_t cs_pol;
    size_t cs;
};

/* Function: spi_setup
 * Configure a SPI interface
 *
 * Parameters:
 *  ctx - A SPI instance
 *  settings - The parameters used to configure the SPI
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int spi_setup(struct spi *ctx, const struct spi_settings *settings);

/* Function: spi_xfer
 * Transfers data over a SPI interface
 *
 * Parameters:
 *  ctx - A SPI instance
 *  rx - A buffer to receive the data
 *  tx - A buffer with the data to transmit
 *  n - The size of the data to transfer (in bytes)
 *
 * Returns:
 * The number of bytes transfered if success, -errno otherwise
 */
int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);

#endif

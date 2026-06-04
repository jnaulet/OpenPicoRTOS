#ifndef HAL_SPI_SAFETY_H
#define HAL_SPI_SAFETY_H

#include "spi.h"

/* Function: spi_claim
 * Claims the spi for this process/thread (opt. MPU-related feature)
 *
 * Parameters:
 *  spi - A spi instance
 */
struct spi *spi_claim(/*@returned@*/ struct spi *ctx);

#endif

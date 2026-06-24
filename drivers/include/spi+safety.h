#ifndef HAL_SPI_SAFETY_H
#define HAL_SPI_SAFETY_H

#include "spi.h"

/**
 * struct spi \***spi_claim**(**struct spi** \*<ins>ctx</ins>);
 * > Claims the spi channel <ins>ctx</ins> & its dependencies
 * ### NOTES
 * > This feature is related to MPU & will only have an effect if
 * > `CONFIG_MPU` is active.
 */
struct spi *spi_claim(/*@returned@*/ struct spi *ctx);

#endif

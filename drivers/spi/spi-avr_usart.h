#ifndef SPI_AVR_USART_H
#define SPI_AVR_USART_H

#include "spi.h"
#include "clock.h"

struct MSPIM_AVR_USART;

typedef enum {
    SPI_AVR_USART_STATE_START,
    SPI_AVR_USART_STATE_XFER,
    SPI_AVR_USART_STATE_COUNT
} spi_avr_usart_state_t;

struct spi {
    /*@temp@*/ struct MSPIM_AVR_USART *base;
    clock_id_t clkid;
    spi_avr_usart_state_t state;
};

int spi_avr_usart_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

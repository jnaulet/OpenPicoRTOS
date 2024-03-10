#ifndef SPI_AVR_IRQDRIVEN_H
#define SPI_AVR_IRQDRIVEN_H

#include "spi.h"
#include "fifo.h"
#include "clock.h"

#include <stdint.h>
#include "picoRTOS_types.h"

struct SPI_AVR;

typedef enum {
    SPI_AVR_STATE_START,
    SPI_AVR_STATE_XFER,
    SPI_AVR_STATE_COUNT
} spi_avr_state_t;

typedef enum {
    SPI_AVR_MODE_NORMAL,
    SPI_AVR_MODE_IRQDRIVEN,
    SPI_AVR_MODE_COUNT
} spi_avr_mode_t;

struct spi {
    /*@temp@*/ struct SPI_AVR *base;
    clock_id_t clkid;
    spi_avr_state_t state;
    spi_avr_mode_t mode;
    /* IRQ-driven opt */
    struct fifo_head8 rx_fifo;
    struct fifo_head8 tx_fifo;
    /*@null@*/ /*@temp@*/ uint8_t *rx_buf;
    /*@null@*/ /*@temp@*/ uint8_t *tx_buf;
};

struct spi_avr_irqdriven_settings {
    picoRTOS_irq_t irq;
    /*@temp@*/ uint8_t *rx_buf;
    /*@temp@*/ uint8_t *tx_buf;
    uint8_t mask;
};

int spi_avr_init(/*@out@*/ struct spi *ctx, int base, clock_id_t clkid);
int spi_avr_setup_irqdriven(struct spi *ctx, struct spi_avr_irqdriven_settings *settings);

/* Runtime calls:
 * int spi_setup(struct spi *ctx, const struct spi_settings *settings);
 * int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n);
 */

#endif

#ifndef UART_AVR_IRQDRIVEN_H
#define UART_AVR_IRQDRIVEN_H

#include "uart.h"
#include "clock.h"

#include "fifo.h"
#include "picoRTOS_types.h"

#define UART_AVR_CS_MIN 5
#define UART_AVR_CS_MAX 8

struct USART_AVR;

typedef enum {
    UART_AVR_MODE_NORMAL,
    UART_AVR_MODE_IRQDRIVEN,
    UART_AVR_MODE_COUNT
} uart_avr_mode_t;

struct uart {
    /*@temp@*/ struct USART_AVR *base;
    clock_id_t clkid;
    uart_avr_mode_t mode;
    /* IRQ-driven opt */
    struct fifo_head8 rx_fifo;
    struct fifo_head8 tx_fifo;
    /*@null@*/ /*@temp@*/ char *rx_buf;
    /*@null@*/ /*@temp@*/ char *tx_buf;
};

struct uart_avr_irq_settings {
    picoRTOS_irq_t irq_rx;
    picoRTOS_irq_t irq_udre;
    /*@temp@*/ char *rx_buf;
    uint8_t rx_mask;
    /*@temp@*/ char *tx_buf;
    uint8_t tx_mask;
};

int uart_avr_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid);
int uart_avr_set_mode_irqdriven(struct uart *ctx, struct uart_avr_irq_settings *settings);

/* Runtime calls:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

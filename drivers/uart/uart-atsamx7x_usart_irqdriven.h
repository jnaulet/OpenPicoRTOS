#ifndef UART_ATSAMX7X_USART_IRQDRIVEN_H
#define UART_ATSAMX7X_USART_IRQDRIVEN_H

#include "uart.h"
#include "clock.h"

#include "fifo.h"
#include "picoRTOS_types.h"

#define UART_ATSAMX7X_USART_CS_MIN 5
#define UART_ATSAMX7X_USART_CS_MAX 8

struct UART_ATSAMX7X_USART;

typedef enum {
    UART_ATSAMX7X_USART_STATE_START,
    UART_ATSAMX7X_USART_STATE_XFER,
    UART_ATSAMX7X_USART_STATE_STATE_COUNT
} uart_atsamx7x_usart_state_t;

struct uart {
    /*@temp@*/ struct UART_ATSAMX7X_USART *base;
    clock_id_t clkid;
    /* IRQ-driven opt */
    struct fifo_head rx_fifo;
    struct fifo_head tx_fifo;
    char rx_buf[32];    /* FIXME */
    char tx_buf[32];    /* FIXME */
};

int uart_atsamx7x_usart_irqdriven_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid, picoRTOS_irq_t irq);

typedef enum {
    UART_ATSAMX7X_USART_USCLKS_MCK  = 0,
    UART_ATSAMX7X_USART_USCLKS_DIV  = 1,
    UART_ATSAMX7X_USART_USCLKS_PCK  = 2,
    UART_ATSAMX7X_USART_USCLKS_SCK  = 3,
    UART_ATSAMX7X_USART_USCLKS_COUNT
} uart_atsamx7x_usart_usclks_t;

int uart_atsamx7x_usart_set_usclks(struct uart *ctx, uart_atsamx7x_usart_usclks_t usclks);

/* Runtime calls:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

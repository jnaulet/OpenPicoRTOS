#ifndef UART_ATTINY1X_USART_H
#define UART_ATTINY1X_USART_H

#include "uart.h"
#include "clock.h"

struct USART_ATTINY1X_UART;

struct uart {
    /*@temp@*/ struct USART_ATTINY1X_UART *base;
    clock_id_t clkid;
};

int uart_attiny1x_usart_init(/*@out@*/ struct uart *ctx,
                             struct USART_ATTINY1X_UART *base,
                             clock_id_t clkid);

/* Implements:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

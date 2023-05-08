#ifndef UART_TINYAVR_USART_H
#define UART_TINYAVR_USART_H

#include "uart.h"
#include "clock.h"

struct USART_TINYAVR_UART;

struct uart {
    /*@temp@*/ struct USART_TINYAVR_UART *base;
    clock_id_t clkid;
};

int uart_tinyavr_usart_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid);

/* Implements:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

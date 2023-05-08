#ifndef UART_ATMEL_SERCOM_H
#define UART_ATMEL_SERCOM_H

#include "uart.h"
#include "clock.h"

#define UART_ATMEL_SERCOM_CS_MIN 5
#define UART_ATMEL_SERCOM_CS_MAX 9

struct UART_ATMEL_SERCOM;

struct uart {
    /*@temp@*/ struct UART_ATMEL_SERCOM *base;
    clock_id_t clkid;
};

int uart_atmel_sercom_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid);

/* Implements:
 *
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

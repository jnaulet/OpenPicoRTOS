#ifndef UART_ATSAMX7X_H
#define UART_ATSAMX7X_H

#include "uart.h"
#include "clock.h"

struct UART_ATSAMX7X;

struct uart {
    /*@temp@*/ struct UART_ATSAMX7X *base;
    clock_id_t clkid;
};

int uart_atsamx7x_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid);

/* Runtime calls:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

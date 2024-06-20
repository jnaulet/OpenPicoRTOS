#ifndef UART_MC9S08PTXX_SCI_H
#define UART_MC9S08PTXX_SCI_H

#include "uart.h"
#include "clock.h"

struct UART_MC9S08PTXX_SCI;

struct uart {
    /*@temp@*/ struct UART_MC9S08PTXX_SCI *base;
    clock_id_t clkid;
};

int uart_mc9s08ptxx_sci_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid);

/* Runtime calls:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

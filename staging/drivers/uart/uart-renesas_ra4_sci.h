#ifndef UART_RENESAS_RA4_SCI_H
#define UART_RENESAS_RA4_SCI_H

#include "uart.h"
#include "clock.h"

#define UART_RENESAS_RA4_SCI_CS_MIN 7
#define UART_RENESAS_RA4_SCI_CS_MAX 9

struct UART_RENESAS_RA4_SCI;

struct uart {
    /*@temp@*/ struct UART_RENESAS_RA4_SCI *base;
    clock_id_t clkid;
};

int uart_renesas_ra4_sci_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid);

/* Implements:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

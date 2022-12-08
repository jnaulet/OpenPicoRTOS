#ifndef UART_SAM_H
#define UART_SAM_H

#include "uart.h"
#include "clock.h"

struct UART_SAM;

struct uart {
    /*@temp@*/ struct UART_SAM *base;
    clock_id_t clkid;
};

int uart_sam_init(/*@out@*/ struct uart *ctx, struct UART_SAM *base, clock_id_t clkid);

/* Runtime calls:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

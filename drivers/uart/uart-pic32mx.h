#ifndef UART_PIC32MX_H
#define UART_PIC32MX_H

#include "uart.h"
#include "clock.h"

struct UART_PIC32MX;

struct uart {
    /*@temp@*/ struct UART_PIC32MX *base;
    clock_id_t clkid;
};

int uart_pic32mx_init(/*@out@*/ struct uart *ctx, struct UART_PIC32MX *base, clock_id_t clkid);

/* Runtime calls:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

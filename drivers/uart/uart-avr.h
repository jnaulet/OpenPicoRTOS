#ifndef UART_AVR_H
#define UART_AVR_H

#include "uart.h"
#include "clock.h"

#define UART_AVR_CS_MIN 5
#define UART_AVR_CS_MAX 8

struct USART_AVR;

struct uart {
    /*@temp@*/ struct USART_AVR *base;
    clock_id_t clkid;
};

int uart_avr_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid);

/* Runtime calls:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

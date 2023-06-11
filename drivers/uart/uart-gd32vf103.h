#ifndef UART_GD32VF103_H
#define UART_GD32VF103_H

#include "uart.h"
#include "clock.h"

struct UART_GD32VF103_USART;

#define UART_GD32VF103_CS_MIN 8
#define UART_GD32VF103_CS_MAX 9

struct uart {
    /*@temp@*/ struct UART_GD32VF103_USART *base;
    clock_id_t clkid;
};

int uart_gd32vf103_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid);

/* Implements:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */
#endif

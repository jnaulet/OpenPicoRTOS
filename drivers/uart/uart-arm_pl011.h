#ifndef UART_ARM_PL011_H
#define UART_ARM_PL011_H

/* UART driver for ARM PrimeCell */

#include "uart.h"
#include "clock.h"

#include <stdbool.h>

struct UART_ARM_PL011;

#define UART_ARM_PL011_CS_MIN 5
#define UART_ARM_PL011_CS_MAX 8

struct uart {
    /*@temp@*/ struct UART_ARM_PL011 *base;
    clock_id_t clkid;
};

int uart_arm_pl011_init(struct uart *ctx, int base, clock_id_t clkid);

/* test */
int uart_arm_pl011_set_loopback(struct uart *ctx, bool loopback);

/* Implements:
 *
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

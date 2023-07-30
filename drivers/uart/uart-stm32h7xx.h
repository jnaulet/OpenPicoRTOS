#ifndef UART_STM32H7XX_H
#define UART_STM32H7XX_H

#include "uart.h"
#include "clock.h"

#define UART_STM32H7XX_CS_MIN 7
#define UART_STM32H7XX_CS_MAX 8

struct UART_STM32H7XX;

struct uart {
    /*@temp@*/ struct UART_STM32H7XX *base;
    clock_id_t clkid;
};

int uart_stm32h7xx_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid);

/* Implements:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

#ifndef UART_N76E003_H
#define UART_N76E003_H

#include "uart.h"
#include "clock.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    UART_N76E003_UART0_T1,
    UART_N76E003_UART0_T3,
    UART_N76E003_UART1_T3,
    UART_N76E003_COUNT
} uart_n76e003_t;

struct uart {
    uart_n76e003_t uart;
    bool busy;
};

int uart_n76e003_init(/*@out@*/ struct uart *ctx, uart_n76e003_t uart);

/* Runtime calls:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

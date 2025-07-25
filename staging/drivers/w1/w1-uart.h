#ifndef W1_UART_H
#define W1_UART_H

/*
 * 1-wire from UART hack
 * Put a 480Ohm resistor between tx & rx
 */

#include "w1.h"
#include "uart.h"
#include <stdint.h>

typedef enum {
    W1_UART_STATE_IDLE,
    W1_UART_STATE_RESET,
    W1_UART_STATE_WRITE_0,
    W1_UART_STATE_WRITE_1,
    W1_UART_STATE_READ,
    W1_UART_STATE_COUNT
} w1_uart_state_t;

typedef enum {
    W1_UART_RESET_STATE_PULSE,
    W1_UART_RESET_STATE_PRESENCE,
    W1_UART_RESET_STATE_COUNT
} w1_uart_reset_state_t;

struct w1 {
    /*@temp@*/ struct uart *uart;
    unsigned int tx_bit;
    unsigned int rx_bit;
    w1_uart_state_t state;
    w1_uart_reset_state_t reset_state;
};

int w1_uart_init(/*@out@*/ struct w1 *ctx, struct uart *uart);

/* Implements:
 * int w1_reset(struct w1 *ctx);
 * int w1_write(struct w1 *ctx, const void *buf, size_t n);
 * int w1_read(struct w1 *ctx, void *buf, size_t n);
 */

#endif

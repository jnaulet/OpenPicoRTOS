#ifndef UART_8051_H
#define UART_8051_H

#include "uart.h"
#include "clock.h"

#include <stdbool.h>
#include <stdint.h>

#include "picoRTOS_types.h"

struct uart {
    uart_8051_mode_t mode;
    clock_id_t clkid;
    bool tx_trig;
};

int uart_8051_init(/*@out@*/ struct uart *ctx, clock_id_t clkid);
/* Runtime calls:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

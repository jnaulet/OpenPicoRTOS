#ifndef UART_8051_H
#define UART_8051_H

#include "uart.h"
#include "clock.h"
#include "fifo.h"

#include <stdbool.h>
#include <stdint.h>

#include "picoRTOS_types.h"

typedef enum {
    UART_8051_MODE_NORMAL,
    UART_8051_MODE_IRQDRIVEN,
    UART_8051_MODE_COUNT
} uart_8051_mode_t;

struct uart {
    uart_8051_mode_t mode;
    clock_id_t clkid;
    bool tx_trig;
    /* IRQ-driven opt */
    struct fifo_head8 rx_fifo;
    struct fifo_head8 tx_fifo;
    /*@null@*/ /*@temp@*/ char *rx_buf;
    /*@null@*/ /*@temp@*/ char *tx_buf;
};

struct uart_8051_irqdriven_settings {
    picoRTOS_irq_t irq_ri_ti;
    picoRTOS_priority_t prio;
    /*@temp@*/ char *rx_buf;
    uint8_t rx_mask;
    /*@temp@*/ char *tx_buf;
    uint8_t tx_mask;
};

int uart_8051_init(/*@out@*/ struct uart *ctx, clock_id_t clkid);
int uart_8051_setup_irqdriven(struct uart *ctx, struct uart_8051_irqdriven_settings *settings);

/* Runtime calls:
 * int uart_setup(struct uart *ctx, const struct uart_settings *settings);
 * int uart_write(struct uart *ctx, const char *buf, size_t n);
 * int uart_read(struct uart *ctx, char *buf, size_t n);
 */

#endif

#ifndef UART_NXP_LINFLEXD_H
#define UART_NXP_LINFLEXD_H

#include "uart.h"
#include "clock.h"

struct UART_NXP_LINFLEXD;

typedef enum {
    UART_NXP_LINFLEXD_STATE_START,
    UART_NXP_LINFLEXD_STATE_XFER,
    UART_NXP_LINFLEXD_STATE_STOP,
    UART_NXP_LINFLEXD_STATE_COUNT
} uart_nxp_linflexd_state_t;

struct uart {
    /*@temp@*/ struct UART_NXP_LINFLEXD *base;
    clock_id_t clkid;
    uart_nxp_linflexd_state_t state;
};

int uart_nxp_linflexd_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid);

#endif

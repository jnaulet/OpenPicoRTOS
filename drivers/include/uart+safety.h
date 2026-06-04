#ifndef HAL_UART_SAFETY_H
#define HAL_UART_SAFETY_H

#include "uart.h"

/* Function: uart_claim
 * Claims the uart for this process/thread (opt. MPU-related feature)
 *
 * Parameters:
 *  uart - A uart instance
 */
struct uart *uart_claim(/*@returned@*/ struct uart *ctx);

#endif

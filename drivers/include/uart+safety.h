#ifndef HAL_UART_SAFETY_H
#define HAL_UART_SAFETY_H

#include "uart.h"

/**
 * struct uart \***uart_claim**(**struct uart** \*<ins>ctx</ins>);
 * > Claims the uart channel <ins>ctx</ins> & its dependencies
 * ### NOTES
 * > This feature is related to MPU & will only have an effect if
 * > `CONFIG_MPU` is active.
 */
struct uart *uart_claim(/*@returned@*/ struct uart *ctx);

#endif

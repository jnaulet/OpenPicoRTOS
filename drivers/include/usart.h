#ifndef HAL_USART_H
#define HAL_USART_H

#include <errno.h>
#include <stddef.h>
#include <stdbool.h>

struct usart;

/* Function: usart_xfer
 * Transfers data over a USART interface
 *
 * Parameters:
 *  ctx - A USART instance
 *  rx - A buffer to receive the data
 *  tx - A buffer with the data to transmit
 *  n - The size of the data to transfer (in bytes)
 *
 * Returns:
 * The number of bytes transfered if success, -errno otherwise
 */
int usart_xfer(struct usart *ctx, void *rx, const void *tx, size_t n);

#endif

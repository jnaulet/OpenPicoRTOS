#ifndef HAL_UART_H
#define HAL_UART_H

#include <errno.h>
#include <stddef.h>
#include <stdbool.h>

struct uart;

struct uart_settings {
    unsigned long baudrate;
    size_t cs;
    bool parenb;
    bool parodd;
    bool cstopb;
};

/* Function: uart_setup
 * Configure a UART interface
 *
 * Parameters:
 *  ctx - The UART interface to configure
 *  settings - The configuration parameters
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int uart_setup(struct uart *ctx, const struct uart_settings *settings);

/* Function: uart_write
 * Write data to a UART interface
 *
 * Parameters:
 *  ctx - A UART interface
 *  buf - A buffer containing the data to write
 *  n - The size of the data to write (in bytes)
 *
 * Returns:
 * The number of bytes written if success, -errno otherwise
 */
int uart_write(struct uart *ctx, const char *buf, size_t n);

/* Function: uart_read
 * Read data from a UART interface
 *
 * Parameters:
 *  ctx - A UART interface
 *  buf - A buffer to recevie the read data
 *  n - The size of the buffer (in bytes)
 *
 * Returns:
 * The number of bytes read if success, -errno otherwise
 */
int uart_read(struct uart *ctx, char *buf, size_t n);

#endif

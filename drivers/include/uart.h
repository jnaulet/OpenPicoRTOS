#ifndef HAL_UART_H
#define HAL_UART_H

#include <errno.h>
#include <stddef.h>

struct uart;

/* Enum: UART parity
 *
 * UART_PAR_IGNORE - Ignore parity change request
 * UART_PAR_NONE - No parity
 * UART_PAR_ODD - Odd parity
 * UART_PAR_EVEN - Even parity
 */
typedef enum {
    UART_PAR_IGNORE,
    UART_PAR_NONE,
    UART_PAR_ODD,
    UART_PAR_EVEN,
    UART_PAR_COUNT
} uart_par_t;

/* Enum: UART stop bits
 *
 * UART_CSTOPB_IGNORE - Ignore stop bit req
 * UART_CSTOPB_1BIT - 1 stop bit
 * UART_CSTOPB_2BIT - 2 stop bits
 */
typedef enum {
    UART_CSTOPB_IGNORE,
    UART_CSTOPB_1BIT,
    UART_CSTOPB_2BIT,
    UART_CSTOPB_COUNT
} uart_cstopb_t;

struct uart_settings {
    unsigned long baudrate; /* 0 = ignore */
    size_t cs;              /* 0 = ignore */
    uart_par_t par;
    uart_cstopb_t cstopb;
};

/**
 * int **uart_setup**(**struct uart** \*<ins>ctx</ins>, 
 * const **struct uart_settings** \*<ins>settings</ins>);
 * > Configure a UART interface
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int uart_setup(struct uart *ctx, const struct uart_settings *settings);

/**
 * int **uart_write**(**struct uart** \*<ins>ctx</ins>, 
 * const **char** \*<ins>buf</ins>, **size_t** <ins>n</ins>);
 * > Write data to a UART interface
 * ### RETURN
 * > Returns the number of bytes written if success, -errno otherwise
 */
int uart_write(struct uart *ctx, const char *buf, size_t n);

/**
 * int **uart_read**(**struct uart** \*<ins>ctx</ins>, 
 * **char** \*<ins>buf</ins>, **size_t** <ins>n</ins>);
 * > Read data from a UART interface
 * ### RETURN
 * > Returns the number of bytes read if success, -errno otherwise
 */
int uart_read(struct uart *ctx, char *buf, size_t n);

#endif

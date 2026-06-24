#ifndef HAL_TWI_H
#define HAL_TWI_H

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

struct twi;

/* Enum:
 *
 * TWI_MODE_IGNORE - Ignore mode
 * TWI_MODE_MASTER - TWI master mode
 * TWI_MODE_SLAVE - TWI slave mode
 */
typedef enum {
    TWI_MODE_IGNORE,
    TWI_MODE_MASTER,
    TWI_MODE_SLAVE,
    TWI_MODE_COUNT
} twi_mode_t;

/* Constants: TWI bitrate
 *
 * TWI_BITRATE_IGNORE - Ignore bitrate
 * TWI_BITRATE_STANDARD - Standard bitrate <= 100Khz
 * TWI_BITRATE_FAST - Fast mode <= 400Khz
 * TWI_BITRATE_FAST_PLUS - Fast plus mode <= 1Mhz
 * TWI_BITRATE_HI_SPEED - Hi speed mode <= 3.4Mhz
 */
#define TWI_BITRATE_IGNORE    0
#define TWI_BITRATE_STANDARD  100000ul
#define TWI_BITRATE_FAST      400000ul
#define TWI_BITRATE_FAST_PLUS 1000000ul
#define TWI_BITRATE_HI_SPEED  3400000ul

typedef uint16_t twi_addr_t;

/* Constants: TWI R/W
 *
 * TWI_WRITE - Twi write command (0)
 * TWI_READ - Twi read command (1)
 */
#define TWI_WRITE 0
#define TWI_READ  1

/* For backwards compatibility */
#define TWI_F_START TWI_F_S
#define TWI_F_STOP  TWI_F_P
#define TWI_F_GC    TWI_F_G

/* Constants: TWI START/STOP/RESTART, GCALL
 *
 * TWI_F_S - Begin with a start/repeated start
 * TWI_F_P - End with a stop
 * TWI_F_G - General call
 * TWI_F_N(x) - Limits the current call transfer to n bytes
 */
#define TWI_F_S    (1 << 10)
#define TWI_F_P    (1 << 9)
#define TWI_F_G    (1 << 8)
#define TWI_F_N_M  0xff
#define TWI_F_N(x) ((x) & TWI_F_N_M)

/* Constants: TWI addresses
 *
 * TWI_ADDR_GENERAL - General call address (0)
 * TWI_ADDR_COUNT - Maximum number of addresses supported (128)
 */
#define TWI_ADDR_GENERAL 0
#define TWI_ADDR_COUNT   128

struct twi_settings {
    unsigned long bitrate;      /* 0 = ignore */
    twi_mode_t mode;
    twi_addr_t slave_addr;
};

/**
 * int **twi_setup**(**struct twi** \*<ins>ctx</ins>, 
 * const **struct twi_settings** \*<ins>settings</ins>);
 * > Configures a TWI interface
 * ### RETURN
 * Returns 0 in case of success, -errno otherwise
 */
int twi_setup(struct twi *ctx, const struct twi_settings *settings);

/**
 * int **twi_poll**(**struct twi** \*<ins>ctx<ins>);
 * > Polls TWI interface for transfer (slave mode)
 * ### RETURN
 * > Returns:
 * > - TWI_READ if a read is recv
 * > - TWI_WRITE if write is recv
 * > - -errno otherwise
 */
int twi_poll(struct twi *ctx);

/**
 * int **twi_write**(**struct twi** \*<ins>ctx</ins>, const **void** \*<ins>buf</ins>, 
 * **size_t** <ins>n</ins>, **int** <ins>flags</ins>);
 * > Write data to a TWI interface
 * ### RETURN
 * Returns the number of byte written if success, -errno otherwise
 */
int twi_write(struct twi *ctx, const void *buf, size_t n, int flags);

/**
 * int **twi_read**(**struct twi** \*<ins>ctx</ins>, **void** \*<ins>buf</ins>, 
 * **size_t** <ins>n</ins>, **int** <ins>flags</ins>);
 * > Read data from a TWI interface
 * ### RETURN
 * > Returns the number of byte read if success, -errno otherwise
 */
int twi_read(struct twi *ctx, void *buf, size_t n, int flags);

#endif

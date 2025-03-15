#ifndef LIN_H
#define LIN_H

#include <errno.h>
#include <stdint.h>
#include <stddef.h>

#define LIN_ID_COUNT    0x40
#define LIN_FRAME_COUNT 8 /* max */

struct lin;

typedef enum {
    LIN_MODE_IGNORE,
    LIN_MODE_MASTER,
    LIN_MODE_SLAVE,
    LIN_MODE_COUNT
} lin_mode_t;

struct lin_settings {
    unsigned long bitrate; /* 0 = ignore */
    lin_mode_t mode;
    /* TODO: break length */
};

/* Function: lin_setup
 * Configures a LIN interface
 *
 * Parameters:
 *  ctx - the LIN interface to configure
 *  settings - The settings to apply
 *
 * Returns:
 *  if success, -errno otherwise
 */
int lin_setup(struct lin *ctx, const struct lin_settings *settings);

/* Function: lin_write
 * Writes data to the LIN interface
 *
 * Parameters:
 *  ctx - The LIN interface to use
 *  id - The ID to transfer (ignored in slave mode)
 *  buf - A pointer to the data to transfer
 *  n - The size of the data (in bytes, should be <= LIN_FRAME_COUNT)
 *
 * Returns:
 * The number of bytes written if success, -errno otherwise
 */
int lin_write(struct lin *ctx, uint8_t id, const void *buf, size_t n);

/* Function: lin_read
 * Reads data from a LIN interface
 *
 * Parameters:
 *  ctx - The LIN interface to use
 *  id - A pointer to the ID to send (master) or to the received id (slave)
 *  buf - A buffer where the read data will be stored
 *  n - The size of the data to read (in bytes)
 *
 * Returns:
 * The number of bytes read if success, -errno otherwise
 */
int lin_read(struct lin *ctx, uint8_t *id, void *buf, size_t n);

#endif

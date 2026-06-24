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

/**
 * int **lin_setup**(**struct lin** \*<ins>ctx</ins>, 
 * const **struct lin_settings** \*<ins>settings</ins>);
 * > Configures a LIN interface
 *
 * ### RETURN
 * Returns 0 if success, -errno otherwise
 */
int lin_setup(struct lin *ctx, const struct lin_settings *settings);

/**
 * int **lin_write**(**struct lin** \*<ins>ctx</ins>, **uint8_t** <ins>id</ins>,
 * const **void** \*<ins>buf</ins>, **size_t** <ins>n</ins>);
 * > Writes data to the LIN interface
 *
 * ### RETURN
 * > Returns the number of bytes written if success, -errno otherwise
 */
int lin_write(struct lin *ctx, uint8_t id, const void *buf, size_t n);

/**
 * int **lin_read**(**struct lin** \*<ins>ctx</ins>, **uint8_t** \*<ins>id</ins>,
 * **void** \*<ins>buf</ins>, **size_t** <ins>n</ins>);
 * > Reads data from a LIN interface
 *
 * ### RETURN
 * Returns the number of bytes read if success, -errno otherwise
 */
int lin_read(struct lin *ctx, uint8_t *id, void *buf, size_t n);

#endif

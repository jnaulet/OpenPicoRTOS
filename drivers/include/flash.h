#ifndef FLASH_H
#define FLASH_H

#include <errno.h>
#include <stddef.h>
#include "flash-attributes.h"

struct flash;

/**
 * int **flash_probe**(**struct flash** \*<ins>ctx</ins>);
 * > Probe the flash
 *
 * ### RETURN
 * > Returns 0 if success, -errno otherwise
 */
int flash_probe(struct flash *ctx);

/**
 * int **flash_get_attributes**(**struct flash** \*<ins>ctx</ins>,
 * **struct flash_attributes** \*<ins>attr</ins>);
 * > Gets the flash's attributes
 *
 * ### RETURN
 * > Returns 0 if success, -errno otherwise
 */
int flash_get_attributes(struct flash *ctx, /*@out@*/ struct flash_attributes *attr);

/**
 * int **flash_erase**(**struct flash** \*<ins>ctx</ins>, **size_t** <ins>offset</ins>);
 * > Erases a sector from flash
 *
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int flash_erase(struct flash *ctx, size_t offset);

/**
 * int **flash_write**(**struct flash** \*<ins>ctx</ins>, **size_t** <ins>offset</ins>,
 * const **void** \*<ins>data</ins>, **size_t** <ins>n</ins>);
 * > Writes data to flash
 *
 * ### RETURN
 * > Returns the number of bytes written if success, -errno otherwise
 */
int flash_write(struct flash *ctx, size_t offset, const void *data, size_t n);

/**
 * int **flash_lock**(**struct flash** \*<ins>ctx</ins>, **size_t** <ins>offset</ins>);
 * > Protects a block in flash
 *
 * ### RETURN
 * > Returns 0 if success, -errno otherwise
 */
int flash_lock(struct flash *ctx, size_t offset);

/**
 * int **flash_unlock**(**struct flash** \*<ins>ctx</ins>, **size_t** <ins>offset</ins>);
 * > Unprotects a flash block
 *
 * ### RETURN
 * > Returns 0 if success, -errno otherwise
 */
int flash_unlock(struct flash *ctx, size_t offset);

#endif

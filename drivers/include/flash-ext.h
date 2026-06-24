#ifndef FLASH_EXT_H
#define FLASH_EXT_H

#include <errno.h>
#include <stddef.h>

#include "flash-attributes.h"

struct flash_ext;

/**
 * int **flash_ext_probe**(**struct flash_ext** \*<ins>ctx</ins>);
 * > Probe the flash
 *
 * ### RETURN
 * > Returns 0 if success, -errno otherwise
 */
int flash_ext_probe(struct flash_ext *ctx);

/**
 * int **flash_ext_get_attributes**(**struct flash_ext** \*<ins>ctx</ins>,
 * **struct flash_attributes** \*<ins>attr</ins>);
 * > Gets the flash's attributes
 *
 * ### RETURN
 * > Returns 0 if success, -errno otherwise
 */
int flash_ext_get_attributes(struct flash_ext *ctx, /*@out@*/ struct flash_attributes *attr);

/**
 * int **flash_ext_erase**(**struct flash_ext** \*<ins>ctx</ins>, 
 * **size_t** <ins>offset</ins>);
 * > Erases a sector from flash
 *
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int flash_ext_erase(struct flash_ext *ctx, size_t offset);

/**
 * int **flash_ext_read**(**struct flash_ext** \*<ins>ctx</ins>, 
 * **size_t** <ins>offset</ins>, const **void** \*<ins>data</ins>,
 * **size_t** <ins>n</ins>);
 * > Reads data from flash
 *
 * ### RETURN
 * > Returns the number of bytes read if success, -errno otherwise
 */
int flash_ext_read(struct flash_ext *ctx, size_t offset, void *data, size_t n);

/**
 * int **flash_ext_write**(**struct flash_ext** \*<ins>ctx</ins>, 
 * **size_t** <ins>offset</ins>, const **void** \*<ins>data</ins>, 
 * **size_t** <ins>n</ins>);
 * > Writes data to flash
 *
 * ### RETURN
 * > Returns the number of bytes written if success, -errno otherwise
 */
int flash_ext_write(struct flash_ext *ctx, size_t offset, const void *data, size_t n);

/**
 * int **flash_ext_lock**(**struct flash_ext** \*<ins>ctx</ins>,
 * **size_t** <ins>offset</ins>);
 * > Protects a block in flash
 *
 * ### RETURN
 * > Returns 0 if success, -errno otherwise
 */
int flash_ext_lock(struct flash_ext *ctx, size_t offset);

/**
 * int **flash_ext_unlock**(**struct flash_ext** \*<ins>ctx</ins>,
 * **size_t** <ins>offset</ins>);
 * > Unprotects a flash block
 *
 * ### RETURN
 * > Returns 0 if success, -errno otherwise
 */
int flash_ext_unlock(struct flash_ext *ctx, size_t offset);

#endif

#ifndef FLASH_EXT_H
#define FLASH_EXT_H

#include <errno.h>
#include <stddef.h>

#include "flash-attributes.h"

struct flash_ext;

/* Function: flash_ext_probe
 * Probe the flash
 *
 * Parameters:
 *  ctx - A flash instance
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int flash_ext_probe(struct flash_ext *ctx);

/* Function: flash_ext_get_attributes
 * Gets the flash's attributes
 *
 * Parameters:
 *  ctx - A flash instance
 *  attributes - The structure that will contain the information
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int flash_ext_get_attributes(struct flash_ext *ctx, /*@out@*/ struct flash_attributes *attr);

/* Function: flash_ext_erase
 * Erases a sector from flash
 *
 * Parameters:
 *  ctx - A flash instance
 *  offset - The offset of the sector/erase_unit to erase
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int flash_ext_erase(struct flash_ext *ctx, size_t offset);

/* Function: flash_ext_read
 * Reads data from flash
 *
 * Parameters:
 *  ctx - A flash instance
 *  offset - The flash address to write to
 *  data - A buffer containing the data to write
 *  n - The size of the data to write in bytes
 *
 * Returns:
 * The number of bytes written if success, -errno otherwise
 */
int flash_ext_read(struct flash_ext *ctx, size_t offset, void *data, size_t n);

/* Function: flash_ext_write
 * Writes data to flash
 *
 * Parameters:
 *  ctx - A flash instance
 *  offset - The flash address to write to
 *  data - A buffer containing the data to write
 *  n - The size of the data to write in bytes
 *
 * Returns:
 * The number of bytes written if success, -errno otherwise
 */
int flash_ext_write(struct flash_ext *ctx, size_t offset, const void *data, size_t n);

/* Function: flash_ext_lock
 * Protects a block in flash
 *
 * Parameters:
 *  ctx - A flash instance
 *  offset - The block to protect
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int flash_ext_lock(struct flash_ext *ctx, size_t offset);

/* Function: flash_ext_unlock
 * Unprotects a flash block
 *
 * Parameters:
 *  ctx - A flash_ext instance
 *  offset - The block to unprotect
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int flash_ext_unlock(struct flash_ext *ctx, size_t offset);

#endif

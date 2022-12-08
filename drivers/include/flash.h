#ifndef FLASH_H
#define FLASH_H

#include <errno.h>
#include <stddef.h>

struct flash;

/* Function: flash_erase_sector
 * Erases a sector from flash
 *
 * Parameters:
 *  ctx - A flash instance
 *  sector - The sector to erase
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int flash_erase_sector(struct flash *ctx, size_t sector);

/* Function: flash_blankcheck_sector
 * Checks if a flash sector is blank/empty
 *
 * Parameters:
 *  ctx - A flash instance
 *  sector - The sector to blankcheck
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int flash_blankcheck_sector(struct flash *ctx, size_t sector);

/* Function: flash_write
 * Writes data to flash
 *
 * Parameters:
 *  ctx - A flash instance
 *  addr - The flash address to write to
 *  data - A buffer containing the data to write
 *  n - The size of the data to write in bytes
 *
 * Returns:
 * The number of bytes written if success, -errno otherwise
 */
int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n);

/* Function: flash_lock_sector
 * Protects a sector in flash
 *
 * Parameters:
 *  ctx - A flash instance
 *  sector - The sector to protect
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int flash_lock_sector(struct flash *ctx, size_t sector);

/* Function: flash_unlock_sector
 * Unprotects a flash sector
 *
 * Parameters:
 *  ctx - A flash instance
 *  sector - The sector to unprotect
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int flash_unlock_sector(struct flash *ctx, size_t sector);

#endif

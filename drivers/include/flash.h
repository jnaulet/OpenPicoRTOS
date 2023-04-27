#ifndef FLASH_H
#define FLASH_H

#include <errno.h>
#include <stddef.h>

struct flash;

/* Function: flash_get_nblocks
 * Gets the number of blocks present in the flash
 *
 * Parameters:
 *  ctx - A flash instance
 *
 * Returns:
 * The numbers of blocks if success, -errno otherwise
 */
int flash_get_nblocks(struct flash *ctx);

/* Function: flash_get_erase_size
 * Gets the minimal erase size for the specified block
 *
 * Parameters:
 *  ctx - A flash instance
 *  block - The block number
 *
 * Returns:
 * The minimum erase size for this block if success, -errno otherwise
 */
int flash_get_erase_size(struct flash *ctx, size_t block);

/* Function: flash_get_write_size
 * Gets the minimal write size for the specified block
 *
 * Parameters:
 *  ctx - A flash instance
 *  block - The block number
 *
 * Returns:
 * The minimum write size for this block if success, -errno otherwise
 */
int flash_get_write_size(struct flash *ctx, size_t block);

/* Function: flash_get_block_addr
 * Gets the memory address of the specified block
 *
 * Parameters:
 *  ctx - A flash instance
 *  block - The block number
 *
 * Returns:
 * The address of the block if success, -errno otherwise
 */
int flash_get_block_addr(struct flash *ctx, size_t block);

/* Function: flash_erase
 * Erases a sector from flash
 *
 * Parameters:
 *  ctx - A flash instance
 *  block - The block to erase
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int flash_erase(struct flash *ctx, size_t block);

/* Function: flash_blankcheck
 * Checks if a flash block is blank/empty
 *
 * Parameters:
 *  ctx - A flash instance
 *  block - The block to blankcheck
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int flash_blankcheck(struct flash *ctx, size_t block);

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

/* Function: flash_lock
 * Protects a block in flash
 *
 * Parameters:
 *  ctx - A flash instance
 *  block - The block to protect
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int flash_lock(struct flash *ctx, size_t block);

/* Function: flash_unlock
 * Unprotects a flash block
 *
 * Parameters:
 *  ctx - A flash instance
 *  block - The block to unprotect
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int flash_unlock(struct flash *ctx, size_t block);

#endif

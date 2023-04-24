#ifndef FLASH_GD32VF103_H
#define FLASH_GD32VF103_H

#include "flash.h"

struct FLASH_GD32VF103;

struct flash {
    /*@temp@*/ struct FLASH_GD32VF103 *base;
    size_t block_count;
};

int flash_gd32vf103_init(/*@out@*/ struct flash *ctx, struct FLASH_GD32VF103 *base, size_t block_count);

/* Implements:
 * int flash_erase_sector(struct flash *ctx, size_t sector);
 * int flash_blankcheck_sector(struct flash *ctx, size_t sector);
 * int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n);
 * int flash_lock_sector(struct flash *ctx, size_t sector);
 * int flash_unlock_sector(struct flash *ctx, size_t sector);
 */

#endif

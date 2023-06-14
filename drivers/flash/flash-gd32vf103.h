#ifndef FLASH_GD32VF103_H
#define FLASH_GD32VF103_H

#include "flash.h"

struct FLASH_GD32VF103;

typedef enum {
    FLASH_GD32VF103_STATE_IDLE,
    FLASH_GD32VF103_STATE_BUSY,
    FLASH_GD32VF103_STATE_COUNT,
} flash_gd32vf103_state_t;

struct flash {
    /*@temp@*/ struct FLASH_GD32VF103 *base;
    size_t block_count;
    flash_gd32vf103_state_t state;
};

int flash_gd32vf103_init(/*@out@*/ struct flash *ctx, int base, size_t block_count);

/* Implements:
 * int flash_erase_sector(struct flash *ctx, size_t sector);
 * int flash_blankcheck_sector(struct flash *ctx, size_t sector);
 * int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n);
 * int flash_lock_sector(struct flash *ctx, size_t sector);
 * int flash_unlock_sector(struct flash *ctx, size_t sector);
 */

#endif

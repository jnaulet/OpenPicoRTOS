#ifndef FLASH_SAME5X_H
#define FLASH_SAME5X_H

#include "flash.h"

struct FLASH_SAME5X;

typedef enum {
    FLASH_SAME5X_STATE_IDLE,
    FLASH_SAME5X_STATE_BUSY,
    FLASH_SAME5X_STATE_COUNT
} flash_same5x_state_t;

struct flash {
    /*@temp@*/ struct FLASH_SAME5X *base;
    flash_same5x_state_t state;
    /* info */
    size_t page_size;   /* from param.psz */
    size_t page_count;  /* form param.nvmp */
    size_t block_size;
    size_t block_count;
};

int flash_same5x_init(/*@out@*/ struct flash *ctx, struct FLASH_SAME5X *base);

/* Implements:
 * int flash_erase_sector(struct flash *ctx, size_t sector);
 * int flash_blankcheck_sector(struct flash *ctx, size_t sector);
 * int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n);
 * int flash_lock_sector(struct flash *ctx, size_t sector);
 * int flash_unlock_sector(struct flash *ctx, size_t sector);
 */

#endif

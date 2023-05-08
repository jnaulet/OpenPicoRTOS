#ifndef FLASH_SAM3X_H
#define FLASH_SAM3X_H

#include "flash.h"

struct FLASH_SAM3X;

typedef enum {
    FLASH_SAM3X_STATE_IDLE,
    FLASH_SAM3X_STATE_BUSY,
    FLASH_SAM3X_STATE_COUNT
} flash_sam3x_state_t;

struct flash {
    /*@temp@*/ struct FLASH_SAM3X *base;
    flash_sam3x_state_t state;
    /* info */
    size_t page_size;
    size_t page_count;
};

int flash_sam3x_init(/*@out@*/ struct flash *ctx, int base);

/* Implements:
 * int flash_erase_sector(struct flash *ctx, size_t sector);
 * int flash_blankcheck_sector(struct flash *ctx, size_t sector);
 * int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n);
 * int flash_lock_sector(struct flash *ctx, size_t sector);
 * int flash_unlock_sector(struct flash *ctx, size_t sector);
 */


#endif

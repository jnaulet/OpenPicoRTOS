#ifndef FLASH_PIC32MX_H
#define FLASH_PIC32MX_H

#include "flash.h"

struct FLASH_PIC32MX;

typedef enum {
    FLASH_PIC32MX_STATE_IDLE,
    FLASH_PIC32MX_STATE_BUSY,
    FLASH_PIC32MX_STATE_COUNT
} flash_pic32mx_state_t;

struct flash {
    /*@temp@*/ struct FLASH_PIC32MX *base;
    flash_pic32mx_state_t state;
    size_t block_count;
};

int flash_pic32mx_init(/*@out@*/ struct flash *ctx, struct FLASH_PIC32MX *base, size_t block_count);

/* Implements:
 * int flash_erase_sector(struct flash *ctx, size_t sector);
 * int flash_blankcheck_sector(struct flash *ctx, size_t sector);
 * int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n);
 * int flash_lock_sector(struct flash *ctx, size_t sector);
 * int flash_unlock_sector(struct flash *ctx, size_t sector);
 */

#endif

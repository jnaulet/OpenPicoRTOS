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

int flash_pic32mx_init(/*@out@*/ struct flash *ctx, int base, size_t block_count);
int flash_get_pic32mx_block_addr(struct flash *ctx, size_t block);
int flash_pic32mx_blankcheck(struct flash *ctx, size_t block);

/* Implements:
 * int flash_get_attributes(struct flash *ctx, struct flash_attributes *attr);
 * int flash_erase(struct flash *ctx, size_t block);
 * int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n);
 * int flash_lock(struct flash *ctx, size_t block);
 * int flash_unlock(struct flash *ctx, size_t block);
 */

#endif

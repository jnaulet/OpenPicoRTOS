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
    struct flash_attributes attr;
};

int flash_same5x_init(/*@out@*/ struct flash *ctx, int base);

/* Implements:
 * int flash_probe(struct flash *ctx);
 * int flash_get_attributes(struct flash *ctx, struct flash_attributes *attr);
 * int flash_erase(struct flash *ctx, size_t offset);
 * int flash_write(struct flash *ctx, size_t offset, const void *data, size_t n);
 * int flash_lock(struct flash *ctx, size_t offset);
 * int flash_unlock(struct flash *ctx, size_t offset);
 */

#endif

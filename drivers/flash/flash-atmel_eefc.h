#ifndef FLASH_ATMEL_EEFC_H
#define FLASH_ATMEL_EEFC_H

#include "flash.h"
#include <stdbool.h>

struct FLASH_ATMEL_EEFC;

struct flash {
    /*@temp@*/ struct FLASH_ATMEL_EEFC *base;
    struct flash_attributes attr;
    bool pending;
};

int flash_atmel_eefc_init(/*@out@*/ struct flash *ctx, int base);
int flash_atmel_eefc_set_fws(struct flash *ctx, size_t fws);

/* TODO: implement
 * int flash_probe(struct flash *ctx);
 * int flash_get_attributes(struct flash *ctx, struct flash_attributes *attr);
 * int flash_erase(struct flash *ctx, size_t offset);
 * int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n);
 * int flash_lock(struct flash *ctx, size_t offset);
 * int flash_unlock(struct flash *ctx, size_t offset);
 */

#endif

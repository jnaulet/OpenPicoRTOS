#ifndef FLASH_ATMEL_EEFC_H
#define FLASH_ATMEL_EEFC_H

#include "flash.h"

struct FLASH_ATMEL_EEFC;

struct flash {
    /*@temp@*/ struct FLASH_ATMEL_EEFC *base;
};

int flash_atmel_eefc_init(/*@out@*/ struct flash *ctx, int base);
int flash_atmel_eefc_set_fws(struct flash *ctx, size_t fws);

/* TODO: implement picoRTOS' flash API
 * int flash_erase_sector(struct flash *ctx, size_t sector);
 * int flash_blankcheck_sector(struct flash *ctx, size_t sector);
 * int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n);
 * int flash_lock_sector(struct flash *ctx, size_t sector);
 * int flash_unlock_sector(struct flash *ctx, size_t sector);
 */

#endif

#ifndef FLASH_EXT_S25FL1_H
#define FLASH_EXT_S25FL1_H

#include "flash-ext.h"
#include "spi.h"

#include <stdint.h>
#include <stdbool.h>

struct flash_ext {
    /*@temp@*/ struct spi *spi;
    uint8_t jedec[3];
    struct flash_attributes attr;
    /* spi xfers */
    size_t left;
    int seq;
};

int flash_ext_s25fl1_init(/*@out@*/ struct flash_ext *ctx, struct spi *spi);
int flash_ext_s25fl1_reset(struct flash_ext *ctx);
int flash_ext_s25fl1_quad_enable(struct flash_ext *ctx, bool enable);

/* Implements:
 * int flash_ext_probe(struct flash_ext *ctx);
 * int flash_ext_get_attributes(struct flash_ext *ctx, struct flash_ext_attributes *attr);
 * int flash_ext_erase(struct flash_ext *ctx, size_t offset);
 * int flash_ext_read(struct flash_ext *ctx, size_t addr, void *data, size_t n);
 * int flash_ext_write(struct flash_ext *ctx, size_t addr, const void *data, size_t n);
 * int flash_ext_lock(struct flash_ext *ctx, size_t offset);
 * int flash_ext_unlock(struct flash_ext *ctx, size_t offset);
 */

#endif

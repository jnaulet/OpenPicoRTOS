#ifndef FLASH_EXT_SST26VF_H
#define FLASH_EXT_SST26VF_H

#include "flash-ext.h"
#include "spi.h"

#include <stdint.h>
#include <stdbool.h>

#define FLASH_EXT_SST26VF_RD_LEN 256
#define FLASH_EXT_SST26VF_PP_LEN 256

struct flash_ext {
    /*@temp@*/ struct spi *spi;
    uint8_t jedec[4];
    struct flash_attributes attr;
    /* spi xfers */
    size_t left;
    int seq;
    /* read/write */
    uint8_t rd[FLASH_EXT_SST26VF_RD_LEN + 4];
    uint8_t pp[FLASH_EXT_SST26VF_PP_LEN + 4];
};

int flash_ext_sst26vf_init(/*@out@*/ struct flash_ext *ctx, struct spi *spi);
int flash_ext_sst26vf_reset(struct flash_ext *ctx);
int flash_ext_sst26vf_quad_enable(struct flash_ext *ctx, bool enable);

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

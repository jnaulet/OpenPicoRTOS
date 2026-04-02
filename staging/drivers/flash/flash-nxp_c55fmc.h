#ifndef FLASH_NXP_C55FMC_H
#define FLASH_NXP_C55FMC_H

#include "flash.h"

typedef enum {
    FLASH_NXP_C55FMC_STATE_IDLE,
    FLASH_NXP_C55FMC_STATE_BUSY,
    FLASH_NXP_C55FMC_STATE_COUNT
} flash_nxp_c55fmc_state;

struct FLASH_NXP_C55FMC;

struct flash {
    /*@temp@*/ struct FLASH_NXP_C55FMC *base;
    struct flash_attributes attr;
    flash_nxp_c55fmc_state state;
    int last;
};

int flash_nxp_c55fmc_init(/*@out@*/ struct flash *ctx, int base);
int flash_nxp_c55fmc_get_block_addr(struct flash *ctx, size_t block);
int flash_nxp_c55fmc_blankcheck(struct flash *ctx, size_t block);

#endif

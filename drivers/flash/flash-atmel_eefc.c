#include "flash-atmel_eefc.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define FCMD_GETD  0x00 /* get flash descriptor */
#define FCMD_WP    0x01 /* write page */
#define FCMD_WPL   0x02 /* write page and lock */
#define FCMD_EWP   0x03 /* erase page and write page */
#define FCMD_EWPL  0x04 /* erase page and write page then lock */
#define FCMD_EA    0x05 /* erase all */
#define FCMD_EPA   0x07 /* erase pages */
#define FCMD_SLB   0x08 /* set lock bit */
#define FCMD_CLB   0x09 /* clear lock bit */
#define FCMD_GLB   0x0a /* get lock bit */
#define FCMD_SGPB  0x0b /* set gpnvm bit */
#define FCMD_CGPB  0x0c /* clear gpnvm bit */
#define FCMD_GGPB  0x0d /* get gpnvm bit */
#define FCMD_STUI  0x0e /* start read unique identifier */
#define FCMD_SPUI  0x0f /* stop read unique identifier */
#define FCMD_GCALB 0x10 /* get calib bit */
#define FCMD_ES    0x11 /* erase sector */
#define FCMD_WUS   0x12 /* write user signature */
#define FCMD_EUS   0x13 /* erase user signature */
#define FCMD_STUS  0x14 /* start read user signature */
#define FCMD_SPUS  0x15 /* stop read user signature */

struct FLASH_ATMEL_EEFC {
    volatile uint32_t EEFC_FMR;
    volatile uint32_t EEFC_FCR;
    volatile uint32_t EEFC_FSR;
    volatile uint32_t EEFC_FRR;
    uint32_t RESERVED0[53];
    volatile uint32_t EEFC_WPMR;
};

/*@-namechecks@*/
#define EEFC_FMR_CLOE   (1 << 26)
#define EEFC_FMR_SCOD   (1 << 16)
#define EEFC_FMR_FWS_M  0xfu
#define EEFC_FMR_FWS(x) (((x) & EEFC_FMR_FWS_M) << 8)
#define EEFC_FMR_FRDY   (1 << 0)

#define EEFC_FCR_FKEY_M  0xffu
#define EEFC_FCR_FKEY(x) (((x) & EEFC_FCR_FKEY_M) << 24)
#define EEFC_FCR_FARG_M  0xffffu
#define EEFC_FCR_FARG(x) (((x) & EEFC_FCR_FARG_M) << 8)
#define EEFC_FCR_FCMD_M  0xffu
#define EEFC_FCR_FCMD(x) ((x) & EEFC_FCR_FCMD_M)

#define EEFC_FSR_MECCEMSB (1 << 19)
#define EEFC_FSR_UECCEMSB (1 << 18)
#define EEFC_FSR_MECCELSB (1 << 17)
#define EEFC_FSR_UECCELSB (1 << 16)
#define EEFC_FSR_FLERR    (1 << 3)
#define EEFC_FSR_FLOCKE   (1 << 2)
#define EEFC_FSR_FCMDE    (1 << 1)
#define EEFC_FSR_FRDY     (1 << 0)

#define EEFC_WPMR_WPKEY_M  0xffffffu
#define EEFC_WPMR_WPKEY(x) (((x) & EEFC_WPMR_WPKEY_M) << 8)
#define EEFC_WPMR_WPEN     (1 << 0)
/*@=namechecks@*/

/* Function: flash_atmel_eefc_init
 * Initializes a NVMCTRL flash controller
 *
 * Parameters:
 *  ctx - The flash controller to init
 *  base - The flash controller base address
 *
 * Returns:
 * Always 0
 */
int flash_atmel_eefc_init(struct flash *ctx, int base)
{
    ctx->base = (struct FLASH_ATMEL_EEFC*)base;
    return 0;
}

/* Function: flash_atmel_eefc_set_fws
 * Set the Flash Wait State parameter (see your chip doc for more info)
 *
 * Parameters:
 *  ctx - The flash controller to use
 *  fws - The FWS to set
 *
 * Returns:
 * Always 0
 */
int flash_atmel_eefc_set_fws(struct flash *ctx, size_t fws)
{
    picoRTOS_assert(fws > 0, return -EINVAL);
    picoRTOS_assert(fws < (size_t)(EEFC_FMR_FWS_M + 1), return -EINVAL);

    ctx->base->EEFC_FMR = (uint32_t)((ctx->base->EEFC_FMR & ~EEFC_FMR_FWS(EEFC_FMR_FWS_M)) |
                                     EEFC_FMR_FWS(fws - 1));

    return 0;
}

/* FIXME: implement */
/*@external@*/ extern int flash_get_nblocks(struct flash *ctx);
/*@external@*/ extern int flash_get_erase_size(struct flash *ctx, size_t block);
/*@external@*/ extern int flash_get_erase_size(struct flash *ctx, size_t block);
/*@external@*/ extern int flash_get_write_size(struct flash *ctx, size_t block);
/*@external@*/ extern int flash_get_block_addr(struct flash *ctx, size_t block);
/*@external@*/ extern int flash_erase(struct flash *ctx, size_t block);
/*@external@*/ extern int flash_blankcheck(struct flash *ctx, size_t block);
/*@external@*/ extern int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n);
/*@external@*/ extern int flash_lock(struct flash *ctx, size_t block);
/*@external@*/ extern int flash_unlock(struct flash *ctx, size_t block);

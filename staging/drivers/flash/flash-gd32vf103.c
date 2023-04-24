#include "flash-gd32vf103.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#ifndef FLASH_GD32VF103_ADDR_START
# error FLASH_GD32VF103_ADDR_START is not defined in picoRTOS_device.h
#endif

#define FLASH_GD32VF103_BLOCK_SIZE 1024

#define FMC_KEY1 0x45670123
#define FMC_KEY2 0xcdef89ab

struct FLASH_GD32VF103 {
    volatile uint32_t FMC_WS;
    volatile uint32_t FMC_KEY;
    volatile uint32_t FMC_OBKEY;
    volatile uint32_t FMC_STAT;
    volatile uint32_t FMC_CTL;
    volatile uint32_t FMC_ADDR;
    volatile uint32_t FMC_OBSTAT;
    volatile uint32_t FMC_WP;
    uint32_t RESERVED0[55];
    volatile uint32_t FMC_PID;
};

#define FMC_WS_WSCNT_M  0x7u
#define FMC_WS_WSCNT(x) ((x) & FMC_WS_WSCNT_M)

#define FMC_STAT_ENDF  (1 << 5)
#define FMC_STAT_WPERR (1 << 4)
#define FMC_STAT_PGERR (1 << 2)
#define FMC_STAT_BUSY  (1 << 0)

#define FMC_CTL_ENDIE (1 << 12)
#define FMC_CTL_ERRIE (1 << 11)
#define FMC_CTL_OBWEN (1 << 9)
#define FMC_CTL_LK    (1 << 7)
#define FMC_CTL_START (1 << 6)
#define FMC_CTL_OBER  (1 << 5)
#define FMC_CTL_OBPG  (1 << 4)
#define FMC_CTL_MER   (1 << 2)
#define FMC_CTL_PER   (1 << 1)
#define FMC_CTL_PG    (1 << 0)

#define FMC_OBSTAT_DATA_M  0xffffu
#define FMC_OBSTAT_DATA(x) (((x) & FMC_OBSTAT_DATA_M) << 10)
#define FMC_OBSTAT_USER_M  0xffu
#define FMC_OBSTAT_USER(x) (((x) & FMC_OBSTAT_USER_M) << 2)
#define FMC_OBSTAT_SPC     (1 << 1)
#define FMC_OBSTAT_OBERR   (1 << 0)

int flash_gd32vf103_init(struct flash *ctx, struct FLASH_GD32VF103 *base, size_t block_count)
{
    ctx->base = base;
    ctx->block_count = block_count;

    return 0;
}

int flash_get_nblocks(struct flash *ctx)
{
    return (int)ctx->block_count;
}

int flash_get_erase_size(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;
    return FLASH_GD32VF103_BLOCK_SIZE;
}

int flash_get_write_size(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;
    return (int)sizeof(uint32_t);
}

int flash_get_block_addr(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;
    return FLASH_GD32VF103_ADDR_START + (int)block * FLASH_GD32VF103_BLOCK_SIZE;
}

static void unlock_fmc_ctl(struct flash *ctx)
{
    ctx->base->FMC_KEY = (uint32_t)FMC_KEY1;
    ctx->base->FMC_KEY = (uint32_t)FMC_KEY2;
}

static void lock_fmc_ctl(struct flash *ctx)
{
    ctx->base->FMC_CTL |= FMC_CTL_LK;
}

int flash_erase(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    if ((ctx->base->FMC_STAT & FMC_STAT_BUSY) != 0)
        return -EAGAIN;

    unlock_fmc_ctl(ctx);

    /* prepare erase */
    ctx->base->FMC_CTL |= FMC_CTL_PER;
    ctx->base->FMC_ADDR = (uint32_t)flash_get_block_addr(ctx, block);

    /* start erase */
    ctx->base->FMC_CTL |= FMC_CTL_START;

    lock_fmc_ctl(ctx);
    return 0;
}

int flash_blankcheck(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    uint32_t *addr = (uint32_t*)flash_get_block_addr(ctx, block);
    size_t n = (size_t)FLASH_GD32VF103_BLOCK_SIZE / sizeof(uint32_t);

    if ((ctx->base->FMC_STAT & FMC_STAT_BUSY) != 0)
        return -EAGAIN;

    while (n-- != 0)
        if (*addr != (uint32_t)-1)
            return -EFAULT;

    return 0;
}

int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert((n % sizeof(uint32_t)) == 0)) return -EINVAL;

    int nwritten = 0;
    uint8_t *dst8 = (uint8_t*)addr;
    const uint8_t *data8 = (const uint8_t*)data;

    if ((ctx->base->FMC_STAT & FMC_STAT_BUSY) != 0)
        return -EAGAIN;

    /* prepare programming */
    unlock_fmc_ctl(ctx);
    ctx->base->FMC_CTL |= FMC_CTL_PG;
    lock_fmc_ctl(ctx);

    /* flash */
    while (nwritten != (int)n) {
        if ((ctx->base->FMC_STAT & FMC_STAT_BUSY) != 0)
            break;

        *(uint32_t*)&dst8[nwritten] = *(uint32_t*)&data8[nwritten];
        nwritten += sizeof(uint32_t);
    }

    if (nwritten == 0)
        return -EAGAIN;

    return nwritten;
}

int flash_lock(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;
    return -ENOSYS;
}

int flash_unlock(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;
    return -ENOSYS;
}

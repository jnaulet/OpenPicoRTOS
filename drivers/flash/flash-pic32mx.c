#include "flash-pic32mx.h"
#include "picoRTOS.h"

#include <stdint.h>
#include "misc-pic32mx.h"

#define NVMKEY_KEY1 0xaa996655
#define NVMKEY_KEY2 0x556699aa

#define NVMOP_PROGRAM_ERASE     0x7
#define NVMOP_UPPER_FLASH_ERASE 0x6
#define NVMOP_LOWER_FLASH_ERASE 0x5
#define NVMOP_PAGE_ERASE        0x4
#define NVMOP_ROW_PROGRAM       0x3
#define NVMOP_QWORD_PROGRAM     0x2
#define NVMOP_WORD_PROGRAM      0x1

#define QWORD_SIZE 16

struct FLASH_PIC32MX {
    struct PIC32MX_CLR_SET_INV NVMCON;
    volatile uint32_t NVMKEY;
    uint32_t RESERVED0[3];
    struct PIC32MX_CLR_SET_INV NVMADDR;
    struct FLASH_PIC32MX_NVMDATA {
        volatile uint32_t NVMDATA;
        uint32_t RESERVED1[3];
    } NVMDATAx[4];
    volatile uint32_t NVMSRCADDR;
    uint32_t RESERVEDx[3];
    struct PIC32MX_CLR_SET_INV NVMPWP;
    struct PIC32MX_CLR_SET_INV NVMBWP;
    struct PIC32MX_CLR_SET_INV NVMCON2;
};

#define NVMCON_WR       (1 << 15)
#define NVMCON_WREN     (1 << 14)
#define NVMCON_WRERR    (1 << 13)
#define NVMCON_LVDERR   (1 << 12)
#define NVMCON_PFSWAP   (1 << 7)
#define NVMCON_BFSWAP   (1 << 6)
#define NVMCON_NVMOP_M  0xfu
#define NVMCON_NVMOP(x) ((x) & NVMCON_NVMOP_M)

#define NVMBWP_PWPULOCK (1u << 31)
#define NVMBWP_PWP_M    0xffffffu
#define NVMBWP_PWP(x)   ((x) & NVMBWP_PWP_M)

#define NVMBWP_LBWULOCK (1 << 15)
#define NVMBWP_LBWP4 (1 << 12)
#define NVMBWP_LBWP3 (1 << 11)
#define NVMBWP_LBWP2 (1 << 10)
#define NVMBWP_LBWP1 (1 << 9)
#define NVMBWP_LBWP0 (1 << 8)
#define NVMBWP_UBWULOCK (1 << 7)
#define NVMBWP_UBWP4 (1 << 4)
#define NVMBWP_UBWP3 (1 << 3)
#define NVMBWP_UBWP2 (1 << 2)
#define NVMBWP_UBWP1 (1 << 1)
#define NVMBWP_UBWP0 (1 << 0)

#define NVMCON2_SWAPLOCK_M  0x3u
#define NVMCON2_SWAPLOCK(x) (((x) & NVMCON2_SWAPLOCK_M) << 6)

/* Function: flash_pic32mx_init
 * Initializes a pic32mx flash controller (RTSP)
 *
 * Parameters:
 *  ctx - The flash controller to init
 *  base - The flash controller base address
 *  block_count - The number of erase blocks in the flash
 *
 * Returns:
 * Always 0
 */
int flash_pic32mx_init(struct flash *ctx, int base, size_t block_count)
{
    ctx->base = (struct FLASH_PIC32MX*)base;
    ctx->state = FLASH_PIC32MX_STATE_IDLE;
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
    return FLASH_PIC32MX_PAGE_SIZE;
}

int flash_get_write_size(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    /* FIXME: depends on ECC */
    return QWORD_SIZE;
}

int flash_get_block_addr(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;
    return FLASH_PIC32MX_ADDR_START + (int)block * FLASH_PIC32MX_PAGE_SIZE;
}

static int nvmop_idle(struct flash *ctx, int nvmop)
{
    if (!picoRTOS_assert(nvmop <= NVMOP_PROGRAM_ERASE)) return -EINVAL;

    register uint32_t x = (uint32_t)NVMKEY_KEY1;
    register uint32_t y = (uint32_t)NVMKEY_KEY2;
    register uint32_t z = (uint32_t)NVMCON_WR;

    register volatile uint32_t *NVMKEY = &ctx->base->NVMKEY;
    register volatile uint32_t *NVMCONSET = &ctx->base->NVMCON.SET;

    ctx->base->NVMCON.REG = (uint32_t)(NVMCON_WREN | NVMCON_NVMOP(nvmop));
    if ((ctx->base->NVMCON.REG & NVMCON_LVDERR) != 0)
        return -EAGAIN;

    *NVMKEY = x;
    *NVMKEY = y;
    *NVMCONSET = z;

    ctx->state = FLASH_PIC32MX_STATE_BUSY;
    return -EAGAIN;
}

static int nvmop_busy(struct flash *ctx)
{
    if ((ctx->base->NVMCON.REG & NVMCON_WR) != 0)
        return -EAGAIN;

    /* clear wren flag */
    ctx->base->NVMCON.CLR = (uint32_t)NVMCON_WREN;
    ctx->state = FLASH_PIC32MX_STATE_IDLE;

    if ((ctx->base->NVMCON.REG & NVMCON_WRERR) != 0 ||
        (ctx->base->NVMCON.REG & NVMCON_LVDERR) != 0)
        return -EFAULT;

    return 0;
}

int flash_erase(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    size_t addr = (size_t)flash_get_block_addr(ctx, block);

    switch (ctx->state) {
    case FLASH_PIC32MX_STATE_IDLE:
        ctx->base->NVMADDR.REG = (uint32_t)addr;
        return nvmop_idle(ctx, NVMOP_PAGE_ERASE);

    case FLASH_PIC32MX_STATE_BUSY:
        return nvmop_busy(ctx);

    default:
        break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int flash_blankcheck(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    size_t n = (size_t)FLASH_PIC32MX_PAGE_SIZE / sizeof(uint32_t);
    size_t addr = (size_t)flash_get_block_addr(ctx, block);
    uint32_t *flash = (uint32_t*)PA_TO_KVA(addr, 0xa0000000); /* FIXME */

    while (n-- != 0)
        if (*flash++ != (uint32_t)-1)
            return -EFAULT;

    return 0;
}

static int flash_write_row(struct flash *ctx, const void *data)
{
    int res;

    switch (ctx->state) {
    case FLASH_PIC32MX_STATE_IDLE:
        ctx->base->NVMSRCADDR = (uint32_t)KVA_TO_PA(data);
        return nvmop_idle(ctx, NVMOP_ROW_PROGRAM);

    case FLASH_PIC32MX_STATE_BUSY:
        if ((res = nvmop_busy(ctx)) < 0) return res;
        return FLASH_PIC32MX_ROW_SIZE;

    default:
        break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int flash_write_qword(struct flash *ctx, const void *data)
{
    int res;
    const uint32_t *data32 = (const uint32_t*)data;

    switch (ctx->state) {
    case FLASH_PIC32MX_STATE_IDLE:
        ctx->base->NVMDATAx[0].NVMDATA = data32[0];
        ctx->base->NVMDATAx[1].NVMDATA = data32[1];
        ctx->base->NVMDATAx[2].NVMDATA = data32[2];
        ctx->base->NVMDATAx[3].NVMDATA = data32[3];
        return nvmop_idle(ctx, NVMOP_QWORD_PROGRAM);

    case FLASH_PIC32MX_STATE_BUSY:
        if ((res = nvmop_busy(ctx)) < 0) return res;
        return QWORD_SIZE;

    default:
        break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int flash_write_word(struct flash *ctx, const void *data)
{
    int res;
    const uint32_t *data32 = (const uint32_t*)data;

    switch (ctx->state) {
    case FLASH_PIC32MX_STATE_IDLE:
        ctx->base->NVMDATAx[0].NVMDATA = *data32;
        return nvmop_idle(ctx, NVMOP_WORD_PROGRAM);

    case FLASH_PIC32MX_STATE_BUSY:
        if ((res = nvmop_busy(ctx)) < 0) return res;
        return (int)sizeof(uint32_t);

    default:
        break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert((n % sizeof(uint32_t)) == 0)) return -EINVAL;

    int nwritten = 0;
    int res = -EAGAIN;
    const uint8_t *data8 = (const uint8_t*)data;

    while (n != 0) {
        /* set physical address */
        ctx->base->NVMADDR.REG = (uint32_t)KVA_TO_PA(addr + nwritten);

        /* complete row */
        if ((addr % FLASH_PIC32MX_ROW_SIZE) == 0 && n >= (size_t)FLASH_PIC32MX_ROW_SIZE)
            res = flash_write_row(ctx, &data8[nwritten]);
        /* 128bit word */
        else if ((addr % QWORD_SIZE) == 0 && n >= (size_t)QWORD_SIZE)
            res = flash_write_qword(ctx, &data8[nwritten]);
        /* 32bit word. Beware, this will result in NOP if ECC is activated */
        else res = flash_write_word(ctx, &data8[nwritten]);

        /* check errors & so */
        if (res < 0)
            break;

        n -= (size_t)res;
        nwritten += res;
    }

    if (nwritten == 0)
        return res;

    return nwritten;
}

int flash_lock(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    /* sector + 1 to include the selected one */
    size_t addr = (size_t)flash_get_block_addr(ctx, block + 1);

    ctx->base->NVMBWP.CLR = (uint32_t)NVMBWP_PWP(NVMBWP_PWP_M);
    ctx->base->NVMBWP.SET = (uint32_t)NVMBWP_PWP(addr);

    return 0;
}

int flash_unlock(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    size_t addr = (size_t)flash_get_block_addr(ctx, block);

    ctx->base->NVMBWP.CLR = (uint32_t)NVMBWP_PWP(NVMBWP_PWP_M);
    ctx->base->NVMBWP.SET = (uint32_t)NVMBWP_PWP(addr);

    return 0;
}

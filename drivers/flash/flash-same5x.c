#include "flash-same5x.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define QWORD_SIZE      16
#define PAGES_PER_BLOCK 16

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#define CMDEX_KEY 0xa5

#define CMD_EP       0x0    /* erase page */
#define CMD_EB       0x1    /* erase block */
#define CMD_WP       0x3    /* write page */
#define CMD_WQW      0x4    /* write quad word */
#define CMD_SWRST    0x10   /* software reset */
#define CMD_LR       0x11   /* lock region */
#define CMD_UR       0x12   /* unlock region */
#define CMD_SPRM     0x13   /* set power reduction mode */
#define CMD_CPRM     0x14   /* clear power reduction mode */
#define CMD_PBC      0x15   /* page buffer clear */
#define CMD_SSB      0x16   /* set security bit */
#define CMD_BKSWRST  0x17   /* bank swap and system reset */
#define CMD_CELCK    0x18   /* chip erase lock */
#define CMD_CEULCK   0x19   /* chip erase unlock */
#define CMD_SBPDIS   0x1a   /* set status.bpdis */
#define CMD_CBPDIS   0x1b   /* clear status.bpdis */
#define CMD_ASEES0   0x30   /* use smarteeprom sector 0 */
#define CMD_ASEES1   0x31   /* use smarteeprom sector 1 */
#define CMD_SEERALOC 0x32   /* start smarteeprom relocation alogrithm */
#define CMD_SEEFLUSH 0x33   /* flush smarteeprom data */
#define CMD_LSEE     0x34   /* lock access to smarteeprom data */
#define CMD_USEE     0x35   /* unlock access to smarteeprom data */
#define CMD_LSEER    0x36   /* lock access to smarteeprom RAS */
#define CMD_USEER    0x34   /* unlock access to smarteeprom RAS */

struct FLASH_SAME5X {
    volatile uint32_t CTRLA;
    volatile uint32_t CTRLB;
    volatile uint32_t PARAM;
    volatile uint16_t INTENCLR;
    volatile uint16_t INTENSET;
    volatile uint16_t INTFLAG;
    volatile uint16_t STATUS;
    volatile uint32_t ADDR;
    volatile uint32_t RUNLOCK;
    volatile uint32_t PBLDATA0;
    volatile uint32_t PBLDATA1;
    volatile uint32_t ECCERR;
    volatile uint8_t DBGCTRL;
    uint8_t RESERVED0;
    volatile uint8_t SEECFG;
    uint8_t RESERVED1;
    volatile uint32_t SEESTAT;
};

#define CTRLA_CACHEDIS1 (1 << 15)
#define CTRLA_CACHEDIS0 (1 << 14)
#define CTRLA_AHBNS1    (1 << 13)
#define CTRLA_AHBNS0    (1 << 12)
#define CTRLA_RWS_M     0xfu
#define CTRLA_RWS(x)    (((x) & CTRLA_RWS_M) << 8)
#define CTRLA_PRM_M     0x3u
#define CTRLA_PRM(x)    (((x) & CTRLA_PRM_M << 6)
#define CTRLA_WMODE_M   0x3u
#define CTRLA_WMODE(x)  (((x) & CTRLA_WMODE_M) << 4)
#define CTRLA_SUSPEN    (1 << 3)
#define CTRLA_AUTOWS    (1 << 2)

#define CTRLB_CMDEX_M  0xffu
#define CTRLB_CMDEX(x) (((x) & CTRLB_CMDEX_M) << 8)
#define CTRLB_CMD_M    0x7fu
#define CTRLB_CMD(x)   ((x) & CTRLB_CMD_M)

#define PARAM_SEE     (1u << 31)
#define PARAM_PSZ_M   0x7u
#define PARAM_PSZ(x)  (((x) & PARAM_PSZ_M) << 16)
#define PARAM_NVMP_M  0xffffu
#define PARAM_NVMP(x) ((x) & PARAM_NVMP_M)

#define INTFLAG_SEEWRC   (1 << 10)
#define INTFLAG_SEESOVF  (1 << 9)
#define INTFLAG_SEESFULL (1 << 8)
#define INTFLAG_SUSP     (1 << 7)
#define INTFLAG_NVME     (1 << 6)
#define INTFLAG_ECCDE    (1 << 5)
#define INTFLAG_ECCSE    (1 << 4)
#define INTFLAG_LOCKE    (1 << 3)
#define INTFLAG_PROGE    (1 << 2)
#define INTFLAG_ADDRE    (1 << 1)
#define INTFLAG_DONE     (1 << 0)

#define STATUS_BOOTPROT_M  0xfu
#define STATUS_BOOTPROT(x) (((x) & STATUS_BOOTPROT_M) << 8)
#define STATUS_BPDIS       (1 << 5)
#define STATUS_AFIRST      (1 << 4)
#define STATUS_SUSP        (1 << 3)
#define STATUS_LOAD        (1 << 2)
#define STATUS_PRM         (1 << 1)
#define STATUS_READY       (1 << 0)

#define SEECFG_APRDIS (1 << 1)
#define SEECFG_WMODE  (1 << 0)

#define SEESTAT_PSZ_M   0x7u
#define SEESTAT_PSZ(x)  (((x) & SEESTAT_PSZ) << 16)
#define SEESTAT_SBLK_M  0xfu
#define SEESTAT_SBLK(x) ((x) & SEESTAT_SBLK_M)

/* Function: flash_same5x_init
 * Initializes a NVMCTRL flash controller
 *
 * Parameters:
 *  ctx - The flash controller to init
 *  base - The flash controller base address
 *
 * Returns:
 * Always 0
 */
int flash_same5x_init(struct flash *ctx, struct FLASH_SAME5X *base)
{
    uint32_t psz;

    ctx->base = base;
    ctx->state = FLASH_SAME5X_STATE_IDLE;

    /* extract info */
    psz = ctx->base->PARAM >> 16;
    ctx->page_size = (size_t)(0x8 << psz);
    ctx->page_count = (size_t)(ctx->base->PARAM & PARAM_NVMP_M);

    ctx->block_size = ctx->page_size * (size_t)PAGES_PER_BLOCK;
    ctx->block_count = ctx->page_count / (size_t)PAGES_PER_BLOCK;

    return 0;
}

int flash_get_nblocks(struct flash *ctx)
{
    /* TODO: AUX ? */
    return (int)ctx->block_count;
}

int flash_get_erase_size(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    /* TODO: AUX ? */
    return (int)ctx->block_size;
}

int flash_get_write_size(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    /* TODO: AUX ? */
    return (int)QWORD_SIZE;
}

int flash_get_block_addr(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    return (int)(block * ctx->block_size);
}

static int run_cmd_busy(struct flash *ctx, size_t n)
{
    if ((ctx->base->STATUS & STATUS_READY) == 0)
        return -EAGAIN;

    /* back to idle */
    ctx->state = FLASH_SAME5X_STATE_IDLE;

    if ((ctx->base->INTFLAG & INTFLAG_PROGE) != 0 ||
        (ctx->base->INTFLAG & INTFLAG_ADDRE) != 0) {
        /* clear flags */
        ctx->base->INTFLAG |= (INTFLAG_PROGE | INTFLAG_ADDRE);
        return -EFAULT;
    }

    return (int)n;
}

static int run_cmd(struct flash *ctx, int cmd, size_t addr)
{
    if (!picoRTOS_assert(cmd <= CMD_USEER)) return -EINVAL;

    switch (ctx->state) {
    case FLASH_SAME5X_STATE_IDLE:
        ctx->base->ADDR = (uint32_t)addr;
        ctx->base->CTRLB = (uint32_t)(CTRLB_CMDEX(CMDEX_KEY) | CTRLB_CMD(cmd));
        ctx->state = FLASH_SAME5X_STATE_BUSY;
        return -EAGAIN;

    case FLASH_SAME5X_STATE_BUSY:
        return run_cmd_busy(ctx, (size_t)0);

    default:
        break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int flash_erase(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    return run_cmd(ctx, CMD_EB, block * ctx->block_size);
}

int flash_blankcheck(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    size_t n = ctx->block_size / sizeof(uint32_t);
    uint32_t *addr = (uint32_t*)(block * ctx->block_size);

    while (n-- != 0)
        if (*addr++ != (uint32_t)-1)
            return -EFAULT;

    return 0;
}

static int flash_write_idle(struct flash *ctx, int cmd, size_t addr,
                            const void *data, size_t n)
{
    uint32_t *addr32 = (uint32_t*)addr;
    uint32_t *data32 = (uint32_t*)data;
    size_t count = n / sizeof(uint32_t);

    while (count-- != 0)
        *addr32++ = *data32++;

    ctx->base->ADDR = (uint32_t)addr;
    ctx->base->CTRLB = (uint32_t)(CTRLB_CMDEX(CMDEX_KEY) | CTRLB_CMD(cmd));

    ctx->state = FLASH_SAME5X_STATE_BUSY;
    return -EAGAIN;
}

static int write_data(struct flash *ctx, int cmd, size_t addr,
                      const void *data, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case FLASH_SAME5X_STATE_IDLE:
        return flash_write_idle(ctx, cmd, addr, data, n);

    case FLASH_SAME5X_STATE_BUSY:
        return run_cmd_busy(ctx, n);

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
        /* complete page */
        if ((addr % ctx->page_size) == 0 && n >= ctx->page_size)
            res = write_data(ctx, CMD_WP, addr + (size_t)nwritten,
                             &data8[nwritten], ctx->page_size);
        /* 128bit boundary */
        else if ((addr % (size_t)QWORD_SIZE) == 0 && n >= (size_t)QWORD_SIZE)
            res = write_data(ctx, CMD_WQW, addr + (size_t)nwritten,
                             &data8[nwritten], (size_t)QWORD_SIZE);

        else{
            picoRTOS_break();
            /*@notreached@*/
            return -EINVAL;
        }

        if (res < 0)
            break;

        nwritten += res;
        n -= (size_t)res;
    }

    if (nwritten == 0)
        return res;

    return nwritten;
}

int flash_lock(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    return run_cmd(ctx, CMD_LR, block * ctx->block_size);
}

int flash_unlock(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->block_count)) return -EINVAL;

    return run_cmd(ctx, CMD_UR, block * ctx->block_size);
}

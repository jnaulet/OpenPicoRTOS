#include "flash-nxp_c55fmc.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define PGM_SIZE 128

struct FLASH_NXP_C55FMC {
    volatile uint32_t MCR;
    uint32_t RESERVED0;
    volatile uint32_t MCRE;
    uint32_t RESERVED1;
    volatile uint32_t LOCK0;
    volatile uint32_t LOCK1;
    volatile uint32_t LOCK2;
    volatile uint32_t LOCK3;
    uint32_t RESERVED2[6];
    volatile uint32_t SEL0;
    volatile uint32_t SEL1;
    volatile uint32_t SEL2;
    volatile uint32_t SEL3;
    uint32_t RESERVED3[2];
    volatile uint32_t ADR;
    volatile uint32_t UT0;
    volatile uint32_t UM[9];
    volatile uint32_t UM9;
    volatile uint32_t OPP0;
    volatile uint32_t OPP1;
    volatile uint32_t OPP2;
    volatile uint32_t OPP3;
    volatile uint32_t TMD;
};

#define MCR_DONE (1 << 10)
#define MCR_PEG  (1 << 9)
#define MCR_PGM  (1 << 4)
#define MCR_ERS  (1 << 2)
#define MCR_EHV  (1 << 0)

/* MAPPINGS */

/* internal mapping for mpc57xxx series */
struct nxp_c55fmc_bit_mapping {
    unsigned long start_addr;
    unsigned long end_addr;
    unsigned long locksel;
    unsigned long bit;
};

static const struct nxp_c55fmc_bit_mapping
    map[FLASH_NXP_C55FMC_SECTOR_COUNT] = FLASH_NXP_C55FMC_MAP;

int flash_nxp_c55fmc_init(struct flash *ctx, int base)
{
    ctx->base = (struct FLASH_NXP_C55FMC*)base;
    return 0;
}

int flash_get_nblocks(/*@unused@*/ struct flash *ctx __attribute__ ((unused)))
{
    return (int)(sizeof(map) / sizeof(*map));
}

int flash_get_erase_size(/*@unused@*/ struct flash *ctx __attribute__ ((unused)),
                         size_t block)
{
    picoRTOS_assert(block < (size_t)FLASH_NXP_C55FMC_SECTOR_COUNT, return -EINVAL);

    return (int)(map[block].end_addr - map[block].start_addr) + 1;
}

int flash_get_write_size(/*@unused@*/ struct flash *ctx __attribute__ ((unused)),
                         size_t block)
{
    picoRTOS_assert(block < (size_t)FLASH_NXP_C55FMC_SECTOR_COUNT, return -EINVAL);

    return (int)sizeof(uint64_t);
}

int flash_get_block_addr(/*@unused@*/ struct flash *ctx __attribute__ ((unused)),
                         size_t block)
{
    picoRTOS_assert(block < (size_t)FLASH_NXP_C55FMC_SECTOR_COUNT, return -EINVAL);

    return (int)map[block].start_addr;
}

static int flash_nxp_c55fmc_SELn(struct flash *ctx, size_t block, uint32_t **raddr)
{
    picoRTOS_assert(block < (size_t)FLASH_NXP_C55FMC_SECTOR_COUNT, return -EINVAL);

    *raddr = ((uint32_t*)&ctx->base->SEL0) + map[block].locksel;
    return 0;
}

static int flash_nxp_c55fmc_LOCKn(struct flash *ctx, size_t block, uint32_t **raddr)
{
    picoRTOS_assert(block < (size_t)FLASH_NXP_C55FMC_SECTOR_COUNT, return -EINVAL);

    *raddr = ((uint32_t*)&ctx->base->LOCK0) + map[block].locksel;
    return 0;
}

static int abort_erase(struct flash *ctx, uint32_t *NXP_C55FMC_SELn)
{
    ctx->base->MCR &= ~MCR_EHV;
    ctx->base->MCR &= ~MCR_ERS;
    *NXP_C55FMC_SELn = 0;

    return 0;
}

static int flash_erase_idle(struct flash *ctx, size_t block)
{
    picoRTOS_assert(block < (size_t)FLASH_NXP_C55FMC_SECTOR_COUNT, return -EINVAL);

    int res;
    uint32_t *NXP_C55FMC_SELn = NULL;
    const struct nxp_c55fmc_bit_mapping *m = &map[block];
    uint32_t mask = (uint32_t)(0x80000000u >> m->bit);

    if ((res = flash_nxp_c55fmc_SELn(ctx, block, &NXP_C55FMC_SELn)) < 0 ||
        NXP_C55FMC_SELn == NULL)
        return res;

    /* set mcr_ers to 1 */
    ctx->base->MCR |= MCR_ERS;
    /* select the block */
    *NXP_C55FMC_SELn = mask;
    /* write to any address in flash */
    *((volatile uint32_t*)m->start_addr) = (uint32_t)-1;
    /* start erase sequence */
    ctx->base->MCR |= MCR_EHV;

    ctx->state = FLASH_NXP_C55FMC_STATE_BUSY;
    return -EAGAIN;
}

static int flash_erase_busy(struct flash *ctx, size_t block)
{
    picoRTOS_assert(block < (size_t)FLASH_NXP_C55FMC_SECTOR_COUNT, return -EINVAL);

    int res;
    uint32_t *NXP_C55FMC_SELn = NULL;

    if ((ctx->base->MCR & MCR_DONE) == 0)
        return -EAGAIN;

    if ((res = flash_nxp_c55fmc_SELn(ctx, block, &NXP_C55FMC_SELn)) < 0 ||
        NXP_C55FMC_SELn == NULL)
        return res;

    /* check for errors */
    if ((ctx->base->MCR & MCR_PEG) == 0) {
        (void)abort_erase(ctx, NXP_C55FMC_SELn);
        ctx->state = FLASH_NXP_C55FMC_STATE_IDLE;
        return -EIO;
    }

    /* set ehv to 0 */
    ctx->base->MCR &= ~MCR_EHV;

    /* finish erasing */
    ctx->base->MCR &= ~MCR_ERS;
    *NXP_C55FMC_SELn = 0; /* un-select block */

    ctx->state = FLASH_NXP_C55FMC_STATE_IDLE;
    return 0;
}

int flash_erase(struct flash *ctx, size_t block)
{
    picoRTOS_assert(block < (size_t)FLASH_NXP_C55FMC_SECTOR_COUNT, return -EINVAL);

    switch (ctx->state) {
    case FLASH_NXP_C55FMC_STATE_IDLE: return flash_erase_idle(ctx, block);
    case FLASH_NXP_C55FMC_STATE_BUSY: return flash_erase_busy(ctx, block);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int flash_blankcheck(/*@unused@*/ struct flash *ctx __attribute__ ((unused)),
                     size_t block)
{
    picoRTOS_assert(block < (size_t)FLASH_NXP_C55FMC_SECTOR_COUNT, return -EINVAL);

    const struct nxp_c55fmc_bit_mapping *m = &map[block];

    uint32_t *ptr32 = (uint32_t*)m->start_addr;
    size_t count = (size_t)((m->end_addr - m->start_addr) + 1) / sizeof(uint32_t);

    while (count-- != 0) {
        if (*ptr32++ != (uint32_t)-1) {
            picoRTOS_break();
            /*@notreached@*/ return -EFAULT;
        }
    }

    return 0;
}

static int abort_write(struct flash *ctx)
{
    ctx->base->MCR &= ~(MCR_PGM | MCR_EHV);
    return 0;
}

static int flash_write_idle(struct flash *ctx, size_t addr, const void *data, size_t n)
{
#define MIN(a, b) ((a) < (b) ? (a) : (b))

    picoRTOS_assert((addr % sizeof(uint64_t)) == 0, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert((n % sizeof(uint64_t)) == 0, return -EINVAL);

    int nwritten = 0;
    uint32_t *dst32 = (uint32_t*)addr;
    uint32_t *src32 = (uint32_t*)data;
    size_t count = MIN(n, (size_t)PGM_SIZE) / sizeof(uint32_t);

    /* set to program mode */
    ctx->base->MCR |= MCR_PGM;

    while (count-- != 0) {
        *dst32++ = *src32++;
        nwritten += sizeof(uint32_t);
        /* page full */
        if (((size_t)dst32 % PGM_SIZE) == 0)
            break;
    }

    /* start transfering the cache to actual flash */
    ctx->base->MCR |= MCR_EHV;

    ctx->state = FLASH_NXP_C55FMC_STATE_BUSY;
    ctx->last = nwritten;

    return -EAGAIN;
}

static int flash_write_busy(struct flash *ctx)
{
    if ((ctx->base->MCR & MCR_DONE) == 0)
        return -EAGAIN;

    /* check for errors */
    if ((ctx->base->MCR & MCR_PEG) == 0) {
        (void)abort_write(ctx);
        ctx->state = FLASH_NXP_C55FMC_STATE_IDLE;
        return -EIO;
    }

    /* set ehv to 0 */
    ctx->base->MCR &= ~MCR_EHV;

    /* finish programming sequence */
    ctx->base->MCR &= ~MCR_PGM;

    ctx->state = FLASH_NXP_C55FMC_STATE_IDLE;
    return ctx->last;
}

int flash_write(struct flash *ctx, size_t addr, const void *data, size_t n)
{
    picoRTOS_assert((addr % sizeof(uint64_t)) == 0, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert((n % sizeof(uint64_t)) == 0, return -EINVAL);

    switch (ctx->state) {
    case FLASH_NXP_C55FMC_STATE_IDLE: return flash_write_idle(ctx, addr, data, n);
    case FLASH_NXP_C55FMC_STATE_BUSY: return flash_write_busy(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int flash_lock(struct flash *ctx, size_t block)
{
    picoRTOS_assert(block < (size_t)FLASH_NXP_C55FMC_SECTOR_COUNT, return -EINVAL);

    int res;
    uint32_t *C55FMC_LOCKn = NULL;
    uint32_t mask = (uint32_t)(0x80000000u >> map[block].bit);

    if ((res = flash_nxp_c55fmc_LOCKn(ctx, block, &C55FMC_LOCKn)) < 0 ||
        C55FMC_LOCKn == NULL)
        return res;

    *C55FMC_LOCKn |= mask;
    return 0;
}

int flash_unlock(struct flash *ctx, size_t block)
{
    picoRTOS_assert(block < (size_t)FLASH_NXP_C55FMC_SECTOR_COUNT, return -EINVAL);

    int res;
    uint32_t *C55FMC_LOCKn = NULL;
    uint32_t mask = (uint32_t)(0x80000000u >> map[block].bit);

    if ((res = flash_nxp_c55fmc_LOCKn(ctx, block, &C55FMC_LOCKn)) < 0 ||
        C55FMC_LOCKn == NULL)
        return res;

    *C55FMC_LOCKn &= ~mask;
    return 0;
}

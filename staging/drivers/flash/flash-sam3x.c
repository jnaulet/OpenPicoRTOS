#include "flash-sam3x.h"
#include "picoRTOS.h"

#include <stdint.h>

#define QWORD_SIZE   16
#define FCR_KEY 0x5a

#define FCMD_GETD  0x0  /* get flash descriptor */
#define FCMD_WP    0x1  /* write page */
#define FCMD_WPL   0x2  /* write page & lock */
#define FCMD_EWP   0x3  /* erase & write page */
#define FCMD_EWPL  0x4  /* erase & write page & lock */
#define FCMD_EA    0x5  /* erase all */
#define FCMD_SLB   0x8  /* set lock bit */
#define FCMD_CLB   0x9  /* clear lock bit */
#define FCMD_GLB   0xa  /* get lock bit */
#define FCMD_SGPB  0xb  /* set gpvnm bit */
#define FCMD_CGPB  0xc  /* clear gpnvm bit */
#define FCMD_GGPB  0xd  /* get gpnvm bit */
#define FCMD_STUI  0xe  /* start read uid */
#define FCMD_SPUI  0xf  /* stop read uid */
#define FCMD_GCALB 0x10 /* get calib bit */

struct FLASH_SAM3X {
    volatile uint32_t EEFC_FMR;
    volatile uint32_t EEFC_FCR;
    volatile uint32_t EEFC_FSR;
    volatile uint32_t EEFC_FRR;
};

#define C99_EEFC_FMR_FAM    (1 << 24)
#define C99_EEFC_FMR_FWS_M  0xfu
#define C99_EEFC_FMR_FWS(x) (((x) & C99_EEFC_FMR_FWS_M) << 8)
#define C99_EEFC_FMR_FRDY   (1 << 0)

#define C99_EEFC_FCR_FKEY_M  0xffu
#define C99_EEFC_FCR_FKEY(x) (((x) & C99_EEFC_FCR_FKEY_M) << 24)
#define C99_EEFC_FCR_FARG_M  0xffffu
#define C99_EEFC_FCR_FARG(x) (((x) & C99_EEFC_FCR_FARG_M) << 8)
#define C99_EEFC_FCR_FCMD_M  0xffu
#define C99_EEFC_FCR_FCMD(x) ((x) & C99_EEFC_FCR_FCMD_M)

#define C99_EEFC_FSR_FLOCKE (1 << 2)
#define C99_EEFC_FSR_FCMDE  (1 << 1)
#define C99_EEFC_FSR_FRDY   (1 << 0)

static int run_cmd_busy(struct flash *ctx, size_t n)
{
    uint32_t fsr = ctx->base->EEFC_FSR;

    if ((fsr & C99_EEFC_FSR_FRDY) == 0)
        return -EAGAIN;

    /* back to idle */
    ctx->state = FLASH_SAM3X_STATE_IDLE;

    if ((fsr & C99_EEFC_FSR_FLOCKE) != 0 ||
        (fsr & C99_EEFC_FSR_FCMDE) != 0)
        return -EFAULT;

    return (int)n;
}

static int run_cmd(struct flash *ctx, int cmd, int arg)
{
    if (!picoRTOS_assert(cmd <= FCMD_GCALB)) return -EINVAL;

    switch (ctx->state) {
    case FLASH_SAM3X_STATE_IDLE:
        ctx->base->EEFC_FCR = (uint32_t)(C99_EEFC_FCR_FKEY(FCR_KEY) |
                                         C99_EEFC_FCR_FARG(arg) |
                                         C99_EEFC_FCR_FCMD(cmd));
        return -EAGAIN;

    case FLASH_SAM3X_STATE_BUSY:
        return run_cmd_busy(ctx, 0);

    default:
        break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int init_efd_busywait(/*@partial@*/ struct flash *ctx)
{
    int res;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    uint32_t fl_size;
    uint32_t fl_nb_plane;
    uint32_t fl_nb_lock;

    while ((res = run_cmd(ctx, FCMD_GETD, 0)) == -EAGAIN && deadlock-- != 0) {
    }

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    /* unfold result */
    /*@i@*/ (void)ctx->base->EEFC_FRR;              /* FL_ID (ignore) */
    fl_size = ctx->base->EEFC_FRR;                  /* FL_SIZE */
    ctx->page_size = (size_t)ctx->base->EEFC_FRR;   /* FL_PAGE_SIZE */
    ctx->page_count = (size_t)fl_size / ctx->page_size;

    /* dump the rest for now */
    fl_nb_plane = ctx->base->EEFC_FRR; /* FL_NB_PLANE */
    while (fl_nb_plane-- != 0)
        /*@i@*/ (void)ctx->base->EEFC_FRR;

    fl_nb_lock = ctx->base->EEFC_FRR; /* FL_NB_LOCK */
    while (fl_nb_lock-- != 0)
        /*@i@*/ (void)ctx->base->EEFC_FRR;

    return res;
}

int flash_sam3x_init(/*@out@*/ struct flash *ctx, int base)
{
    int res;

    ctx->base = (struct FLASH_SAM3X*)base;
    ctx->state = FLASH_SAM3X_STATE_IDLE;

    /* info */
    if ((res = init_efd_busywait(ctx)) < 0)
        return res;

    return 0;
}

int flash_get_nblocks(struct flash *ctx)
{
    return (int)ctx->page_count;
}

int flash_get_erase_size(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->page_count)) return -EINVAL;
    return (int)ctx->page_size;
}

int flash_get_write_size(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->page_count)) return -EINVAL;
    return QWORD_SIZE;
}

int flash_get_block_addr(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->page_count)) return -EINVAL;
    return (int)(block * ctx->page_size);
}

int flash_erase(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->page_count)) return -EINVAL;

    return run_cmd(ctx, FCMD_EWP, (int)block);
}

int flash_blankcheck(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->page_count)) return -EINVAL;

    size_t n = ctx->page_size / sizeof(uint32_t);
    uint32_t *addr = (uint32_t*)flash_get_block_addr(ctx, block);

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

    ctx->base->EEFC_FCR = (uint32_t)(C99_EEFC_FCR_FKEY(FCR_KEY) | C99_EEFC_FCR_FCMD(cmd));
    ctx->state = FLASH_SAM3X_STATE_BUSY;
    return -EAGAIN;
}

static int write_data(struct flash *ctx, int cmd, size_t addr,
                      const void *data, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case FLASH_SAM3X_STATE_IDLE:
        return flash_write_idle(ctx, cmd, addr, data, n);

    case FLASH_SAM3X_STATE_BUSY:
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
            res = write_data(ctx, FCMD_WP, addr + (size_t)nwritten,
                             &data8[nwritten], ctx->page_size);
        /* 128bit boundary */
        else if ((addr % (size_t)QWORD_SIZE) == 0 && n >= (size_t)QWORD_SIZE)
            res = write_data(ctx, FCMD_WP, addr + (size_t)nwritten,
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
    if (!picoRTOS_assert(block < ctx->page_count)) return -EINVAL;

    return run_cmd(ctx, FCMD_SLB, (int)block);
}

int flash_unlock(struct flash *ctx, size_t block)
{
    if (!picoRTOS_assert(block < ctx->page_count)) return -EINVAL;

    return run_cmd(ctx, FCMD_CLB, (int)block);
}

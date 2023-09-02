#include "twi-sam3x.h"
#include "picoRTOS.h"

#include <stdint.h>

struct TWI_SAM3X {
    volatile uint32_t TWI_CR;
    volatile uint32_t TWI_MMR;
    volatile uint32_t TWI_SMR;
    volatile uint32_t TWI_IADR;
    volatile uint32_t TWI_CWGR;
    uint32_t RESERVED0[3];
    volatile uint32_t TWI_SR;
    volatile uint32_t TWI_IER;
    volatile uint32_t TWI_IDR;
    volatile uint32_t TWI_IMR;
    volatile uint32_t TWI_RHR;
    volatile uint32_t TWI_THR;
};

#define TWI_CR_SWRST (1 << 7)
#define TWI_CR_QUICK (1 << 6)
#define TWI_CR_SVDIS (1 << 5)
#define TWI_CR_SVEN  (1 << 4)
#define TWI_CR_MSDIS (1 << 3)
#define TWI_CR_MSEN  (1 << 2)
#define TWI_CR_STOP  (1 << 1)
#define TWI_CR_START (1 << 0)

#define TWI_MMR_DADR_M    0x7fu
#define TWI_MMR_DADR(x)   (((x) & TWI_MMR_DADR_M) << 16)
#define TWI_MMR_MREAD     (1 << 12)
#define TWI_MMR_IADRSZ_M  0x3u
#define TWI_MMR_IADRSZ(x) (((x) & TWI_MMR_IADRSZ_M) << 8)

#define TWI_SMR_SADR_M  0x7fu
#define TWI_SMR_SADR(x) (((x) & TWI_SMR_SADR_M) << 16)

#define TWI_IADR_IADR_M  0xffffffu
#define TWI_IADR_IADR(x) ((x) & TWI_IADR_IADR_M)

#define TWI_CWGR_CKDIV_M  0x7u
#define TWI_CWGR_CKDIV(x) (((x) & TWI_CWGR_CKDIV_M) << 16)
#define TWI_CWGR_CHDIV_M  0xffu
#define TWI_CWGR_CHDIV(x) (((x) & TWI_CWGR_CHDIV_M) << 8)
#define TWI_CWGR_CLDIV_M  0xffu
#define TWI_CWGR_CLDIV(x) ((x) & TWI_CWGR_CLDIV_M)

#define TWI_SR_TXBUFE (1 << 15)
#define TWI_SR_RXBUFF (1 << 14)
#define TWI_SR_ENDTX  (1 << 13)
#define TWI_SR_ENDRX  (1 << 12)
#define TWI_SR_EOSACC (1 << 11)
#define TWI_SR_SCLWS  (1 << 10)
#define TWI_SR_ARBLST (1 << 9)
#define TWI_SR_NACK   (1 << 8)
#define TWI_SR_OVRE   (1 << 6)
#define TWI_SR_GACC   (1 << 5)
#define TWI_SR_SVACC  (1 << 4)
#define TWI_SR_SVREAD (1 << 3)
#define TWI_SR_TXRDY  (1 << 2)
#define TWI_SR_RXRDY  (1 << 1)
#define TWI_SR_TXCOMP (1 << 0)

/* Function: twi_sam3x_init
 * Initializes a TWI / I2C
 *
 * Parameters:
 *  ctx - The TWI to init
 *  base - The TWI base address
 *  clkid - The TWI clock ID
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int twi_sam3x_init(struct twi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct TWI_SAM3X*)base;
    ctx->clkid = clkid;
    ctx->state = TWI_SAM3X_STATE_IDLE;
    ctx->mode = TWI_MODE_COUNT;
    ctx->twi_cr = 0;

    /* software reset */
    ctx->base->TWI_CR = (uint32_t)TWI_CR_SWRST;

    /* spin wait */
    clock_freq_t freq = clock_get_freq(CLOCK_SYSCLK);
    int wait = (int)freq / 100; /* least 10ms */

    while (wait-- != 0) {
    }

    return 0;
}

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return -EIO);

    /* Tlow = ((CLDIV * 2^CKDIV) + 4) * Tmck
     * Thigh = ((CHDIV * 2^CKDIV) + 4) * Tmck
     */
    unsigned long ckdiv;
    unsigned long cndiv;
    unsigned long divider = (unsigned long)freq / bitrate;

    /* solve this equation in an iterative way */
    for (ckdiv = 0; ckdiv <= (unsigned long)TWI_CWGR_CKDIV_M; ckdiv++)
        for (cndiv = 0; cndiv <= (unsigned long)TWI_CWGR_CLDIV_M; cndiv++)
            if ((cndiv * (1ul << ckdiv) + 4ul) == divider) {
                ctx->base->TWI_CWGR = (uint32_t)(TWI_CWGR_CKDIV(ckdiv) |
                                                 TWI_CWGR_CHDIV(cndiv / 2) |
                                                 TWI_CWGR_CLDIV(cndiv / 2));
                return 0;
            }

    picoRTOS_break();
    /*@notreached@*/
    return -EINVAL;
}

static int set_mode(struct twi *ctx, twi_mode_t mode)
{
    picoRTOS_assert(mode != TWI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < TWI_MODE_COUNT, return -EINVAL);

    if (mode == TWI_MODE_MASTER)
        ctx->mode = TWI_MODE_MASTER;
    else{
        ctx->base->TWI_CR = (uint32_t)(TWI_CR_SVEN | TWI_CR_MSDIS);
        ctx->mode = TWI_MODE_SLAVE;
    }

    return 0;
}

int twi_setup(struct twi *ctx, struct twi_settings *settings)
{
    picoRTOS_assert(settings->mode < TWI_MODE_COUNT, return -EINVAL);
    picoRTOS_assert(settings->slave_addr < (twi_addr_t)TWI_ADDR_COUNT, return -EINVAL);

    int res;

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* mode */
    if (settings->mode != TWI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    if (ctx->mode == TWI_MODE_MASTER)
        ctx->base->TWI_MMR = (uint32_t)TWI_MMR_DADR(settings->slave_addr);

    if (ctx->mode == TWI_MODE_SLAVE)
        ctx->base->TWI_SMR = (uint32_t)TWI_SMR_SADR(settings->slave_addr);

    return 0;
}

int twi_poll(struct twi *ctx)
{
    /* clear on read */
    uint32_t twi_sr = ctx->base->TWI_SR;

    if ((twi_sr & TWI_SR_SVACC) != 0 ||
        (twi_sr & TWI_SR_EOSACC) != 0) {
        if ((twi_sr & TWI_SR_SVREAD) == 0) return TWI_WRITE;
        else return TWI_READ;
    }

    return -EAGAIN;
}

static int twi_write_as_master_idle(struct twi *ctx)
{
    if ((ctx->base->TWI_SR & TWI_SR_TXCOMP) == 0)
        return -EAGAIN;

    ctx->base->TWI_MMR &= ~TWI_MMR_MREAD;
    ctx->twi_cr = (uint32_t)(TWI_CR_SVDIS | TWI_CR_MSEN);
    ctx->base->TWI_CR = ctx->twi_cr;

    ctx->state = TWI_SAM3X_STATE_XFER;
    return -EAGAIN;
}

static int twi_write_as_master_xfer(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int sent = 0;
    const uint8_t *buf8 = (const uint8_t*)buf;

    while (n-- != 0) {

        uint32_t stop = 0;

        /* NACK */
        if ((ctx->base->TWI_SR & TWI_SR_NACK) != 0) {
            ctx->state = TWI_SAM3X_STATE_IDLE;
            break;
        }

        if ((ctx->base->TWI_SR & TWI_SR_TXRDY) == 0)
            break;

        /* last byte */
        if (n == 0) {
            stop = (uint32_t)TWI_CR_STOP;
            ctx->state = TWI_SAM3X_STATE_IDLE;
        }

        ctx->base->TWI_CR = (ctx->twi_cr | stop);
        ctx->base->TWI_THR = (uint32_t)buf8[sent++];
    }

    if (sent == 0)
        return -EAGAIN;

    return sent;
}

static int twi_write_as_master(struct twi *ctx, const void *buf, size_t n)
{
    switch (ctx->state) {
    case TWI_SAM3X_STATE_IDLE: return twi_write_as_master_idle(ctx);
    case TWI_SAM3X_STATE_XFER: return twi_write_as_master_xfer(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_write_as_slave(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int sent = 0;
    const uint8_t *buf8 = (const uint8_t*)buf;

    while ((size_t)sent != n) {
        if ((ctx->base->TWI_SR & TWI_SR_TXRDY) == 0 ||
            (ctx->base->TWI_SR & TWI_SR_TXCOMP) != 0)
            break;

        ctx->base->TWI_THR = (uint32_t)buf8[sent++];
    }

    if (sent == 0)
        return -EAGAIN;

    return sent;
}

int twi_write(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if (ctx->mode == TWI_MODE_MASTER)
        return twi_write_as_master(ctx, buf, n);

    if (ctx->mode == TWI_MODE_SLAVE)
        return twi_write_as_slave(ctx, buf, n);

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_read_as_master_idle(struct twi *ctx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->TWI_SR & TWI_SR_TXCOMP) == 0)
        return -EAGAIN;

    ctx->base->TWI_MMR |= TWI_MMR_MREAD;
    ctx->twi_cr = (uint32_t)(TWI_CR_SVDIS | TWI_CR_MSEN);

    if (n == (size_t)1) ctx->base->TWI_CR = ctx->twi_cr | TWI_CR_START | TWI_CR_STOP;
    else ctx->base->TWI_CR = ctx->twi_cr | TWI_CR_START;

    ctx->state = TWI_SAM3X_STATE_XFER;
    return -EAGAIN;
}

static int twi_read_as_master_xfer(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int recv = 0;
    uint8_t *buf8 = (uint8_t*)buf;

    while (n-- != 0) {

        uint32_t stop = 0;

        /* NACK */
        if ((ctx->base->TWI_SR & TWI_SR_NACK) != 0) {
            ctx->state = TWI_SAM3X_STATE_IDLE;
            break;
        }

        if ((ctx->base->TWI_SR & TWI_SR_RXRDY) == 0)
            break;

        if (n == 0) {
            stop = (uint32_t)TWI_CR_STOP;
            ctx->state = TWI_SAM3X_STATE_IDLE;
        }

        ctx->base->TWI_CR = (ctx->twi_cr | stop);
        buf8[recv++] = (uint8_t)ctx->base->TWI_RHR;
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

static int twi_read_as_master(struct twi *ctx, void *buf, size_t n)
{
    switch (ctx->state) {
    case TWI_SAM3X_STATE_IDLE: return twi_read_as_master_idle(ctx, n);
    case TWI_SAM3X_STATE_XFER: return twi_read_as_master_xfer(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_read_as_slave(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int recv = 0;
    uint8_t *buf8 = (uint8_t*)buf;

    while ((size_t)recv != n) {
        if ((ctx->base->TWI_SR & TWI_SR_RXRDY) == 0 ||
            (ctx->base->TWI_SR & TWI_SR_TXCOMP) != 0)
            break;

        buf8[recv++] = (uint8_t)ctx->base->TWI_RHR;
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

int twi_read(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if (ctx->mode == TWI_MODE_MASTER)
        return twi_read_as_master(ctx, buf, n);

    if (ctx->mode == TWI_MODE_SLAVE)
        return twi_read_as_slave(ctx, buf, n);

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

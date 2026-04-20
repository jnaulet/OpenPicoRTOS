#include "flash-ext-s25fl1.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <string.h>

static int run_cmd(struct flash_ext *ctx, uint8_t *buf, size_t n)
{
    int res;
    size_t index;

    if (ctx->left == 0)
        ctx->left = n;

    index = n - ctx->left;
    if ((res = spi_xfer(ctx->spi, &buf[index], &buf[index], ctx->left)) < 0)
        return res;

    ctx->left -= (size_t)res;
    return (ctx->left == 0) ? 0 : -EAGAIN;
}

static int run_single(struct flash_ext *ctx, uint8_t cmd)
{
#define S25FL1_WREN  0x06
#define S25FL1_WRDIS 0x04
    return run_cmd(ctx, &cmd, sizeof(cmd));
}

static int run_busy(struct flash_ext *ctx)
{
#define S25FL1_RSR1      0x05
#define S25FL1_RSR1_BUSY (1 << 0)
    int res;
    uint8_t rsr[2] = { (uint8_t)S25FL1_RSR1, 0 };

    if ((res = run_cmd(ctx, rsr, sizeof(rsr))) < 0)
        return res;

    if ((rsr[1] & S25FL1_RSR1_BUSY) != 0)
        return -EAGAIN;

    return 0;
}

int flash_ext_s25fl1_init(struct flash_ext *ctx, struct spi *spi)
{
    ctx->spi = spi;
    ctx->left = 0;
    ctx->seq = 0;
    memset(ctx->jedec, 0, sizeof(ctx->jedec));
    memset(&ctx->attr, 0, sizeof(ctx->attr));

    return 0;
}

int flash_ext_s25fl1_reset(struct flash_ext *ctx)
{
#define S25FL1_RSTEN     0x66
#define S25FL1_RST       0x99
    int res = 0;

    for (;;) {
        switch (ctx->seq) {
        case 0: res = run_single(ctx, (uint8_t)S25FL1_RSTEN); break;
        case 1: res = run_single(ctx, (uint8_t)S25FL1_RST); break;
        case 2: arch_delay_us(2ul); /* TRST */ /*@fallthrough@*/
        default: ctx->seq = 0; return 0;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

int flash_ext_s25fl1_quad_enable(struct flash_ext *ctx, bool enable)
{
#define S25FL1_WSR     0x01
#define S25FL1_RSR2    0x35
#define S25FL1_RSR3    0x33
#define S25FL1_RSR2_QE (1 << 1)

    /* this is a damn long sequence */
    static uint8_t rsr[2] = { 0, 0 };
    static uint8_t wsr[4] = { (uint8_t)S25FL1_WSR, 0, 0, 0 };

    int res = 0;

    for (;;) {
        switch (ctx->seq) {
        case 0:
            rsr[0] = (uint8_t)S25FL1_RSR1;
            res = run_cmd(ctx, rsr, sizeof(rsr));
            break;

        case 1:
            wsr[1] = rsr[1]; /* store prev result */
            rsr[0] = (uint8_t)S25FL1_RSR2;
            res = run_cmd(ctx, rsr, sizeof(rsr));
            break;

        case 2:
            if (enable) wsr[2] = rsr[1] | S25FL1_RSR2_QE;
            else wsr[2] = rsr[1] & ~S25FL1_RSR2_QE;
            if (wsr[2] == rsr[1]) {
                /* aready in QE */
                ctx->seq = 0;
                return 0;
            }
            /* next step */
            rsr[0] = (uint8_t)S25FL1_RSR3;
            res = run_cmd(ctx, rsr, sizeof(rsr));
            break;

        case 3:
            wsr[3] = rsr[1]; /* store prev result */
            res = run_single(ctx, (uint8_t)S25FL1_WREN);
            break;

        case 4:
            wsr[0] = (uint8_t)S25FL1_WSR;
            res = run_cmd(ctx, wsr, sizeof(wsr));
            break;

        case 5:
            res = run_busy(ctx);
            break;

        default:
            /* try again */
            ctx->seq = 0;
            continue;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

int flash_ext_probe(struct flash_ext *ctx)
{
#define S25FL1_RDID  0x9f
    int res;
    static uint8_t jedec[4] = { 0, 0, 0, 0 };

    jedec[0] = (uint8_t)S25FL1_RDID;
    if ((res = run_cmd(ctx, jedec, sizeof(jedec))) < 0)
        return res;

    memcpy(ctx->jedec, &jedec[1], sizeof(ctx->jedec));
    /* set attr according to JEDEC */
    picoRTOS_assert(ctx->jedec[0] == (uint8_t)0x1, return -EIO);
    picoRTOS_assert(ctx->jedec[1] == (uint8_t)0x40, return -EIO);

    ctx->attr.erase_unit_len = (size_t)4096;
    ctx->attr.write_unit_len = sizeof(uint8_t);
    ctx->attr.lock_unit_len = (size_t)4096;

    switch (ctx->jedec[2]) {
    case 0x15: ctx->attr.erase_unit_count = (size_t)512; break;
    case 0x16: ctx->attr.erase_unit_count = (size_t)1024; break;
    case 0x17: ctx->attr.erase_unit_count = (size_t)2048; break;
    default:
        picoRTOS_assert(false, return -EIO);
    }

    /* compute total size */
    ctx->attr.total_size = ctx->attr.erase_unit_len *
                           ctx->attr.erase_unit_count;
    return 0;
}

int flash_ext_get_attributes(struct flash_ext *ctx, struct flash_attributes *attr)
{
    memcpy(attr, &ctx->attr, sizeof(*attr));
    return 0;
}

int flash_ext_erase(struct flash_ext *ctx, size_t offset)
{
#define S25FL1_SE 0x20
    picoRTOS_assert((offset % ctx->attr.erase_unit_len) == 0, return -EINVAL);
    picoRTOS_assert(offset < ctx->attr.total_size, return -EINVAL);

    int res = 0;

    for (;;) {
        /* *INDENT-OFF* */
        uint8_t se[4] = { (uint8_t)S25FL1_SE, (uint8_t)(offset >> 16),
                          (uint8_t)(offset >> 8), (uint8_t)offset };
        /* *INDENT-ON* */

        switch (ctx->seq) {
        case 0: res = run_single(ctx, (uint8_t)S25FL1_WREN); break;
        case 1: res = run_cmd(ctx, se, sizeof(se)); break;
        case 2: res = run_busy(ctx); break;
        default: ctx->seq = 0; return 0;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/* FIXME: perfectly arbitrary */
static int make_rd_req(uint8_t *rd, size_t offset, size_t n)
{
#define S25FL1_RD     0x3
#define S25FL1_RD_LEN 256
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)S25FL1_RD_LEN, return -EINVAL);

    rd[0] = (uint8_t)S25FL1_RD;
    rd[1] = (uint8_t)(offset >> 16);
    rd[2] = (uint8_t)(offset >> 8);
    rd[3] = (uint8_t)offset;

    return (int)n;
}

int flash_ext_read(struct flash_ext *ctx, size_t offset, void *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert((offset + n) < ctx->attr.total_size, return -EINVAL);

    int res = 0;

    n = MIN(n, (size_t)S25FL1_RD_LEN);

    for (;;) {
        static uint8_t rd[S25FL1_RD_LEN + 4];

        switch (ctx->seq) {
        case 0: res = make_rd_req(rd, offset, n); break;
        case 2: res = run_cmd(ctx, rd, sizeof(rd)); break;
        case 3: memcpy(data, &rd[4], n); /*@fallthrough@*/
        default: ctx->seq = 0; return (int)n;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

static int make_pp_req(uint8_t *pp, size_t offset, const void *data, size_t n)
{
#define S25FL1_PP 0x02
#define S25FL1_PP_LEN 256
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)S25FL1_PP_LEN, return -EINVAL);

    pp[0] = (uint8_t)S25FL1_PP;
    pp[1] = (uint8_t)(offset >> 16);
    pp[2] = (uint8_t)(offset >> 8);
    pp[3] = (uint8_t)offset;
    memcpy(&pp[4], data, n);

    return (int)n;
}

int flash_ext_write(struct flash_ext *ctx, size_t offset, const void *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert((offset % (size_t)S25FL1_PP_LEN) == 0, return -EINVAL);
    /* TODO: check offset & offset + n */
#define S25FL1_PP_LEN 256
#define MIN(a, b)     (((a) < (b)) ? (a) : (b))

    int res = 0;

    n = MIN(n, (size_t)S25FL1_PP_LEN);

    for (;;) {
        static uint8_t pp[S25FL1_PP_LEN + 4];

        switch (ctx->seq) {
        case 0: res = run_single(ctx, (uint8_t)S25FL1_WREN); break;
        case 1: res = make_pp_req(pp, offset, data, n); break;
        case 2: res = run_cmd(ctx, pp, sizeof(pp)); break;
        case 3: res = run_busy(ctx); break;
        default: ctx->seq = 0; return (int)n;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

int flash_ext_lock(struct flash_ext *ctx, size_t offset)
{
#define S25FL1_SBP     0x39
#define S25FL1_SBP_A11 0x2
    int res = 0;

    /*@i@*/ (void)offset; /* FIXME */

    for (;;) {
        /* *INDENT-OFF* */
        uint8_t sbp[4] = { (uint8_t)S25FL1_SBP, 0,
	                   (uint8_t)S25FL1_SBP_A11, 0 };
        /* *INDENT-ON* */
        switch (ctx->seq) {
        case 0: res = run_single(ctx, (uint8_t)S25FL1_WREN); break;
        case 1: res = run_cmd(ctx, sbp, sizeof(sbp)); break;
        case 2: res = run_busy(ctx); break;
        case 3: res = run_single(ctx, (uint8_t)S25FL1_WRDIS); break; /* FIXME: read doc */
        default: ctx->seq = 0; return 0;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

int flash_ext_unlock(struct flash_ext *ctx, size_t offset)
{
    int res = 0;

    for (;;) {
        /* *INDENT-OFF* */
        uint8_t sbp[4] = { (uint8_t)S25FL1_SBP, (uint8_t)(offset >> 16),
	                   (uint8_t)((offset >> 8) & ~0x3u), 0 };
        /* *INDENT-ON* */
        switch (ctx->seq) {
        case 0: res = run_single(ctx, (uint8_t)S25FL1_WREN); break;
        case 1: res = run_cmd(ctx, sbp, sizeof(sbp)); break;
        case 2: res = run_busy(ctx); break;
        case 3: res = run_single(ctx, (uint8_t)S25FL1_WRDIS); break;
        default: ctx->seq = 0; return 0;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

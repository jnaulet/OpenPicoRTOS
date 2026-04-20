#include "flash-ext-sst26vf.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <string.h>

#define SST26VF_RSTEN    0x66
#define SST26VF_RST      0x99
#define SST26VF_JEDEC_ID 0x9f
#define SST26VF_EQIO     0x38
#define SST26VF_RSTQIO   0xff
#define SST26VF_SE       0x20
#define SST26VF_RDSR     0x05
#define SST26VF_WREN     0x06
#define SST26VF_PP       0x02
#define SST26VF_RD       0x03

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
    return run_cmd(ctx, &cmd, sizeof(cmd));
}

static int run_busy(struct flash_ext *ctx)
{
#define SST26VF_SR_BUSY (1 << 0)
    int res;
    uint8_t sr[2] = { (uint8_t)SST26VF_RDSR, 0 };

    if ((res = run_cmd(ctx, sr, sizeof(sr))) < 0)
        return res;

    if ((sr[1] & SST26VF_SR_BUSY) != 0)
        return -EAGAIN;

    return 0;
}

int flash_ext_sst26vf_init(struct flash_ext *ctx, struct spi *spi)
{
    ctx->spi = spi;
    ctx->left = 0;
    ctx->seq = 0;
    memset(ctx->jedec, 0, sizeof(ctx->jedec));
    memset(&ctx->attr, 0, sizeof(ctx->attr));

    return 0;
}

int flash_ext_sst26vf_reset(struct flash_ext *ctx)
{
    int res = 0;

    for (;;) {
        switch (ctx->seq) {
        case 0: res = run_single(ctx, (uint8_t)SST26VF_RSTEN); break;
        case 1: res = run_single(ctx, (uint8_t)SST26VF_RST); break;
        case 2: arch_delay_us(1ul); /* TR(o) */ /*@fallthrough@*/
        default: ctx->seq = 0; return 0;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

int flash_ext_sst26vf_quad_enable(struct flash_ext *ctx, bool enable)
{
    if (enable) return run_single(ctx, (uint8_t)SST26VF_EQIO);
    else return run_single(ctx, (uint8_t)SST26VF_RSTQIO);
}

int flash_ext_probe(struct flash_ext *ctx)
{
    int res;
    uint8_t jedec[4] = { 0, 0, 0, 0 };

    jedec[0] = (uint8_t)SST26VF_JEDEC_ID;
    if ((res = run_cmd(ctx, jedec, sizeof(jedec))) < 0)
        return res;

    memcpy(ctx->jedec, &jedec[1], sizeof(ctx->jedec));
    picoRTOS_assert(ctx->jedec[0] == (uint8_t)0xbf, return -EIO);
    picoRTOS_assert(ctx->jedec[1] == (uint8_t)0x26, return -EIO);

    ctx->attr.erase_unit_len = (size_t)4096;
    ctx->attr.write_unit_len = sizeof(uint8_t);
    ctx->attr.lock_unit_len = ctx->attr.erase_unit_len; /* FIXME */

    switch (ctx->jedec[2] & 0xf) {
    case 0x01: ctx->attr.erase_unit_count = (size_t)512; break;
    case 0x02: ctx->attr.erase_unit_count = (size_t)1024; break;
    default:
        picoRTOS_assert(false, return -EIO);
    }

    /* compute total size */
    ctx->attr.total_size = ctx->attr.erase_unit_len *
                           ctx->attr.erase_unit_count;
    return 0;
}

/* cppcheck-suppress [constParameterPointer] */
int flash_ext_get_attributes(struct flash_ext *ctx, struct flash_attributes *attr)
{
    memcpy(attr, &ctx->attr, sizeof(*attr));
    return 0;
}

int flash_ext_erase(struct flash_ext *ctx, size_t offset)
{
    picoRTOS_assert((offset % ctx->attr.erase_unit_len) == 0, return -EINVAL);
    picoRTOS_assert(offset < ctx->attr.total_size, return -EINVAL);

    int res = 0;

    for (;;) {
        /* *INDENT-OFF* */
        uint8_t se[4] = { (uint8_t)SST26VF_SE, (uint8_t)(offset >> 16),
                          (uint8_t)(offset >> 8), (uint8_t)offset };
        /* *INDENT-ON* */

        switch (ctx->seq) {
        case 0: res = run_single(ctx, (uint8_t)SST26VF_WREN); break;
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
#define SST26VF_RD_LEN 256
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)SST26VF_RD_LEN, return -EINVAL);

    rd[0] = (uint8_t)SST26VF_RD;
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

    n = MIN(n, (size_t)SST26VF_RD_LEN);

    for (;;) {
        static uint8_t rd[SST26VF_RD_LEN + 4];

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
#define SST26VF_PP_LEN 256
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)SST26VF_PP_LEN, return -EINVAL);

    pp[0] = (uint8_t)SST26VF_PP;
    pp[1] = (uint8_t)(offset >> 16);
    pp[2] = (uint8_t)(offset >> 8);
    pp[3] = (uint8_t)offset;
    memcpy(&pp[4], data, n);

    return (int)n;
}

int flash_ext_write(struct flash_ext *ctx, size_t offset, const void *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert((offset % (size_t)SST26VF_PP_LEN) == 0, return -EINVAL);
    picoRTOS_assert((offset + n) < ctx->attr.total_size, return -EINVAL);

    int res = 0;

    n = MIN(n, (size_t)SST26VF_PP_LEN);

    for (;;) {
        static uint8_t pp[SST26VF_PP_LEN + 4];

        switch (ctx->seq) {
        case 0: res = run_single(ctx, (uint8_t)SST26VF_WREN); break;
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
    /*@i@*/ (void)ctx;
    /*@i@*/ (void)offset;

    return 0;
}

int flash_ext_unlock(struct flash_ext *ctx, size_t offset)
{
    /*@i@*/ (void)ctx;
    /*@i@*/ (void)offset;

    return 0;
}

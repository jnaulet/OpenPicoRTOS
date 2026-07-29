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
#define SST26VF_WRSR     0x01
#define SST26VF_WREN     0x06
#define SST26VF_PP       0x02
#define SST26VF_RD       0x03
#define SST26VF_WBPR     0x42
#define SST26VF_nWDLR    0xe8
#define SST26VF_ULBPR    0x98

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
        case 2: arch_delay_us(1000ul); /* TR(o) */ /*@fallthrough@*/
        default: ctx->seq = 0; return 0;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

int flash_ext_sst26vf_quad_enable(struct flash_ext *ctx, bool enable)
{
#define SST26VF_CR_IOC  (1 << 1)
#define SST26VF_CR_WPEN (1 << 7)

    if (enable) {

        int res = 0;

        for (;;) {
            /* *INDENT-OFF* */
            uint8_t cr[3] = { (uint8_t)SST26VF_WRSR, 0, (uint8_t)SST26VF_CR_IOC };
            /* *INDENT-ON* */

            switch (ctx->seq) {
            case 0: res = run_single(ctx, (uint8_t)SST26VF_WREN); break;
            case 1: res = run_cmd(ctx, cr, sizeof(cr)); break;
            default: ctx->seq = 0; return 0;
            }

            if (res < 0) break;
            ctx->seq++;
        }

        return res;
    }

    return 0;
}

int flash_ext_probe(struct flash_ext *ctx)
{
    int res;

    ctx->jedec[0] = (uint8_t)SST26VF_JEDEC_ID;
    if ((res = run_cmd(ctx, ctx->jedec, sizeof(ctx->jedec))) < 0)
        return res;

    picoRTOS_assert(ctx->jedec[1] == (uint8_t)0xbf, return -EIO);
    picoRTOS_assert(ctx->jedec[2] == (uint8_t)0x26, return -EIO);

    ctx->attr.erase_unit_len = (size_t)4096;
    ctx->attr.write_unit_len = sizeof(uint8_t);
    ctx->attr.lock_unit_len = (size_t)8192;

    switch (ctx->jedec[3] & 0xf) {
    case 0x01: ctx->attr.erase_unit_count = (size_t)512; break;
    case 0x02: ctx->attr.erase_unit_count = (size_t)1024; break;
    case 0x03: ctx->attr.erase_unit_count = (size_t)2048; break;
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
        switch (ctx->seq) {
        case 0: res = make_rd_req(ctx->rd, offset, n); break;
        case 2: res = run_cmd(ctx, ctx->rd, sizeof(ctx->rd)); break;
        case 3: memcpy(data, &ctx->rd[4], n); /*@fallthrough@*/
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
        switch (ctx->seq) {
        case 0: res = run_single(ctx, (uint8_t)SST26VF_WREN); break;
        case 1: res = make_pp_req(ctx->pp, offset, data, n); break;
        case 2: res = run_cmd(ctx, ctx->pp, sizeof(ctx->pp)); break;
        case 3: res = run_busy(ctx); break;
        default: ctx->seq = 0; return (int)n;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

struct bp {
    int bit;
    unsigned addr_lo;
    unsigned addr_hi;
};

#define BP_REG_COUNT 136

static const struct bp bp_reg[BP_REG_COUNT] = {
    /* *INDENT-OFF* */
    { 142, 0x7fe000u, 0x7fffffu },
    { 140, 0x7fc000u, 0x7fdfffu },
    { 138, 0x7fa000u, 0x7fbfffu },
    { 136, 0x7f8000u, 0x7f9fffu },
    { 134, 0x006000u, 0x007fffu },
    { 132, 0x004000u, 0x005fffu },
    { 130, 0x002000u, 0x003fffu },
    { 128, 0x000000u, 0x001fffu },
    { 127, 0x7f0000u, 0x7f7fffu },
    { 126, 0x008000u, 0x00ffffu },
    { 125, 0x7e0000u, 0x7effffu },
    { 124, 0x7d0000u, 0x7dffffu },
    { 123, 0x7c0000u, 0x7cffffu },
    { 122, 0x7b0000u, 0x7bffffu },
    { 121, 0x7a0000u, 0x7affffu },
    { 120, 0x790000u, 0x79ffffu },
    { 119, 0x780000u, 0x78ffffu },
    { 118, 0x770000u, 0x77ffffu },
    { 117, 0x760000u, 0x76ffffu },
    { 116, 0x750000u, 0x75ffffu },
    { 115, 0x740000u, 0x74ffffu },
    { 114, 0x730000u, 0x73ffffu },
    { 113, 0x720000u, 0x72ffffu },
    { 112, 0x710000u, 0x71ffffu },
    { 111, 0x700000u, 0x70ffffu },
    { 110, 0x6f0000u, 0x6fffffu },
    { 109, 0x6e0000u, 0x6effffu },
    { 108, 0x6d0000u, 0x6dffffu },
    { 107, 0x6c0000u, 0x6cffffu },
    { 106, 0x6b0000u, 0x6bffffu },
    { 105, 0x6a0000u, 0x6affffu },
    { 104, 0x690000u, 0x69ffffu },
    { 103, 0x680000u, 0x68ffffu },
    { 102, 0x670000u, 0x67ffffu },
    { 101, 0x660000u, 0x66ffffu },
    { 100, 0x650000u, 0x65ffffu },
    { 99,  0x640000u, 0x64ffffu },
    { 98,  0x630000u, 0x63ffffu },
    { 97,  0x620000u, 0x62ffffu },
    { 96,  0x610000u, 0x61ffffu },
    { 95,  0x600000u, 0x60ffffu },
    { 94,  0x5f0000u, 0x5fffffu },
    { 93,  0x5e0000u, 0x5effffu },
    { 92,  0x5d0000u, 0x5dffffu },
    { 91,  0x5c0000u, 0x5cffffu },
    { 90,  0x5b0000u, 0x5bffffu },
    { 89,  0x5a0000u, 0x5affffu },
    { 88,  0x590000u, 0x59ffffu },
    { 87,  0x580000u, 0x58ffffu },
    { 86,  0x570000u, 0x57ffffu },
    { 85,  0x560000u, 0x56ffffu },
    { 84,  0x550000u, 0x55ffffu },
    { 83,  0x540000u, 0x54ffffu },
    { 82,  0x530000u, 0x53ffffu },
    { 81,  0x520000u, 0x52ffffu },
    { 80,  0x510000u, 0x51ffffu },
    { 79,  0x500000u, 0x50ffffu },
    { 78,  0x4f0000u, 0x4fffffu },
    { 77,  0x4e0000u, 0x4effffu },
    { 76,  0x4d0000u, 0x4dffffu },
    { 75,  0x4c0000u, 0x4cffffu },
    { 74,  0x4b0000u, 0x4bffffu },
    { 73,  0x4a0000u, 0x4affffu },
    { 72,  0x490000u, 0x49ffffu },
    { 71,  0x480000u, 0x48ffffu },
    { 70,  0x470000u, 0x47ffffu },
    { 69,  0x460000u, 0x46ffffu },
    { 68,  0x450000u, 0x45ffffu },
    { 67,  0x440000u, 0x44ffffu },
    { 66,  0x430000u, 0x43ffffu },
    { 65,  0x420000u, 0x42ffffu },
    { 64,  0x410000u, 0x41ffffu },
    { 63,  0x400000u, 0x40ffffu },
    { 62,  0x3f0000u, 0x3fffffu },
    { 61,  0x3e0000u, 0x3effffu },
    { 60,  0x3d0000u, 0x3dffffu },
    { 59,  0x3c0000u, 0x3cffffu },
    { 58,  0x3b0000u, 0x3bffffu },
    { 57,  0x3a0000u, 0x3affffu },
    { 56,  0x390000u, 0x39ffffu },
    { 55,  0x380000u, 0x38ffffu },
    { 54,  0x370000u, 0x37ffffu },
    { 53,  0x360000u, 0x36ffffu },
    { 52,  0x350000u, 0x35ffffu },
    { 51,  0x340000u, 0x34ffffu },
    { 50,  0x330000u, 0x33ffffu },
    { 49,  0x320000u, 0x32ffffu },
    { 48,  0x310000u, 0x31ffffu },
    { 47,  0x300000u, 0x30ffffu },
    { 46,  0x2f0000u, 0x2fffffu },
    { 45,  0x2e0000u, 0x2effffu },
    { 44,  0x2d0000u, 0x2dffffu },
    { 43,  0x2c0000u, 0x2cffffu },
    { 42,  0x2b0000u, 0x2bffffu },
    { 41,  0x2a0000u, 0x2affffu },
    { 40,  0x290000u, 0x29ffffu },
    { 39,  0x280000u, 0x28ffffu },
    { 38,  0x270000u, 0x27ffffu },
    { 37,  0x260000u, 0x26ffffu },
    { 36,  0x250000u, 0x25ffffu },
    { 35,  0x240000u, 0x24ffffu },
    { 34,  0x230000u, 0x23ffffu },
    { 33,  0x220000u, 0x22ffffu },
    { 32,  0x210000u, 0x21ffffu },
    { 31,  0x200000u, 0x20ffffu },
    { 30,  0x1f0000u, 0x1fffffu },
    { 29,  0x1e0000u, 0x1effffu },
    { 28,  0x1d0000u, 0x1dffffu },
    { 27,  0x1c0000u, 0x1cffffu },
    { 26,  0x1b0000u, 0x1bffffu },
    { 25,  0x1a0000u, 0x1affffu },
    { 24,  0x190000u, 0x19ffffu },
    { 23,  0x180000u, 0x18ffffu },
    { 22,  0x170000u, 0x17ffffu },
    { 21,  0x160000u, 0x16ffffu },
    { 20,  0x150000u, 0x15ffffu },
    { 19,  0x140000u, 0x14ffffu },
    { 18,  0x130000u, 0x13ffffu },
    { 17,  0x120000u, 0x12ffffu },
    { 16,  0x110000u, 0x11ffffu },
    { 15,  0x100000u, 0x10ffffu },
    { 14,  0x0f0000u, 0x0fffffu },
    { 13,  0x0e0000u, 0x0effffu },
    { 12,  0x0d0000u, 0x0dffffu },
    { 11,  0x0c0000u, 0x0cffffu },
    { 10,  0x0b0000u, 0x0bffffu },
    { 9,   0x0a0000u, 0x0affffu },
    { 8,   0x090000u, 0x09ffffu },
    { 7,   0x080000u, 0x08ffffu },
    { 6,   0x070000u, 0x07ffffu },
    { 5,   0x060000u, 0x06ffffu },
    { 4,   0x050000u, 0x05ffffu },
    { 3,   0x040000u, 0x04ffffu },
    { 2,   0x030000u, 0x03ffffu },
    { 1,   0x020000u, 0x02ffffu },
    { 0,   0x010000u, 0x01ffffu },
    /* *INDENT-ON* */
};

static int make_wbpr_req(uint8_t *wbpr, size_t offset, bool enable)
{
    int i = BP_REG_COUNT;

    memset(&wbpr[1], 0xff, (size_t)FLASH_EXT_SST26VF_WBPR_LEN);
    wbpr[0] = (uint8_t)SST26VF_WBPR;

    while (i-- != 0) {
        if (offset >= (size_t)bp_reg[i].addr_lo &&
            offset <= (size_t)bp_reg[i].addr_hi) {
            /* select the right bit */
            size_t lshift = (size_t)(bp_reg[i].bit & 0x7);
            size_t index = (size_t)((FLASH_EXT_SST26VF_WBPR_LEN - 1) - (bp_reg[i].bit / 8));

            if (!enable)
                wbpr[index + 1] &= ~(1u << lshift);

            return (int)offset;
        }
    }

    picoRTOS_assert_void(false);
    return -ENOENT;
}

int flash_ext_lock(struct flash_ext *ctx, size_t offset)
{
    picoRTOS_assert((offset % ctx->attr.lock_unit_len) == 0, return -EINVAL);
    picoRTOS_assert(offset < ctx->attr.total_size, return -EINVAL);

    int res = 0;

    for (;;) {
        switch (ctx->seq) {
        case 0: res = run_single(ctx, (uint8_t)SST26VF_WREN); break;
        case 1: res = make_wbpr_req(ctx->wbpr, offset, true); break;
        case 2: res = run_cmd(ctx, ctx->wbpr, sizeof(ctx->wbpr)); break;
        case 3: res = run_busy(ctx); break;
        default: ctx->seq = 0; return 0;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

int flash_ext_unlock(struct flash_ext *ctx, size_t offset)
{
    picoRTOS_assert((offset % ctx->attr.lock_unit_len) == 0, return -EINVAL);
    picoRTOS_assert(offset < ctx->attr.total_size, return -EINVAL);

    int res = 0;

    for (;;) {
        switch (ctx->seq) {
        case 0: res = run_single(ctx, (uint8_t)SST26VF_WREN); break;
        case 1: res = make_wbpr_req(ctx->wbpr, offset, false); break;
        case 2: res = run_cmd(ctx, ctx->wbpr, sizeof(ctx->wbpr)); break;
        case 3: res = run_busy(ctx); break;
        default: ctx->seq = 0; return 0;
        }

        if (res < 0) break;
        ctx->seq++;
    }

    return res;
}

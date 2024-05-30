#include "spi-mc9s08ptxx.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <stdint.h>

struct SPI_MC9S08PTXX {
    volatile uint8_t C1;
    volatile uint8_t C2;
    volatile uint8_t BR;
    volatile uint8_t S;
    volatile uint8_t DH;    /* 16bit */
    volatile uint8_t DL;
    volatile uint8_t MH;    /* 16bit */
    volatile uint8_t ML;
    volatile uint8_t C3;    /* 16bit */
    volatile uint8_t CI;    /* 16bit */
};

#define C1_SPIE  (1 << 7)
#define C1_SPE   (1 << 6)
#define C1_SPTIE (1 << 5)
#define C1_MSTR  (1 << 4)
#define C1_CPOL  (1 << 3)
#define C1_CPHA  (1 << 2)
#define C1_SSOE  (1 << 1)
#define C1_LSBFE (1 << 0)

#define C2_SPMIE   (1 << 7)
#define C2_SPIMODE (1 << 6) /* 16bit */
#define C2_MODFEN  (1 << 4)
#define C2_BIDIROE (1 << 3)
#define C2_SPISWAI (1 << 1)
#define C2_SPC0    (1 << 0)

#define BR_SPRR_M  0x7u
#define BR_SPRR(x) (((x) & BR_SPRR_M) << 4)
#define BR_SPR_M   0xfu
#define BR_SPR(x)  ((x) & BR_SPR_M)

#define S_SPRF  (1 << 7)
#define S_SPMF  (1 << 6)
#define S_SPTEF (1 << 5)
#define S_MODF  (1 << 4)

/* 16bit exclusive */

#define C3_TNEAREF_MARK (1 << 5)
#define C3_RNFULLF_MARK (1 << 4)
#define C3_INTCLR       (1 << 3)
#define C3_TNEARIEN     (1 << 2)
#define C3_RNFULLIEN    (1 << 1)
#define C3_FIFOMODE     (1 << 0)

#define CI_TXFERR    (1 << 7)
#define CI_RXFERR    (1 << 6)
#define CI_TXFOF     (1 << 5)
#define CI_RXFOF     (1 << 4)
#define CI_TNEAREFCI (1 << 3)
#define CI_RNFULLFCI (1 << 2)
#define CI_SPTEFCI   (1 << 1)
#define CI_SPRFCI    (1 << 0)

int spi_mc9s08ptxx_init(struct spi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct SPI_MC9S08PTXX*)base;
    ctx->clkid = clkid;
    ctx->frame_width = (size_t)1;

    /* enable */
    ctx->base->C1 |= C1_SPE;

    return 0;
}

/* hooks */

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    unsigned int spr;
    unsigned int sprr;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return -EIO);

    for (sprr = 1u; sprr <= 8u; sprr++) {
        for (spr = 1u; spr <= 9u; spr++) {
            /* brute-force */
            unsigned long x = (unsigned long)freq /
                              (unsigned long)(sprr * (1u << spr));

            if (x == bitrate) {
                /* setup */
                ctx->base->BR = (uint8_t)(BR_SPRR(sprr - 1) |
                                          BR_SPR(spr - 1));
                return 0;
            }
        }
    }

    picoRTOS_break();
    /*@notreached@*/ return -EINVAL;
}

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    picoRTOS_assert(mode != SPI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < SPI_MODE_COUNT, return -EINVAL);

    /* master */
    if (mode == SPI_MODE_MASTER) {
        ctx->base->C1 |= (C1_MSTR | C1_SSOE);     /* master, auto-SS */
        ctx->base->C2 |= C2_MODFEN;
    }else
        ctx->base->C1 &= ~C1_MSTR;

    return 0;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT, return -EINVAL);

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->C1 &= ~C1_CPOL;
        ctx->base->C1 &= ~C1_CPHA;
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->C1 &= ~C1_CPOL;
        ctx->base->C1 |= C1_CPHA;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->C1 |= C1_CPOL;
        ctx->base->C1 &= ~C1_CPHA;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->C1 |= C1_CPOL;
        ctx->base->C1 |= C1_CPHA;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EINVAL;
    }

    return 0;
}

static int set_frame_size(struct spi *ctx, size_t frame_size)
{
    switch (frame_size) {
    case 8: ctx->base->C2 &= ~C2_SPIMODE; break;
    case 16: ctx->base->C2 |= C2_SPIMODE; break;
    default:
        picoRTOS_assert(false, return -EINVAL);
    }

    ctx->frame_width = frame_size / (size_t)8;
    return 0;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res = 0;

    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if (settings->mode != SPI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    if (settings->clkmode != SPI_CLOCK_MODE_IGNORE &&
        (res = set_clkmode(ctx, settings->clkmode)) < 0)
        return res;

    if (settings->frame_size != 0 &&
        (res = set_frame_size(ctx, settings->frame_size)) < 0)
        return res;

    /* cs_pol: ignore */
    /* cs: ignore */

    return res;
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->S & S_SPRF) != 0) {
        switch (ctx->frame_width) {
        case 1: *(uint8_t*)rx = ctx->base->DL; break;
        case 2: *(uint16_t*)rx = ((uint16_t)ctx->base->DH << 8 |
                                  ctx->base->DL); break;
        default:
            picoRTOS_assert(false, return -EIO);
        }
        /* normal return */
        return (int)ctx->frame_width;
    }

    if ((ctx->base->S & S_SPTEF) != 0) {
        switch (ctx->frame_width) {
        case 1:
            ctx->base->DL = *(uint8_t*)tx;
            break;

        case 2:
            ctx->base->DH = ((uint8_t*)tx)[0];
            ctx->base->DL = ((uint8_t*)tx)[1];
            break;

        default:
            picoRTOS_assert(false, return -EIO);
        }
    }

    return -EAGAIN;
}

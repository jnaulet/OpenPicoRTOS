#include "spi-lgt8fx8p.h"
#include "picoRTOS.h"

#include <stdint.h>

struct SPI_LGT8FX8P {
    volatile uint8_t SPFR;      /* 0x39 */
    uint8_t RESERVED0[18];
    volatile uint8_t SPCR;      /* 0x4c */
    volatile uint8_t SPSR;      /* 0x4d */
    volatile uint8_t SPDR;      /* 0x4e */
};

#define SPFR_RDFULL   (1 << 7)
#define SPFR_RDEMPT   (1 << 6)
#define SPFR_RDPTR_M  0x3u
#define SPFR_RDPTR(x) (((x) & SPFR_RDPTR_M) << 4)
#define SPFR_WRFULL   (1 << 3)
#define SPFR_WREMPTY  (1 << 2)
#define SPFR_WRPTR_M  0x3u
#define SPFR_WRPTR(x) ((x) & SPFR_WRPTR_M)

#define SPCR_SPIE   (1 << 7)
#define SPCR_SPE    (1 << 6)
#define SPCR_DORD   (1 << 5)
#define SPCR_MSTR   (1 << 4)
#define SPCR_CPOL   (1 << 3)
#define SPCR_CPHA   (1 << 2)
#define SPCR_SPR_M  0x3
#define SPCR_SPR(x) ((x) & SPCR_SPR_M)

#define SPSR_SPIF  (1 << 7)
#define SPSR_WCOL  (1 << 6)
#define SPSR_SPI2X (1 << 0)

/* Function: spi_lgt8fx8p_init
 * Initializes a SPI
 *
 * Parameters:
 *  ctx - The SPI to init
 *  base - The SPI base address
 *  clkid - The SPI source clock id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int spi_lgt8fx8p_init(struct spi *ctx, int base, clock_id_t clkid)
{
    /* internals */
    ctx->base = (struct SPI_LGT8FX8P*)base;
    ctx->clkid = clkid;
    ctx->balance = 0;

    /* enable */
    ctx->base->SPCR |= SPCR_SPE;

    return 0;
}

/* hooks */

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    picoRTOS_assert(mode != SPI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < SPI_MODE_COUNT, return -EINVAL);

    /* master */
    if (mode == SPI_MODE_MASTER)
        ctx->base->SPCR |= SPCR_MSTR;
    else
        ctx->base->SPCR &= ~SPCR_MSTR;

    return 0;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT, return -EINVAL);

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->SPCR &= ~SPCR_CPOL;
        ctx->base->SPCR &= ~SPCR_CPHA;
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->SPCR &= ~SPCR_CPOL;
        ctx->base->SPCR |= SPCR_CPHA;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->SPCR |= SPCR_CPOL;
        ctx->base->SPCR &= ~SPCR_CPHA;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->SPCR |= SPCR_CPOL;
        ctx->base->SPCR |= SPCR_CPHA;
        break;

    default:
        picoRTOS_assert_void(false);
        /*@notreached@*/
        return -EINVAL;
    }

    return 0;
}

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    size_t i;
    clock_freq_t freq = clock_get_freq(ctx->clkid);
    static const unsigned spr[] = { 4u, 0u, 5u, 1u, 6u, 2u, 3u };

    /* try to find the closest value */
    for (i = 0; i < sizeof(spr); i++) {
        unsigned long approx = (unsigned long)freq >> (i + 1);
        if (approx <= bitrate) {
            /* sck freq */
            ctx->base->SPCR &= ~SPCR_SPR(SPCR_SPR_M);
            ctx->base->SPCR |= SPCR_SPR(spr[i]);
            ctx->base->SPSR &= ~SPSR_SPI2X;
            ctx->base->SPSR |= SPSR_SPI2X & (spr[i] >> 2);
            return 0;
        }
    }

    picoRTOS_assert_void(false);
    /*@notreached@*/ return -EINVAL;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res = 0;

    /* mode */
    if (settings->mode != SPI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    /* clock mode */
    if (settings->clkmode != SPI_CLOCK_MODE_IGNORE &&
        (res = set_clkmode(ctx, settings->clkmode)) < 0)
        return res;

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* frame_size: ignore */
    /* cs_pol: ignore */
    /* cs: ignore */

    return res;
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    uint8_t *rx8 = (uint8_t*)rx;
    const uint8_t *tx8 = (const uint8_t*)tx;

    int recv = 0;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((n - ctx->balance) != 0) {
        /* fill tx fifo */
        if ((ctx->base->SPFR & SPFR_WRFULL) != 0)
            break;

        ctx->base->SPDR = tx8[ctx->balance++];
    }

    /* empty rx fifo */
    while ((ctx->base->SPFR & SPFR_RDEMPT) == 0 &&
           deadlock-- != 0) rx8[recv++] = ctx->base->SPDR;

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    if (recv == 0)
        return -EAGAIN;

    ctx->balance -= recv;
    return recv;
}

#include "spi-avr.h"
#include "picoRTOS.h"

#include <stdint.h>

struct SPI_AVR {
    volatile uint8_t SPCR;      /* 0x4c */
    volatile uint8_t SPSR;      /* 0x4d */
    volatile uint8_t SPDR;      /* 0x4e */
};

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

/* Function: spi_avr_init
 * Initializes a SPI
 *
 * Parameters:
 *  ctx - The SPI to init
 *  base - The SPI base address
 *  speed - The SPI speed (normal or 2x)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int spi_avr_init(struct spi *ctx, int base, spi_avr_speed_t speed)
{
    picoRTOS_assert(speed < SPI_AVR_SPEED_COUNT, return -EINVAL);

    /* internals */
    ctx->base = (struct SPI_AVR*)base;
    ctx->state = SPI_AVR_STATE_START;

    if (speed == SPI_AVR_SPEED_SPI2X)
        ctx->base->SPSR |= SPSR_SPI2X;

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
        picoRTOS_break();
        /*@notreached@*/
        return -EINVAL;
    }

    return 0;
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

    /* bitrate: ignore */
    /* frame_size: ignore */
    /* cs_pol: ignore */
    /* cs: ignore */

    return res;
}

static int run_state_start(struct spi *ctx, const void *tx)
{
    ctx->base->SPDR = *(uint8_t*)tx;
    ctx->state = SPI_AVR_STATE_XFER;
    return -EAGAIN;
}

static int run_state_xfer(struct spi *ctx, void *rx,
                          const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->SPSR & SPSR_SPIF) == 0)
        return -EAGAIN;

    *(uint8_t*)rx = ctx->base->SPDR; /* SPIF moves to 0 */

    if (n > (size_t)1)
        /* continue xfer */
        ctx->base->SPDR = *((uint8_t*)tx + 1);
    else
        ctx->state = SPI_AVR_STATE_START;

    return 1;
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case SPI_AVR_STATE_START: return run_state_start(ctx, tx);
    case SPI_AVR_STATE_XFER: return run_state_xfer(ctx, rx, tx, n);
    default:
        break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

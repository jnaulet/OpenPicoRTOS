#include "spi-n76e003.h"
#include "picoRTOS_device.h"

#include <picoRTOS.h>

#ifndef S_SPLINT_S
__sfr __at(ADDR_SPCR)  SPCR;
__sfr __at(ADDR_SPCR2) SPCR2; /* page 1 */
__sfr __at(ADDR_SPSR)  SPSR;
__sfr __at(ADDR_SPDR)  SPDR;
#else
static unsigned char SPCR;
/*@unused@*/ static unsigned char SPCR2;
static unsigned char SPSR;
static unsigned char SPDR;
#endif

#define SPCR_SSOE   (1u << 7)
#define SPCR_SPIEN  (1 << 6)
#define SPCR_LSBFE  (1 << 5)
#define SPCR_MSTR   (1 << 4)
#define SPCR_CPOL   (1 << 3)
#define SPCR_CPHA   (1 << 2)
#define SPCR_SPR_M  0x3u
#define SPCR_SPR(x) ((x) & SPCR_SPR_M)

#define SPCR2_SPIS_M  0x3u
#define SPCR2_SPIS(x) ((x) & SPCR2_SPIS_M)

#define SPSR_SPIF    (1u << 7)
#define SPSR_WCOL    (1 << 6)
#define SPSR_SPIOVF  (1 << 5)
#define SPSR_MODF    (1 << 4)
#define SPSR_DISMODF (1 << 3)
#define SPSR_TXBUF   (1 << 2)

int spi_n76e003_init(struct spi *ctx, clock_id_t clkid)
{
    ctx->clkid = clkid;

    /* by default: SS is gpio, unfortunately */
    SPSR |= SPSR_DISMODF;
    SPCR &= ~SPCR_SSOE;

    SPCR |= SPCR_SPIEN;
    return 0;
}

static int set_bitrate(const struct spi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return (int)freq);

    char spr = (char)((unsigned long)freq / bitrate);

    /* max speed */
    SPCR &= ~SPCR_SPR(SPCR_SPR_M);

    switch (spr) {
    case 2: return 0;
    case 4: SPCR |= SPCR_SPR(1); return 0;
    case 8: SPCR |= SPCR_SPR(2); return 0;
    case 16: SPCR |= SPCR_SPR(3); return 0;
    default: break;
    }

    return -EINVAL;
}

static int set_mode(spi_mode_t mode)
{
    picoRTOS_assert(mode != SPI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < SPI_MODE_COUNT, return -EINVAL);

    if (mode == SPI_MODE_MASTER) SPCR |= SPCR_MSTR;
    else SPCR &= ~SPCR_MSTR;

    return 0;
}

static int set_clkmode(spi_clock_mode_t clkmode)
{
    picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT, return -EINVAL);

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        SPCR &= ~SPCR_CPOL;
        SPCR &= ~SPCR_CPHA;
        break;

    case SPI_CLOCK_MODE_1:
        SPCR &= ~SPCR_CPOL;
        SPCR |= SPCR_CPHA;
        break;

    case SPI_CLOCK_MODE_2:
        SPCR |= SPCR_CPOL;
        SPCR &= ~SPCR_CPHA;
        break;

    case SPI_CLOCK_MODE_3:
        SPCR |= SPCR_CPOL;
        SPCR |= SPCR_CPHA;
        break;

    default:
        picoRTOS_assert_void(false);
        /*@notreached@*/
        return -EINVAL;
    }

    return 0;
}

/* cppcheck-suppress [constParameterPointer] */
int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res;

    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if (settings->clkmode != SPI_CLOCK_MODE_IGNORE &&
        (res = set_clkmode(settings->clkmode)) < 0)
        return res;

    /* ignore frame size */

    if (settings->mode != SPI_MODE_IGNORE &&
        (res = set_mode(settings->mode)) < 0)
        return res;

    /* ignore cs_pol */
    /* ignore cs */

    return 0;
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    /*@i@*/ (void)ctx;

    if ((SPSR & SPSR_SPIF) != (unsigned char)0) {
        *(unsigned char*)rx = SPDR;
        SPSR &= ~SPSR_SPIF; /* ack */
        return 1;
    }

    if ((SPSR & SPSR_TXBUF) == (unsigned char)0)
        SPDR = *(unsigned char*)tx;

    return -EAGAIN;
}

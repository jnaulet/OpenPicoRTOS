#include "spi-renesas_ra4.h"
#include "picoRTOS.h"

#include <stdint.h>

struct SPI_RENESAS_RA4 {
    volatile uint8_t SPCR;
    volatile uint8_t SSLP;
    volatile uint8_t SPPCR;
    volatile uint8_t SPSR;
    volatile uint32_t SPDR;
    uint16_t RESERVED0;
    volatile uint8_t SPBR;
    volatile uint8_t SPDCR;
    volatile uint8_t SPCKD;
    volatile uint8_t SSLND;
    volatile uint8_t SPND;
    volatile uint8_t SPCR2;
    volatile uint16_t SPCMD0;
};

#define SPCR_SPRIE  (1 << 7)
#define SPCR_SPE    (1 << 6)
#define SPCR_SPTIE  (1 << 5)
#define SPCR_SPEIE  (1 << 4)
#define SPCR_MSTR   (1 << 3)
#define SPCR_MODFEN (1 << 2)
#define SPCR_TXMD   (1 << 1)
#define SPCR_SPMS   (1 << 0)

#define SSLP_SSL3P (1 << 3)
#define SSLP_SSL2P (1 << 2)
#define SSLP_SSL1P (1 << 1)
#define SSLP_SSL0P (1 << 0)

#define SPPCR_MOIFE (1 << 5)
#define SPPCR_MOIFV (1 << 4)
#define SPPCR_SPLP2 (1 << 1)
#define SPPCR_SPLP  (1 << 0)

#define SPSR_SPRF  (1 << 7)
#define SPSR_SPTEF (1 << 5)
#define SPSR_UDRF  (1 << 4)
#define SPSR_PERF  (1 << 3)
#define SPSR_MODF  (1 << 2)
#define SPSR_IDLNF (1 << 1)
#define SPSR_OVRF0 (1 << 0)

#define SPBR_SPBR_M  0xffu
#define SPBR_SPBR(x) ((x) & SPBR_SPBR_M)

#define SPDCR_SPLW   (1 << 5)
#define SPDCR_SPRDTD (1 << 4)

#define SPCKD_SCKDL_M  0x7u
#define SPCKD_SCKDL(x) ((x) & SPCKD_SCKDL_M)

#define SSLND_SLNDL_M  0x7u
#define SSLND_SLNDL(x) ((x) & SSLND_SLNDL_M)

#define SPND_SPNDL_M  0x7u
#define SPND_SPNDL(x) ((x) & SPND_SPNDL_M)

#define SPCR2_SCKASE (1 << 4)
#define SPCR2_PTE    (1 << 3)
#define SPCR2_SPIIE  (1 << 2)
#define SPCR2_SPOE   (1 << 1)
#define SPCR2_SPPE   (1 << 0)

#define SPCMD0_SCKDEN  (1 << 15)
#define SPCMD0_SLNDEN  (1 << 14)
#define SPCMD0_SPNDEN  (1 << 13)
#define SPCMD0_LSBF    (1 << 12)
#define SPCMD0_SPB_M   0xfu
#define SPCMD0_SPB(x)  (((x) & SPCMD0_SPB_M) << 8)
#define SPCMD0_SSLA_M  0x7u
#define SPCMD0_SSLA(x) (((x) & SPCMD0_SSLA_M) << 4)
#define SPCMD0_BRDV_M  0x3u
#define SPCMD0_BRDV(x) (((x) & SPCMD0_BRDV_M) << 2)
#define SPCMD0_CPOL    (1 << 1)
#define SPCMD0_CPHA    (1 << 0)

/* Function: spi_renesas_ra4_init
 * Initializes a SPI
 *
 * Parameters:
 *  ctx - The SPI to init
 *  base - The SPI base address
 *  clkid - The SPI peripheral clock ID
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int spi_renesas_ra4_init(struct spi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct SPI_RENESAS_RA4*)base;
    ctx->clkid = clkid;
    ctx->ssl = 0;

    /* disable anyway */
    ctx->base->SPCR &= ~SPCR_SPE;
    /* word access only */
    ctx->base->SPDCR |= SPDCR_SPLW;

    return 0;
}

static int set_loopback(struct spi *ctx, bool loopback)
{
    if (loopback) ctx->base->SPPCR |= SPPCR_SPLP2;
    else ctx->base->SPPCR &= ~SPPCR_SPLP2;

    return 0;
}

int spi_renesas_ra4_setup(struct spi *ctx, const struct spi_renesas_ra4_settings *settings)
{
    return set_loopback(ctx, settings->loopback);
}

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    unsigned long n;
    unsigned long p2;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return -EIO);

    for (n = 0; n < (unsigned long)SPBR_SPBR_M; n++) {
        for (p2 = 0; p2 <= (unsigned long)SPCMD0_BRDV_M; p2++) {
            unsigned long f = (unsigned long)freq / (2ul * (n + 1ul) * (1ul << p2));
            if (f == bitrate) {
                ctx->base->SPBR = (uint8_t)SPBR_SPBR(n);
                ctx->base->SPCMD0 &= ~SPCMD0_BRDV(SPCMD0_BRDV_M);
                ctx->base->SPCMD0 |= SPCMD0_BRDV(p2);
                return 0;
            }
        }
    }

    picoRTOS_break();
    /*@notreached@*/ return -EINVAL;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT, return -EINVAL);

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->SPCMD0 &= ~SPCMD0_CPOL;
        ctx->base->SPCMD0 &= ~SPCMD0_CPHA;
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->SPCMD0 &= ~SPCMD0_CPOL;
        ctx->base->SPCMD0 |= SPCMD0_CPHA;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->SPCMD0 |= SPCMD0_CPOL;
        ctx->base->SPCMD0 &= ~SPCMD0_CPHA;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->SPCMD0 |= SPCMD0_CPOL;
        ctx->base->SPCMD0 |= SPCMD0_CPHA;
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
#define div_ceil(x, y) (((x) + ((y) - 1)) / (y))

    picoRTOS_assert(frame_size >= (size_t)SPI_RENESAS_RA4_FRAME_SIZE_MIN, return -EINVAL);
    picoRTOS_assert(frame_size <= (size_t)SPI_RENESAS_RA4_FRAME_SIZE_MAX, return -EINVAL);

    int spb = 15;

    ctx->frame_size = frame_size;
    ctx->frame_width = div_ceil(frame_size, (size_t)8);

    switch (frame_size) {
    case 8: spb = 4; break;
    case 9:     /*@fallthrough@*/
    case 10:    /*@fallthrough@*/
    case 11:    /*@fallthrough@*/
    case 12:    /*@fallthrough@*/
    case 13:    /*@fallthrough@*/
    case 14:    /*@fallthrough@*/
    case 15:    /*@fallthrough@*/
    case 16: spb = (int)frame_size - 1; break;
    case 20: spb = 0; break;
    case 24: spb = 1; break;
    case 32: spb = 2; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    ctx->base->SPCMD0 &= ~SPCMD0_SPB(SPCMD0_SPB_M);
    ctx->base->SPCMD0 |= SPCMD0_SPB(spb);

    return 0;
}

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    picoRTOS_assert(mode != SPI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < SPI_MODE_COUNT, return -EINVAL);

    if (mode == SPI_MODE_MASTER) ctx->base->SPCR |= SPCR_MSTR;
    else ctx->base->SPCR &= ~SPCR_MSTR;

    return 0;
}

static int set_cs_pol(struct spi *ctx, spi_cs_pol_t cs_pol)
{
    picoRTOS_assert(cs_pol != SPI_CS_POL_IGNORE, return -EINVAL);
    picoRTOS_assert(cs_pol < SPI_CS_POL_COUNT, return -EINVAL);

    uint8_t mask = (uint8_t)(1 << ctx->ssl);

    if (cs_pol == SPI_CS_POL_ACTIVE_LOW) ctx->base->SSLP &= ~mask;
    else ctx->base->SSLP |= mask;

    return 0;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    picoRTOS_assert(settings->cs < (size_t)SPI_RENESAS_RA4_SSL_COUNT, return -EINVAL);

    int res;

    /* 1st: disable */
    ctx->base->SPCR &= ~SPCR_SPE;

    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if (settings->clkmode != SPI_CLOCK_MODE_IGNORE &&
        (res = set_clkmode(ctx, settings->clkmode)) < 0)
        return res;

    if (settings->frame_size != 0 &&
        (res = set_frame_size(ctx, settings->frame_size)) < 0)
        return res;

    if (settings->mode != SPI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    /* cs */
    ctx->ssl = settings->cs;
    ctx->base->SPCMD0 &= ~SPCMD0_SSLA(SPCMD0_SSLA_M);
    ctx->base->SPCMD0 |= SPCMD0_SSLA(ctx->ssl);

    if (settings->cs_pol != SPI_CS_POL_IGNORE &&
        (res = set_cs_pol(ctx, settings->cs_pol)) < 0)
        return res;

    /* enable back */
    ctx->base->SPCR |= SPCR_SPE;

    return 0;
}

static int receive_data(struct spi *ctx, uint8_t *data)
{
    if ((ctx->base->SPSR & SPSR_SPRF) == 0)
        return -EAGAIN;

    uint32_t spi_data = ctx->base->SPDR;

    switch (ctx->frame_width) {
    case sizeof(uint8_t):
        *data = (uint8_t)spi_data;
        return (int)sizeof(uint8_t);

    case sizeof(uint16_t):
        *data = (uint16_t)spi_data;
        return (int)sizeof(uint16_t);

    default:
        break;
    }

    *(uint32_t*)data = spi_data;
    return (int)sizeof(uint32_t);
}

static int transmit_data(struct spi *ctx, const uint8_t *data)
{
    if ((ctx->base->SPSR & SPSR_SPTEF) == 0)
        return -EAGAIN;

    switch (ctx->frame_width) {
    case sizeof(uint8_t):
        ctx->base->SPDR = (uint32_t)*data;
        return (int)sizeof(uint8_t);

    case sizeof(uint16_t):
        ctx->base->SPDR = (uint32_t)*(uint16_t*)data;
        return (int)sizeof(uint16_t);

    default:
        break;
    }

    ctx->base->SPDR = *(uint32_t*)data;
    return (int)sizeof(uint32_t);
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert((n & (ctx->frame_width - 1)) == 0, return -EINVAL);

    size_t recv = 0;
    uint8_t *rx8 = rx;
    const uint8_t *tx8 = tx;

    while (recv != n) {

        int res;
        bool xfer_occurred = false;
        size_t tx_n = recv + ctx->balance;

        /* balance should never exceed 1 on this chip */
        if (ctx->balance == 0) {
            /* fill up TX FIFO */
            if ((res = transmit_data(ctx, &tx8[tx_n])) > 0) {
                ctx->balance += res;
                xfer_occurred = true;
            }
        }

        if ((res = receive_data(ctx, &rx8[recv])) > 0) {
            recv += (size_t)res;
            ctx->balance -= res;
            xfer_occurred = true;
        }

        /* nothing was xfered, out */
        if (!xfer_occurred)
            break;
    }

    if (recv == 0)
        return -EAGAIN;

    return (int)recv;
}

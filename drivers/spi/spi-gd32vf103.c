#include "spi-gd32vf103.h"
#include "picoRTOS.h"

#include <stdint.h>

struct SPI_GD32VF103 {
    volatile uint32_t SPI_CTL0;
    volatile uint32_t SPI_CTL1;
    volatile uint32_t SPI_STAT;
    volatile uint32_t SPI_DATA;
    volatile uint32_t SPI_CRCPOLY;
    volatile uint32_t SPI_RCRC;
    volatile uint32_t SPI_TCRC;
    volatile uint32_t SPI_I2SCTL;
    volatile uint32_t SPI_I2SPSC;
};

#define SPI_CTL0_BDEN    (1 << 15)
#define SPI_CTL0_BDOEN   (1 << 14)
#define SPI_CTL0_CRCEN   (1 << 13)
#define SPI_CTL0_CRCNT   (1 << 12)
#define SPI_CTL0_FF16    (1 << 11)
#define SPI_CTL0_RO      (1 << 10)
#define SPI_CTL0_SWNSSEN (1 << 9)
#define SPI_CTL0_SWNSS   (1 << 8)
#define SPI_CTL0_LF      (1 << 7)
#define SPI_CTL0_SPIEN   (1 << 6)
#define SPI_CTL0_PSC_M   0x7u
#define SPI_CTL0_PSC(x)  (((x) & SPI_CTL0_PSC_M) << 3)
#define SPI_CTL0_MSTMOD  (1 << 2)
#define SPI_CTL0_CKPL    (1 << 1)
#define SPI_CTL0_CKPH    (1 << 0)

#define SPI_CTL1_TBEIE  (1 << 7)
#define SPI_CTL1_RNBEIE (1 << 6)
#define SPI_CTL1_ERRIE  (1 << 5)
#define SPI_CTL1_TMOD   (1 << 4)
#define SPI_CTL1_NSSP   (1 << 3)
#define SPI_CTL1_NSSDRV (1 << 2)
#define SPI_CTL1_DMATEN (1 << 1)
#define SPI_CTL1_DMAREN (1 << 0)

#define SPI_STAT_FERR    (1 << 8)
#define SPI_STAT_TRANS   (1 << 7)
#define SPI_STAT_RXOERR  (1 << 6)
#define SPI_STAT_CONFERR (1 << 5)
#define SPI_STAT_CRCERR  (1 << 4)
#define SPI_STAT_TXURERR (1 << 3)
#define SPI_STAT_I2SCH   (1 << 2)
#define SPI_STAT_TBE     (1 << 1)
#define SPI_STAT_RBNE    (1 << 0)

/* Function: spi_gd32vf103_init
 * Initializes a SPI
 *
 * Parameters:
 *  ctx - The SPI to init
 *  base - The SPI base address
 *  clkid - The clock id for this SPI
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int spi_gd32vf103_init(struct spi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct SPI_GD32VF103*)base;
    ctx->clkid = clkid;
    ctx->frame_size = (size_t)8;
    ctx->frame_width = (size_t)1;

    /* dma opt */
    ctx->state = SPI_GD32VF103_STATE_DMA_START;
    ctx->fill = NULL;
    ctx->drain = NULL;
    ctx->threshold = 0;

    /* disable */
    ctx->base->SPI_CTL0 = (uint32_t)0;

    return 0;
}

int spi_gd32vf103_setup(struct spi *ctx, struct spi_gd32vf103_settings *settings)
{
    ctx->threshold = settings->threshold;
    ctx->fill = settings->fill;
    ctx->drain = settings->drain;

    return 0;
}

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    unsigned long psc;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    for (psc = 1ul; psc <= 8ul; psc++) {
        unsigned long f = (unsigned long)freq >> psc;
        if (f == bitrate) {
            ctx->base->SPI_CTL0 &= ~SPI_CTL0_PSC(SPI_CTL0_PSC_M);
            ctx->base->SPI_CTL0 |= SPI_CTL0_PSC(psc - 1ul);
            return 0;
        }
    }

    picoRTOS_break();
    /*@notreached@*/ return -EINVAL;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    if (!picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT)) return -EINVAL;

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->SPI_CTL0 &= ~SPI_CTL0_CKPL;
        ctx->base->SPI_CTL0 &= ~SPI_CTL0_CKPH;
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->SPI_CTL0 &= ~SPI_CTL0_CKPL;
        ctx->base->SPI_CTL0 |= SPI_CTL0_CKPH;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->SPI_CTL0 |= SPI_CTL0_CKPL;
        ctx->base->SPI_CTL0 &= ~SPI_CTL0_CKPH;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->SPI_CTL0 |= SPI_CTL0_CKPL;
        ctx->base->SPI_CTL0 |= SPI_CTL0_CKPH;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    return 0;
}

static int set_frame_size(struct spi *ctx, size_t frame_size)
{
#define div_ceil(x, y) (((x) + ((y) - 1)) / (y))

    if (!picoRTOS_assert(frame_size > 0)) return -EINVAL;

    switch (frame_size) {
    case 8: ctx->base->SPI_CTL0 &= ~SPI_CTL0_FF16; break;
    case 16: ctx->base->SPI_CTL0 |= SPI_CTL0_FF16; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    ctx->frame_size = frame_size;
    ctx->frame_width = div_ceil(frame_size, (size_t)8);

    return 0;
}

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    if (!picoRTOS_assert(mode != SPI_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(mode < SPI_MODE_COUNT)) return -EINVAL;

    if (mode == SPI_MODE_MASTER) {
        ctx->base->SPI_CTL0 |= SPI_CTL0_MSTMOD;
        ctx->base->SPI_CTL1 |= SPI_CTL1_NSSDRV;
    }else
        ctx->base->SPI_CTL0 &= ~SPI_CTL0_MSTMOD;

    return 0;
}

static int spi_enable(struct spi *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* turn on */
    while ((ctx->base->SPI_CTL0 & SPI_CTL0_SPIEN) == 0 && deadlock-- != 0) {
        /*@i@*/ (void)ctx->base->SPI_STAT;
        ctx->base->SPI_CTL0 |= SPI_CTL0_SPIEN;
    }

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res;

    ctx->base->SPI_CTL0 &= ~SPI_CTL0_SPIEN;

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

    /* ignore cs & cs_pol */

    return spi_enable(ctx);
}

static int receive_data(struct spi *ctx, uint8_t *data)
{
    if ((ctx->base->SPI_STAT & SPI_STAT_RBNE) == 0)
        return -EAGAIN;

    uint32_t spi_data = ctx->base->SPI_DATA;

    switch (ctx->frame_width) {
    case sizeof(uint8_t):
        *data = (uint8_t)spi_data;
        return (int)sizeof(uint8_t);

    default:
        break;
    }

    *(uint16_t*)data = (uint16_t)spi_data;
    return (int)sizeof(uint16_t);
}

static int transmit_data(struct spi *ctx, const uint8_t *data)
{
    if ((ctx->base->SPI_STAT & SPI_STAT_TBE) == 0)
        return -EAGAIN;

    switch (ctx->frame_width) {
    case sizeof(uint8_t):
        ctx->base->SPI_DATA = (uint32_t)*data;
        return (int)sizeof(uint8_t);

    default:
        break;
    }

    ctx->base->SPI_DATA = (uint32_t)*(uint16_t*)data;
    return (int)sizeof(uint16_t);
}

static int spi_xfer_dma_start(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    /* null check */
    if ( !picoRTOS_assert(ctx->fill != NULL)) return -EIO;
    if ( !picoRTOS_assert(ctx->drain != NULL)) return -EIO;

    /* fill xfer */
    struct dma_xfer fill = {
        (intptr_t)tx,                   /* source address */
        (intptr_t)&ctx->base->SPI_DATA, /* destination addresss */
        true,                           /* incr_read */
        false,                          /* incr_write */
        ctx->frame_width,               /* size */
        n                               /* byte count */
    };

    /* drain xfer */
    struct dma_xfer drain = {
        (intptr_t)&ctx->base->SPI_DATA, /* source addresss */
        (intptr_t)rx,                   /* destination address */
        false,                          /* incr_read */
        true,                           /* incr_write */
        ctx->frame_width,               /* size */
        n                               /* byte count */
    };

    /* register xfers */
    (void)dma_setup(ctx->fill, &fill);
    (void)dma_setup(ctx->drain, &drain);

    /* trigger xfers */
    ctx->base->SPI_CTL1 |= (SPI_CTL1_DMATEN | SPI_CTL1_DMAREN);

    ctx->state = SPI_GD32VF103_STATE_DMA_WAIT;
    return -EAGAIN;
}

static int spi_xfer_dma_wait(struct spi *ctx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    /* null check */
    if ( !picoRTOS_assert(ctx->fill != NULL)) return -EIO;
    if ( !picoRTOS_assert(ctx->drain != NULL)) return -EIO;

    if (dma_xfer_done(ctx->fill) == 0 &&
        dma_xfer_done(ctx->drain) == 0) {
        /* end xfers */
        ctx->base->SPI_CTL1 &= ~(SPI_CTL1_DMATEN | SPI_CTL1_DMAREN);
        /* back to normal */
        ctx->state = SPI_GD32VF103_STATE_DMA_START;
        return (int)n;
    }

    return -EAGAIN;
}

static int spi_xfer_dma(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case SPI_GD32VF103_STATE_DMA_START: return spi_xfer_dma_start(ctx, rx, tx, n);
    case SPI_GD32VF103_STATE_DMA_WAIT: return spi_xfer_dma_wait(ctx, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int spi_xfer_nodma(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert((n & (ctx->frame_width - 1)) == 0)) return -EINVAL;

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

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* DMA */
    if (ctx->fill != NULL && ctx->drain != NULL &&
        n >= ctx->threshold)
        return spi_xfer_dma(ctx, rx, tx, n);

    /* NO DMA */
    return spi_xfer_nodma(ctx, rx, tx, n);
}

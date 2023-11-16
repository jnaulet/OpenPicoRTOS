#include "spi-ti_f28x.h"
#include "picoRTOS.h"

#define SPI_TI_F28X_FIFO_COUNT 16

struct SPI_REGS {
    volatile uint16_t SPICCR;
    volatile uint16_t SPICTL;
    volatile uint16_t SPISTS;
    uint16_t RESERVED0;
    volatile uint16_t SPIBRR;
    uint16_t RESERVED1;
    volatile uint16_t SPIRXEMU;
    volatile uint16_t SPIRXBUF;
    volatile uint16_t SPITXBUF;
    volatile uint16_t SPIDAT;
    volatile uint16_t SPIFFTX;
    volatile uint16_t SPIFFRX;
    volatile uint16_t SPIFFCT;
    uint16_t RESERVED2[2];
    volatile uint16_t SPIPRI;
};

#define SPICCR_SPISWRESET  (1 << 7)
#define SPICCR_CLKPOLARITY (1 << 6)
#define SPICCR_SPILBK      (1 << 4)
#define SPICCR_SPICHAR_M   0xfu
#define SPICCR_SPICHAR(x)  ((x) & SPICCR_SPICHAR_M)

#define SPICTL_CLKPHASE    (1 << 3)
#define SPICTL_MASTERSLAVE (1 << 2)
#define SPICTL_TALK        (1 << 1)
#define SPICTL_SPIINTENA   (1 << 0)

#define SPISTS_OVERRUN  (1 << 7)
#define SPISTS_INT_FLAG (1 << 6)
#define SPISTS_BUFFULL  (1 << 5)

#define SPIBRR_SPIBITRATE_M  0x7fu
#define SPIBRR_SPIBITRATE(x) ((x) & SPIBRR_SPIBITRATE_M)

#define SPIFFTX_SPIRST      (1 << 15)
#define SPIFFTX_SPIFFENA    (1 << 14)
#define SPIFFTX_TXFIFO      (1 << 13)
#define SPIFFTX_TXFFST_M    0x1fu
#define SPIFFTX_TXFFST(x)   (((x) & SPIFFTX_TXFFST_M) << 8)
#define SPIFFTX_TXFFINT     (1 << 7)
#define SPIFFTX_TXFFINTCLR  (1 << 6)
#define SPIFFTX_TXFFIL_M    0x1fu
#define SPIFFTX_TXFFIL(x)   ((x) & SPIFFTX_TXFFIL_M)

#define SPIFFRX_RXFIFORESET (1 << 13)
#define SPIFFRX_RXFFST_M    0x1fu
#define SPIFFRX_RXFFST(x)   (((x) & SPIFFRX_RXFFST_M) << 8)
#define SPIFFRX_RXFFINT     (1 << 7)
#define SPIFFRX_RXFFINTCLR  (1 << 6)
#define SPIFFRX_RXFFIL_M    0x1fu
#define SPIFFRX_RXFFIL(x)   ((x) & SPIFFRX_RXFFIL_M)

#define SPIPRI_FREE (1 << 4)

static int init_fifos(struct spi *ctx)
{
    /* reset fifos */
    ctx->base->SPIFFTX = (uint16_t)0;
    ctx->base->SPIFFRX = (uint16_t)0;

    /* start tx fifo */
    ctx->base->SPIFFTX = (uint16_t)(SPIFFTX_TXFFINTCLR | SPIFFTX_TXFIFO |
                                    SPIFFTX_SPIFFENA | SPIFFTX_SPIRST |
                                    SPIFFTX_TXFFIL(SPI_TI_F28X_FIFO_COUNT - 1));
    /* start rx fifo */
    ctx->base->SPIFFRX = (uint16_t)(SPIFFRX_RXFIFORESET | SPIFFRX_RXFFINTCLR |
                                    SPIFFRX_RXFFIL(1));

    /* no delay */
    ctx->base->SPIFFCT = 0u;

    return 0;
}

/* Function: spi_ti_f28x_init
 * Inits a SPI
 *
 * Parameters:
 *  ctx - The SPI to init
 *  base - The SPI base address
 *  clkid - The SPI input clock id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int spi_ti_f28x_init(struct spi *ctx, int base, clock_id_t clkid)
{
    int res;

    ctx->base = (struct SPI_REGS*)base;
    ctx->clkid = clkid;
    ctx->balance = 0;
    ctx->lshift = 0;
    ctx->frame_size = (size_t)16;

    /* dma opt */
    ctx->state = SPI_TI_F28X_STATE_DMA_START;
    ctx->fill = NULL;
    ctx->drain = NULL;
    ctx->threshold = (size_t)0;

    /* fifos */
    if ((res = init_fifos(ctx)) < 0)
        return res;

    /* hold reset low for programming */
    ctx->base->SPICCR = (uint16_t)0;

    ctx->base->SPICTL |= SPICTL_TALK;       /* tx enable */
    ctx->base->SPICTL &= ~SPICTL_SPIINTENA; /* no interrupts */
    ctx->base->SPIPRI |= SPIPRI_FREE;       /* debug */

    /* out of reset */
    ctx->base->SPICCR |= SPICCR_SPISWRESET;

    return 0;
}

static int set_loopback(struct spi *ctx, bool loopback)
{
    /* hold reset low for programming */
    ctx->base->SPICCR &= ~SPICCR_SPISWRESET;

    /* set loopback */
    if (loopback) ctx->base->SPICCR |= SPICCR_SPILBK;
    else ctx->base->SPICCR &= ~SPICCR_SPILBK;

    /* un-reset (inverted, nice job !) */
    ctx->base->SPICCR |= SPICCR_SPISWRESET;

    return 0;
}

/* Function: spi_ti_f28x_setup
 * Configures SPI specifics (DMA, loopback)
 *
 * Parameters:
 *  ctx - The spi to setup
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int spi_ti_f28x_setup(struct spi *ctx, struct spi_ti_f28x_settings *settings)
{
    ctx->fill = settings->fill;
    ctx->drain = settings->drain;
    ctx->threshold = settings->threshold;

    return set_loopback(ctx, settings->loopback);
}

/* hooks */

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    /* According to RM, BRR = (LSPCLK freq / SPI CLK freq) - 1 */
    clock_freq_t freq;

    if ((freq = clock_get_freq(ctx->clkid)) < 0)
        return (int)freq;

    unsigned long ufreq = (unsigned long)freq;
    uint16_t brr = (uint16_t)(ufreq / bitrate) - 1u;

    ctx->base->SPIBRR = (uint16_t)SPIBRR_SPIBITRATE(brr);

    return 0;
}

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    picoRTOS_assert(mode != SPI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < SPI_MODE_COUNT, return -EINVAL);

    if (mode == SPI_MODE_MASTER) ctx->base->SPICTL |= SPICTL_MASTERSLAVE;
    else ctx->base->SPICTL &= ~SPICTL_MASTERSLAVE;

    return 0;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT, return -EINVAL);

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->SPICCR &= ~SPICCR_CLKPOLARITY;
        ctx->base->SPICTL &= ~SPICTL_CLKPHASE;
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->SPICCR &= ~SPICCR_CLKPOLARITY;
        ctx->base->SPICTL |= SPICTL_CLKPHASE;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->SPICCR |= SPICCR_CLKPOLARITY;
        ctx->base->SPICTL &= ~SPICTL_CLKPHASE;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->SPICCR |= SPICCR_CLKPOLARITY;
        ctx->base->SPICTL |= SPICTL_CLKPHASE;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    return 0;
}

static int set_frame_size(struct spi *ctx, size_t frame_size)
{
    picoRTOS_assert(frame_size <= (size_t)16, return -EINVAL);

    ctx->base->SPICCR &= ~SPICCR_SPICHAR_M;
    ctx->base->SPICCR |= SPICCR_SPICHAR(frame_size - 1);

    ctx->frame_size = frame_size;
    ctx->lshift = (size_t)16 - frame_size;

    return 0;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res = 0;

    ctx->base->SPICCR &= ~SPICCR_SPISWRESET;

    /* bitrate */
    if (settings->bitrate != 0 &&
        ((res = set_bitrate(ctx, settings->bitrate)) < 0))
        return res;

    /* mode */
    if (settings->mode != SPI_MODE_IGNORE &&
        ((res = set_mode(ctx, settings->mode)) < 0))
        return res;

    /* clkmode */
    if (settings->mode != SPI_MODE_IGNORE &&
        ((res = set_clkmode(ctx, settings->clkmode)) < 0))
        return res;

    /* frame size */
    if (settings->frame_size != 0 &&
        ((res = set_frame_size(ctx, settings->frame_size)) < 0))
        return res;

    /* cs_pol: ignore */
    /* cs: ignore */

    ctx->base->SPICCR |= SPICCR_SPISWRESET;
    return res;
}

static int write_char(struct spi *ctx, uint16_t data)
{
    if ((ctx->base->SPIFFTX & SPIFFTX_TXFFINT) == 0)
        return -EAGAIN;

    /* data is left-justified on tx */
    ctx->base->SPITXBUF = (data << ctx->lshift);
    ctx->base->SPIFFTX |= SPIFFTX_TXFFINTCLR;
    return 1;
}

static int read_char(struct spi *ctx, uint16_t *data)
{
    if ((ctx->base->SPIFFRX & SPIFFRX_RXFFINT) == 0)
        return -EAGAIN;

    /* data is right-justified on rx */
    *data = ctx->base->SPIRXBUF;
    ctx->base->SPIFFRX |= SPIFFRX_RXFFINTCLR;
    return 1;
}

static int spi_xfer_dma_start(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    /* null check */
    picoRTOS_assert(ctx->fill != NULL, return -EIO);
    picoRTOS_assert(ctx->drain != NULL, return -EIO);

    int res;

    /* fill xfer */
    struct dma_xfer fill = {
        (intptr_t)tx,                   /* source address */
        (intptr_t)&ctx->base->SPITXBUF, /* destination address */
        DMA_XFER_INCREMENT_ON,          /* incr_read */
        DMA_XFER_INCREMENT_OFF,         /* incr_write */
        sizeof(uint16_t),               /* size */
        n                               /* byte count */
    };

    /* drain xfer */
    struct dma_xfer drain = {
        (intptr_t)&ctx->base->SPIRXBUF, /* source address */
        (intptr_t)rx,                   /* destination address */
        DMA_XFER_INCREMENT_OFF,         /* incr_read */
        DMA_XFER_INCREMENT_ON,          /* incr_write */
        sizeof(uint16_t),               /* size */
        n                               /* byte count */
    };

    /* register xfers */
    if ((res = dma_setup(ctx->drain, &drain)) < 0 ||
        (res = dma_setup(ctx->fill, &fill)) < 0)
        return res;

    ctx->state = SPI_TI_F28X_STATE_DMA_WAIT;
    return -EAGAIN;
}

static int spi_xfer_dma_wait(struct spi *ctx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    /* null check */
    picoRTOS_assert(ctx->fill != NULL, return -EIO);
    picoRTOS_assert(ctx->drain != NULL, return -EIO);

    if (dma_xfer_done(ctx->fill) == 0 &&
        dma_xfer_done(ctx->drain) == 0) {
        /* back to normal */
        ctx->state = SPI_TI_F28X_STATE_DMA_START;
        return (int)n;
    }

    return -EAGAIN;
}

static int spi_xfer_dma(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case SPI_TI_F28X_STATE_DMA_START: return spi_xfer_dma_start(ctx, rx, tx, n);
    case SPI_TI_F28X_STATE_DMA_WAIT: return spi_xfer_dma_wait(ctx, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int spi_xfer_nodma(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    size_t recv = 0;
    uint16_t *rx16 = rx;
    const uint16_t *tx16 = tx;

    while (recv != n) {

        int res;
        bool xfer_occurred = false;
        size_t tx_n = recv + ctx->balance;

        if (tx_n < n) {
            /* fill up TX FIFO */
            if ((res = write_char(ctx, tx16[tx_n])) > 0) {
                ctx->balance += res;
                xfer_occurred = true;
            }
        }

        if ((res = read_char(ctx, &rx16[recv])) > 0) {
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
    picoRTOS_assert(n > 0, return -EINVAL);

    /* DMA */
    if (ctx->fill != NULL && ctx->drain != NULL &&
        n >= ctx->threshold)
        return spi_xfer_dma(ctx, rx, tx, n);

    /* NO DMA */
    return spi_xfer_nodma(ctx, rx, tx, n);
}

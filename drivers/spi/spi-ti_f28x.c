#include "spi-ti_f28x.h"
#include "picoRTOS.h"

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

#define SPIFFTX_SPIRST     (1 << 15)
#define SPIFFTX_SPIFFENA   (1 << 14)
#define SPIFFTX_TXFIFO     (1 << 13)
#define SPIFFTX_TXFFINT    (1 << 7)
#define SPIFFTX_TXFFINTCLR (1 << 6)

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
                                    SPIFFTX_SPIFFENA | SPIFFTX_SPIRST);
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

/* Function: spi_ti_f28x_set_loopback
 * Sets the SPI in loopback mode (for debug)
 *
 * Parameters:
 *  ctx - The spi to setup
 *  loopback - enable/disable loopback mode
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int spi_ti_f28x_set_loopback(struct spi *ctx, bool loopback)
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

/* hooks */

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

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
    if (!picoRTOS_assert(mode != SPI_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(mode < SPI_MODE_COUNT)) return -EINVAL;

    if (mode == SPI_MODE_MASTER) ctx->base->SPICTL |= SPICTL_MASTERSLAVE;
    else ctx->base->SPICTL &= ~SPICTL_MASTERSLAVE;

    return 0;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    if (!picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT)) return -EINVAL;

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
    if (!picoRTOS_assert(frame_size <= (size_t)16)) return -EINVAL;

    ctx->base->SPICCR &= ~SPICCR_SPICHAR_M;
    ctx->base->SPICCR |= SPICCR_SPICHAR(frame_size - 1);

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
    if ((ctx->base->SPISTS & SPISTS_BUFFULL) != 0)
        return -EAGAIN;

    /* data is left-justified on tx */
    ctx->base->SPITXBUF = (data << ctx->lshift);
    return 1;
}

static int read_char(struct spi *ctx, uint16_t *data)
{
    if ((ctx->base->SPISTS & SPISTS_INT_FLAG) == 0)
        return -EAGAIN;

    /* overrun */
    if ((ctx->base->SPISTS & SPISTS_OVERRUN) != 0) {
        ctx->base->SPISTS |= SPISTS_OVERRUN;
        return -EPIPE;
    }

    /* data is right-justified on rx */
    *data = ctx->base->SPIRXBUF;
    return 1;
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

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

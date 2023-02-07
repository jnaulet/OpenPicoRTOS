#include "spi-attiny1x.h"
#include "picoRTOS.h"

#include <stdint.h>

struct SPI_ATTINY1X {
    volatile uint8_t CTRLA;
    volatile uint8_t CTRLB;
    volatile uint8_t INTCTRL;
    volatile uint8_t INTFLAGS;
    volatile uint8_t DATA;
};

#define CTRLA_DORD     (1 << 6)
#define CTRLA_MASTER   (1 << 5)
#define CTRLA_CLK2X    (1 << 4)
#define CTRLA_PRESC_M  0x3u
#define CTRLA_PRESC(x) (((x) & CTRLA_PRESC_M) << 1)
#define CTRLA_ENABLE   (1 << 0)

#define CTRLB_BUFEN   (1 << 7)
#define CTRLB_BUFWR   (1 << 6)
#define CTRLB_SSD     (1 << 2)
#define CTRLB_MODE_M  0x3u
#define CTRLB_MODE(x) ((x) & CTRLB_MODE_M)

#define INTFLAGS_IF     (1 << 7)
#define INTFLAGS_RXCIF  (1 << 7)
#define INTFLAGS_WRCOL  (1 << 6)
#define INTFLAGS_TXCIF  (1 << 6)
#define INTFLAGS_DREIF  (1 << 5)
#define INTFLAGS_SSIF   (1 << 4)
#define INTFLAGS_BUFOVF (1 << 0)

/* Function: spi_attiny1x_init
 * Initializes a SPI
 *
 * Parameters:
 *  ctx - The SPI to init
 *  base - The SPI base address
 *  clkid - The SPI clock ID (usually PER_CLK)
 *
 * Returns:
 * always 0
 */
int spi_attiny1x_init(struct spi *ctx, struct SPI_ATTINY1X *base, clock_id_t clkid)
{
    ctx->base = base;
    ctx->clkid = clkid;
    ctx->balance = 0;

    /* turn on */
    ctx->base->CTRLA = (uint8_t)CTRLA_ENABLE;
    ctx->base->CTRLB = (uint8_t)CTRLB_BUFEN;

    return 0;
}

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    if (!picoRTOS_assert(mode != SPI_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(mode < SPI_MODE_COUNT)) return -EINVAL;

    /* master */
    if (mode == SPI_MODE_MASTER)
        ctx->base->CTRLA |= CTRLA_MASTER;
    else
        ctx->base->CTRLA &= ~CTRLA_MASTER;

    return 0;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    if (!picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT)) return -EINVAL;

    ctx->base->CTRLB &= ~CTRLB_MODE(CTRLB_MODE_M);
    ctx->base->CTRLB |= CTRLB_MODE(clkmode);

    return 0;
}

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    /* can only play with presc & clk2x,
    * this will be very approximative */
    clock_freq_t freq = clock_get_freq(ctx->clkid);
    unsigned long pdiv = (unsigned long)freq / bitrate;

    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    /* find closest power of 2... */
    ctx->base->CTRLA &= ~(CTRLA_CLK2X | CTRLA_PRESC(CTRLA_PRESC_M));

    if (pdiv > 127ul)
        ctx->base->CTRLA |= CTRLA_PRESC(3);
    else if (pdiv > 31ul) {
        if (pdiv < 64ul) ctx->base->CTRLA |= CTRLA_CLK2X;
        ctx->base->CTRLA |= CTRLA_PRESC(2);
    }else if (pdiv > 7ul) {
        if (pdiv < 16ul) ctx->base->CTRLA |= CTRLA_CLK2X;
        ctx->base->CTRLA |= CTRLA_PRESC(1);
    }else if (pdiv > 1ul) {
        if (pdiv < 4ul) ctx->base->CTRLA |= CTRLA_CLK2X;
        ctx->base->CTRLA |= CTRLA_PRESC(0);
    }else{
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

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* frame_size: ignore */
    /* cs_pol: ignore */
    /* cs: ignore */

    return res;
}

static int receive_data(struct spi *ctx, uint8_t *data)
{
    if ((ctx->base->INTFLAGS & INTFLAGS_RXCIF) == 0)
        return -EAGAIN;

    *data = ctx->base->DATA;
    return (int)sizeof(uint8_t);
}

static int transmit_data(struct spi *ctx, const uint8_t *data)
{
    if ((ctx->base->INTFLAGS & INTFLAGS_DREIF) == 0)
        return -EAGAIN;

    ctx->base->DATA = *data;
    return (int)sizeof(uint8_t);
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    size_t recv = 0;
    uint8_t *rx8 = rx;
    const uint8_t *tx8 = tx;

    while (recv != n) {

        int res;
        bool xfer_occurred = false;
        size_t tx_n = recv + ctx->balance;

        if (tx_n < n) {
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

#include "spi-avr_irqdriven.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

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

#define SCK_FOSC_4   0
#define SCK_FOSC_16  1
#define SCK_FOSC_64  2
#define SCK_FOSC_128 3
#define SCK_FOSC_2   4
#define SCK_FOSC_8   5
#define SCK_FOSC_32  6

static void spi_isr(void *priv)
{
    struct spi *ctx = (struct spi*)priv;

    picoRTOS_assert(ctx->rx_buf != NULL, return );
    picoRTOS_assert(ctx->tx_buf != NULL, return );

    do {
        if (fifo_head_is_writable(&ctx->rx_fifo)) {
            ctx->rx_buf[ctx->rx_fifo.w] = ctx->base->SPDR;
            fifo_head_push(&ctx->rx_fifo);
        }

        /* stream next char if available */
        if (fifo_head_is_readable(&ctx->tx_fifo)) {
            fifo_head_pop(&ctx->tx_fifo);
            ctx->base->SPDR = ctx->tx_buf[ctx->tx_fifo.r];
        }else
            ctx->state = SPI_AVR_STATE_START;
        /* try to send as many bytes as possible to avoid
         * systematic context switching */
    } while ((ctx->base->SPSR & SPSR_SPIF) != 0);
}

int spi_avr_init(struct spi *ctx, int base, clock_id_t clkid)
{
    /* internals */
    ctx->base = (struct SPI_AVR*)base;
    ctx->clkid = clkid;
    ctx->state = SPI_AVR_STATE_START;
    ctx->mode = SPI_AVR_MODE_NORMAL;

    /* enable */
    ctx->base->SPCR |= SPCR_SPE;

    return 0;
}

/* Function: spi_avr_setup_irqdriven
 * Configured SPI into IRQ-driven mode
 *
 * Parameters:
 *  ctx - The SPI to setop
 *  settings - The IRQ settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int spi_avr_setup_irqdriven(struct spi *ctx, struct spi_avr_irqdriven_settings *settings)
{
    picoRTOS_assert(settings->irq <= (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return -EINVAL);
    picoRTOS_assert((settings->mask & (settings->mask + 1)) == 0, return -EINVAL);

    fifo_head_init(&ctx->rx_fifo, settings->mask);
    fifo_head_init(&ctx->tx_fifo, settings->mask);
    ctx->rx_buf = settings->rx_buf;
    ctx->tx_buf = settings->tx_buf;

    /* register interrupts */
    picoRTOS_register_interrupt(settings->irq, spi_isr, ctx);

    ctx->base->SPCR |= SPCR_SPIE;
    ctx->mode = SPI_AVR_MODE_IRQDRIVEN;

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

    picoRTOS_break();
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

static int normal_run_state_start(struct spi *ctx, const void *tx)
{
    ctx->base->SPDR = *(uint8_t*)tx;
    ctx->state = SPI_AVR_STATE_XFER;
    return -EAGAIN;
}

static int normal_run_state_xfer(struct spi *ctx, void *rx,
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

static int spi_xfer_normal(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case SPI_AVR_STATE_START: return normal_run_state_start(ctx, tx);
    case SPI_AVR_STATE_XFER: return normal_run_state_xfer(ctx, rx, tx, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int irqdriven_run_state_start(struct spi *ctx, void *rx,
                                     const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(ctx->rx_buf != NULL, return -EIO);
    picoRTOS_assert(ctx->tx_buf != NULL, return -EIO);

    int recv = 0;
    int sent = 1;

    uint8_t *rx8 = (uint8_t*)rx;
    const uint8_t *tx8 = (const uint8_t*)tx;

    /* first: try to rx */
    while ((size_t)recv != n) {
        if (fifo_head_is_readable(&ctx->rx_fifo)) {
            fifo_head_pop(&ctx->rx_fifo);
            rx8[recv++] = ctx->rx_buf[ctx->rx_fifo.r];
        }else
            break;
    }

    if (recv != 0)
        return recv;

    /* no rx: trigger a block tx */
    while ((size_t)sent != n) {
        if (fifo_head_is_writable(&ctx->tx_fifo)) {
            ctx->tx_buf[ctx->tx_fifo.w] = tx8[sent++];
            fifo_head_push(&ctx->tx_fifo);
        }else
            break;
    }

    /* force-sent 1st byte */
    ctx->base->SPDR = *tx8;
    ctx->state = SPI_AVR_STATE_XFER;

    return -EAGAIN;
}

static int spi_xfer_irqdriven(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case SPI_AVR_STATE_START: return irqdriven_run_state_start(ctx, rx, tx, n);
    case SPI_AVR_STATE_XFER: return -EAGAIN; /* wait for xfer to finish */
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->mode) {
    case SPI_AVR_MODE_NORMAL: return spi_xfer_normal(ctx, rx, tx, n);
    case SPI_AVR_MODE_IRQDRIVEN: return spi_xfer_irqdriven(ctx, rx, tx, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

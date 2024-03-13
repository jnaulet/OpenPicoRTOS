#include "spi-atmel_sercom.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <stdint.h>

struct SPI_ATMEL_SERCOM {
    volatile uint32_t CTRLA;
    volatile uint32_t CTRLB;
    volatile uint32_t CTRLC;
    volatile uint32_t BAUD;
    uint32_t RESERVED0;
    volatile uint16_t INTENCLR;
    volatile uint16_t INTENSET;
    volatile uint16_t INTFLAG;
    volatile uint16_t STATUS;
    volatile uint32_t SYNCBUSY;
    uint16_t RESERVED1;
    volatile uint16_t LENGTH;
    volatile uint32_t ADDR;
    volatile uint32_t DATA;
    uint32_t RESERVED2;
    volatile uint8_t DBGCTRL;
};

#define CTRLA_DORD     (1 < 30)
#define CTRLA_CPOL     (1 < 29)
#define CTRLA_CPHA     (1 < 28)
#define CTRLA_FORM_M   0xfu
#define CTRLA_FORM(x)  (((x) & CTRLA_FORM_M) << 24)
#define CTRLA_DIPO_M   0x3u
#define CTRLA_DIPO(x)  (((x) & CTRLA_DIPO_M) << 20)
#define CTRLA_DOPO_M   0x3u
#define CTRLA_DOPO(x)  (((x) & CTRLA_DIPO_M) << 16)
#define CTRLA_IBON     (1 << 8)
#define CTRLA_RUNSTDBY (1 << 7)
#define CTRLA_MODE_M   0x7u
#define CTRLA_MODE(x)  (((x) & CTRLA_MODE_M) << 2)
#define CTRLA_ENABLE   (1 << 1)
#define CTRLA_SWRST    (1 << 0)

#define CTRLB_RXEN      (1 << 17)
#define CTRLB_AMODE_M   0x3u
#define CTRLB_AMODE(x)  (((x) & CTRLB_AMODE_M) << 14)
#define CTRLB_MSSEN     (1 << 13)
#define CTRLB_SSDE      (1 << 9)
#define CTRLB_PLOADEN   (1 << 6)
#define CTRLB_CHSIZE_M  0x7u
#define CTRLB_CHSIZE(x) ((x) & CTRLB_CHSIZE_M)

#define CTRLC_DATA32B    (1 << 24)
#define CTRLC_ICSPACE_M  0x3fu
#define CTRLC_ICSPACE(x) ((x) & CTRLC_ICSPACE_M)

#define BAUD_BAUD_M  0xffu
#define BAUD_BAUD(x) ((x) & BAUD_BAUD_M)

#define INTFLAG_ERROR (1 << 7)
#define INTFLAG_SSL   (1 << 3)
#define INTFLAG_RXC   (1 << 2)
#define INTFLAG_TXC   (1 << 1)
#define INTFLAG_DRE   (1 << 0)

#define SYNCBUSY_LENGTH (1 << 4)
#define SYNCBUSY_CTRLB  (1 << 2)
#define SYNCBUSY_ENABLE (1 << 1)
#define SYNCBUSY_SWRST  (1 << 0)

#define LENGTH_LENEN  (1 << 8)
#define LENGTH_LEN_M  0xffu
#define LENGTH_LEN(x) ((x) & LENGTH_LEN_M)

#define ADDR_ADDRMASK_M  0xffu
#define ADDR_ADDRMASK(x) (((x) & ADDR_ADDRMASK_M) << 16)
#define ADDR_ADDR_M      0xffu
#define ADDR_ADDR(x)     ((x) & ADDR_ADDR_M)

#define DBGCTRL_DBGSTOP (1 << 0)

static int sync_busywait(struct spi *ctx, uint32_t mask)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((ctx->base->SYNCBUSY & mask) != 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

/* Function: spi_atmel_sercom_init
 * Initializes a SERCOM SPI
 *
 * Parameters:
 *  ctx - The SPI to init
 *  base - The SERCOM base address
 *  clkid - The clock id for this SPI
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int spi_atmel_sercom_init(struct spi *ctx, int base, clock_id_t clkid)
{
    int res;

    ctx->base = (struct SPI_ATMEL_SERCOM*)base;
    ctx->clkid = clkid;
    ctx->balance = 0;
    ctx->frame_size = (size_t)8;
    ctx->frame_width = (size_t)1;
    /* dma opt */
    ctx->state = SPI_ATMEL_SERCOM_STATE_DMA_START;
    ctx->fill = NULL;
    ctx->drain = NULL;
    ctx->threshold = (size_t)0;

    ctx->base->CTRLB |= (CTRLB_RXEN | CTRLB_MSSEN | CTRLB_PLOADEN);
    if ((res = sync_busywait(ctx, (uint32_t)SYNCBUSY_CTRLB)) < 0)
        return res;

    return 0;
}

/* Function: spi_atmel_sercom_setup_dma
 * Configures a SERCOM SPI for DMA use
 *
 * Parameters:
 *  ctx - The SPI to init
 *  settings - The settings to apply
 *
 * Returns:
 * Always 0
 */
int spi_atmel_sercom_setup_dma(struct spi *ctx, struct spi_atmel_sercom_dma_settings *settings)
{
    ctx->fill = settings->fill;
    ctx->drain = settings->drain;
    ctx->threshold = settings->threshold;

    return 0;
}

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    unsigned long baud;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return (int)freq);

    /* According to datasheet, BAUD = fref / (2 * fBAUD) - 1 */
    baud = (unsigned long)freq / (2ul * bitrate) - 1ul;
    picoRTOS_assert(baud < 0x100ul, return -EINVAL);

    ctx->base->BAUD = (uint32_t)BAUD_BAUD(baud);
    return 0;
}

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    picoRTOS_assert(mode != SPI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < SPI_MODE_COUNT, return -EINVAL);

    ctx->base->CTRLA &= ~CTRLA_MODE(CTRLA_MODE_M);

    if (mode == SPI_MODE_MASTER) ctx->base->CTRLA |= CTRLA_MODE(0x3);
    else ctx->base->CTRLA |= CTRLA_MODE(0x2);

    return 0;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT, return -EINVAL);

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->CTRLA &= ~CTRLA_CPOL;
        ctx->base->CTRLA &= ~CTRLA_CPHA;
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->CTRLA &= ~CTRLA_CPOL;
        ctx->base->CTRLA |= CTRLA_CPHA;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->CTRLA |= CTRLA_CPOL;
        ctx->base->CTRLA &= ~CTRLA_CPHA;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->CTRLA |= CTRLA_CPOL;
        ctx->base->CTRLA |= CTRLA_CPHA;
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

    picoRTOS_assert(frame_size >= (size_t)SPI_ATMEL_SERCOM_FRAME_SIZE_MIN, return -EINVAL);
    picoRTOS_assert(frame_size <= (size_t)SPI_ATMEL_SERCOM_FRAME_SIZE_MAX, return -EINVAL);

    if (frame_size <= (size_t)8)
        ctx->base->CTRLC &= ~CTRLC_DATA32B;
    else{
        size_t len = ((frame_size - 1) >> 3) + 1;
        ctx->base->LENGTH = (uint16_t)(LENGTH_LENEN | LENGTH_LEN(len));
        ctx->base->CTRLC |= CTRLC_DATA32B;
    }

    ctx->frame_size = frame_size;
    ctx->frame_width = div_ceil(frame_size, (size_t)8);

    return sync_busywait(ctx, (uint32_t)SYNCBUSY_LENGTH);
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res;

    ctx->base->CTRLA &= ~CTRLA_ENABLE;

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

    /* ignore cs_pol */
    /* ignore cs */

    ctx->base->CTRLA |= CTRLA_ENABLE;
    return sync_busywait(ctx, (uint32_t)SYNCBUSY_ENABLE);
}

static int write_data(struct spi *ctx, const uint8_t *data)
{
    if ((ctx->base->INTFLAG & INTFLAG_DRE) == 0)
        return -EAGAIN;

    switch (ctx->frame_width) {
    case sizeof(uint8_t):
        ctx->base->DATA = (uint32_t)*data;
        return (int)sizeof(uint8_t);

    /* byte order: 3 - 2 - 1 - 0 */
    case sizeof(uint16_t):
        /*@i@*/ ctx->base->DATA = (uint32_t)__builtin_bswap16(*(uint16_t*)data);
        return (int)sizeof(uint16_t);

    default:
        break;
    }

    /*@i@*/ ctx->base->DATA = __builtin_bswap32(*(uint32_t*)data);
    return (int)sizeof(uint32_t);
}

static int read_data(struct spi *ctx, uint8_t *data)
{
    if ((ctx->base->INTFLAG & INTFLAG_RXC) == 0)
        return -EAGAIN;

    switch (ctx->frame_width) {
    case sizeof(uint8_t):
        *data = (uint8_t)ctx->base->DATA;
        return (int)sizeof(uint8_t);

    case sizeof(uint16_t):
        /*@i@*/ *(uint16_t*)data = __builtin_bswap16((uint16_t)ctx->base->DATA);
        return (int)sizeof(uint16_t);

    default:
        break;
    }

    /*@i@*/ *(uint32_t*)data = __builtin_bswap32(ctx->base->DATA);
    return (int)sizeof(uint32_t);
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
        (intptr_t)tx,               /* source address */
        (intptr_t)&ctx->base->DATA, /* destination addresss */
        DMA_XFER_INCREMENT_ON,      /* incr_read */
        DMA_XFER_INCREMENT_OFF,     /* incr_write */
        ctx->frame_width,           /* size */
        n                           /* byte count */
    };

    /* drain xfer */
    struct dma_xfer drain = {
        (intptr_t)&ctx->base->DATA, /* source address */
        (intptr_t)rx,               /* destination address */
        DMA_XFER_INCREMENT_OFF,     /* incr_read */
        DMA_XFER_INCREMENT_ON,      /* incr_write */
        ctx->frame_width,           /* size */
        n                           /* byte count */
    };

    /* register xfers */
    if ((res = dma_setup(ctx->drain, &drain)) < 0 ||
        (res = dma_setup(ctx->fill, &fill)) < 0)
        return res;

    ctx->state = SPI_ATMEL_SERCOM_STATE_DMA_WAIT;
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
        ctx->state = SPI_ATMEL_SERCOM_STATE_DMA_START;
        return (int)n;
    }

    return -EAGAIN;
}

static int spi_xfer_dma(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case SPI_ATMEL_SERCOM_STATE_DMA_START: return spi_xfer_dma_start(ctx, rx, tx, n);
    case SPI_ATMEL_SERCOM_STATE_DMA_WAIT: return spi_xfer_dma_wait(ctx, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int spi_xfer_nodma(struct spi *ctx, void *rx, const void *tx, size_t n)
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

        if (tx_n < n) {
            /* fill up TX FIFO */
            if ((res = write_data(ctx, &tx8[tx_n])) > 0) {
                ctx->balance += res;
                xfer_occurred = true;
            }
        }

        if ((res = read_data(ctx, &rx8[recv])) > 0) {
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

#include "spi-stm32h7xx.h"
#include "picoRTOS.h"

#include <stdint.h>

struct SPI_STM32H7XX {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CFG1;
    volatile uint32_t CFG2;
    volatile uint32_t IER;
    volatile uint32_t SR;
    volatile uint32_t IFCR;
    uint32_t RESERVED0;
    union {
        volatile uint32_t U32;
        volatile uint16_t U16;
        volatile uint8_t U8;
    } TXDR;
    uint32_t RESERVED1[3];
    union {
        volatile uint32_t U32;
        volatile uint16_t U16;
        volatile uint8_t U8;
    } RXDR;
    uint32_t RESERVED2[3];
    volatile uint32_t CRCPOLY;
    volatile uint32_t TXCRC;
    volatile uint32_t RXCRC;
    volatile uint32_t UDRDR;
    volatile uint32_t I2SCFGR;
};

#define CR1_IOLOCK (1 << 16)
#define CR1_TCRCINI (1 << 15)
#define CR1_RCRCINIT (1 << 14)
#define CR1_CRC33_17 (1 << 13)
#define CR1_SSI (1 << 12)
#define CR1_HDDIR (1 << 11)
#define CR1_CSUSP (1 << 10)
#define CR1_CSTART (1 << 9)
#define CR1_MASRX (1 << 8)
#define CR1_SPE (1 << 0)

#define CR2_TSER_M 0xffffu
#define CR2_TSER(x) (((x) & CR2_TSER_M) << 16)
#define CR2_TSIZE_M 0xffffu
#define CR2_TSIZE(x) ((x) & CR2_TSIZE_M)

#define CFG1_MBR_M 0x7u
#define CFG1_MBR(x) (((x) & CFG1_MBR_M) << 28)
#define CFG1_CRC (1 << 22)
#define CFG1_CRCSIZE_M 0x1fu
#define CFG1_CRCSIZE(x) (((x) & CFG1_CRCSIZE_M) << 16)
#define CFG1_TXDMAEN (1 << 15)
#define CFG1_RXDMAEN (1 << 14)
#define CFG1_UDRDET_M 0x3u
#define CFG1_UDRDET(x) (((x) & CFG1_UDRDET_M) << 11)
#define CFG1_UDRCFG_M 0x3u
#define CFG1_UDRCFG(x) (((x) & CFG1_UDRCFG_M) << 9)
#define CFG1_FTHLV_M 0xfu
#define CFG1_FTHLV(x) (((x) & CFG1_FTHLV_M) << 5)
#define CFG1_DSIZE_M 0x1fu
#define CFG1_DSIZE(x) ((x) & CFG1_DSIZE_M)

#define CFG2_SSOM (1 << 30)
#define CFG2_SSOE (1 << 29)
#define CFG2_SSIOP (1 << 28)
#define CFG2_SSM (1 << 26)
#define CFG2_CPOL (1 << 25)
#define CFG2_CPHA (1 << 24)
#define CFG2_LSBFRST (1 << 23)
#define CFG2_MASTER (1 << 22)
#define CFG2_SP_M 0x7u
#define CFG2_SP(x) (((x) & CFG2_SP_M) << 19)
#define CFG2_COMM_M 0x3u
#define CFG2_COMM(x) (((x) & CFG2_COMM_M) << 17)
#define CFG2_IOSWP (1 << 15)
#define CFG2_MIDI_M 0xfu
#define CFG2_MIDI(x) (((x) & CFG2_MIDI_M) << 4)
#define CFG2_MSSI_M 0xfu
#define CFG2_MSSI(x) ((x) & CFG2_MSSI_M)

#define SR_CTSIZE_M 0xffffu
#define SR_CTSIZE(x) (((x) & SR_CTSIZE_M) << 16)
#define SR_RXWNE (1 << 15)
#define SR_RXPLVL_M 0x3u
#define SR_RXPLVL(x) (((x) & SR_RXPLVL_M) << 13)
#define SR_TXC (1 << 12)
#define SR_SUSP (1 << 11)
#define SR_TSERF (1 << 10)
#define SR_MODF (1 << 9)
#define SR_TIFRE (1 << 8)
#define SR_CRCE (1 << 7)
#define SR_OVR (1 << 6)
#define SR_UDR (1 << 5)
#define SR_TXTF (1 << 4)
#define SR_EOT (1 << 3)
#define SR_DXP (1 << 2)
#define SR_TXP (1 << 1)
#define SR_RXP (1 << 0)

static int get_fifo_depth(struct spi *ctx)
{
    int depth = 0;
    int timeout = CONFIG_DEADLOCK_COUNT;

    /* turn on */
    ctx->base->CR1 |= CR1_SPE;

    /* fill-in fifo ram */
    while ((ctx->base->SR & SR_TXP) != 0 && timeout-- != 0) {
        ctx->base->TXDR.U8 = 0;
        depth++;
    }

    if (!picoRTOS_assert(timeout != -1))
        return -EBUSY;

    /* turn off */
    ctx->base->CR1 &= ~CR1_SPE;
    return depth / 2;
}

/* Function: spi_stm32h7xx_init
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
int spi_stm32h7xx_init(struct spi *ctx, int base, clock_id_t clkid)
{
    int res;

    ctx->base = (struct SPI_STM32H7XX*)base;
    ctx->clkid = clkid;
    ctx->frame_size = (size_t)8;
    ctx->frame_width = (size_t)1;
    ctx->balance = 0;
    ctx->state = SPI_STM32H7XX_STATE_IDLE;
    ctx->fifo_depth = (size_t)4;
    ctx->fill = NULL;
    ctx->drain = NULL;
    ctx->threshold = 0;

    if ((res = get_fifo_depth(ctx)) < 0)
        return res;

    ctx->fifo_depth = (size_t)res;
    return 0;
}

/* Function: spi_stm32h7xx_setup
 * Configures a spi port
 *
 * Parameters:
 *  ctx - The SPI instance
 *  settings - The SPI settings (mostly DMA)
 *
 * Returns:
 * Always 0
 */
int spi_stm32h7xx_setup(struct spi *ctx, struct spi_stm32h7xx_settings *settings)
{
    ctx->fill = settings->fill;
    ctx->drain = settings->drain;
    ctx->threshold = settings->threshold;

    return 0;
}

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    unsigned long mbr = 1ul;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return (int)freq;

    /* try to find the closest value */
    while (mbr <= 8ul) {
        if ((unsigned long)freq / (1ul << mbr) <= bitrate)
            break;

        mbr++;
    }

    ctx->base->CFG1 &= ~CFG1_MBR(CFG1_MBR_M);
    ctx->base->CFG1 |= CFG1_MBR(mbr - 1ul);
    return 0;
}

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    if (!picoRTOS_assert(mode != SPI_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(mode < SPI_MODE_COUNT)) return -EINVAL;

    if (mode == SPI_MODE_MASTER) ctx->base->CFG2 |= CFG2_MASTER;
    else ctx->base->CFG2 &= ~CFG2_MASTER;

    return 0;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    if (!picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT)) return -EINVAL;

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->CFG2 &= ~CFG2_CPOL;
        ctx->base->CFG2 &= ~CFG2_CPHA;
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->CFG2 &= ~CFG2_CPOL;
        ctx->base->CFG2 |= CFG2_CPHA;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->CFG2 |= CFG2_CPOL;
        ctx->base->CFG2 &= ~CFG2_CPHA;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->CFG2 |= CFG2_CPOL;
        ctx->base->CFG2 |= CFG2_CPHA;
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

    if (!picoRTOS_assert(frame_size >= (size_t)SPI_STM32H7XX_DSIZE_MIN)) return -EINVAL;
    if (!picoRTOS_assert(frame_size <= (size_t)SPI_STM32H7XX_DSIZE_MAX)) return -EINVAL;

    ctx->base->CFG1 &= ~CFG1_DSIZE(CFG1_DSIZE_M);
    ctx->base->CFG1 |= CFG1_DSIZE(frame_size - 1);

    ctx->frame_size = frame_size;
    ctx->frame_width = div_ceil(frame_size, (size_t)8);
    return 0;
}

static int set_cs_pol(struct spi *ctx, spi_cs_pol_t cs_pol)
{
    if (!picoRTOS_assert(cs_pol != SPI_CS_POL_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(cs_pol < SPI_CS_POL_COUNT)) return -EINVAL;

    if (cs_pol == SPI_CS_POL_ACTIVE_LOW) ctx->base->CFG2 &= ~CFG2_SSIOP;
    else ctx->base->CFG2 |= CFG2_SSIOP;

    return 0;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res;

    /* disable */
    ctx->base->CR1 &= ~CR1_SPE;

    /* force hw cs for now */
    ctx->base->CFG2 &= ~CFG2_SSM;
    ctx->base->CFG2 |= CFG2_SSOE;

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

    if (settings->cs_pol != SPI_CS_POL_IGNORE &&
        (res = set_cs_pol(ctx, settings->cs_pol)) < 0)
        return res;

    /* enable */
    ctx->base->CR1 |= CR1_SPE;
    return 0;
}

static int spi_xfer_dma_idle(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    /* null check */
    if (!picoRTOS_assert(ctx->fill != NULL)) return -EIO;
    if (!picoRTOS_assert(ctx->drain != NULL)) return -EIO;

    int res;

    /* fill xfer */
    struct dma_xfer fill = {
        (intptr_t)tx,                   /* source address */
        (intptr_t)&ctx->base->TXDR.U32, /* destination addresss */
        true,                           /* incr_read */
        false,                          /* incr_write */
        ctx->frame_width,               /* size */
        n                               /* byte count */
    };

    /* drain xfer */
    struct dma_xfer drain = {
        (intptr_t)&ctx->base->RXDR.U32, /* source addresss */
        (intptr_t)rx,                   /* destination address */
        false,                          /* incr_read */
        true,                           /* incr_write */
        ctx->frame_width,               /* size */
        n                               /* byte count */
    };

    /* prepare xfers */
    ctx->base->CR1 &= ~CR1_SPE;

    /* register xfers */
    if ((res = dma_setup(ctx->fill, &fill)) < 0 ||
        (res = dma_setup(ctx->drain, &drain)) < 0)
        return res;

    ctx->base->CFG1 |= (CFG1_TXDMAEN | CFG1_RXDMAEN);
    ctx->base->CFG1 &= ~CFG1_FTHLV(CFG1_FTHLV_M);
    ctx->base->CFG1 |= CFG1_FTHLV(ctx->frame_width);
    ctx->base->CR2 = (uint32_t)CR2_TSIZE(n / ctx->frame_width);

    ctx->base->CR1 |= CR1_SPE;
    ctx->base->CR1 |= CR1_CSTART;

    ctx->state = SPI_STM32H7XX_STATE_XFER;
    return -EAGAIN;
}

static int spi_xfer_dma_xfer(struct spi *ctx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    /* null check */
    if (!picoRTOS_assert(ctx->fill != NULL)) return -EIO;
    if (!picoRTOS_assert(ctx->drain != NULL)) return -EIO;

    if (dma_xfer_done(ctx->fill) == 0 &&
        dma_xfer_done(ctx->drain) == 0) {
        /* end xfers */
        ctx->base->CFG1 &= ~(CFG1_TXDMAEN | CFG1_RXDMAEN);
        ctx->base->IFCR = (uint32_t)(SR_TXTF | SR_EOT);
        /* back to normal */
        ctx->state = SPI_STM32H7XX_STATE_IDLE;
        return (int)n;
    }

    return -EAGAIN;
}

static int spi_xfer_dma(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case SPI_STM32H7XX_STATE_IDLE: return spi_xfer_dma_idle(ctx, rx, tx, n);
    case SPI_STM32H7XX_STATE_XFER: return spi_xfer_dma_xfer(ctx, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int spi_xfer_nodma_idle(struct spi *ctx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert((n & (ctx->frame_width - 1)) == 0)) return -EINVAL;

    /* strongly suggested */
    ctx->base->CR1 &= ~CR1_SPE;
    ctx->base->CR2 = (uint32_t)CR2_TSIZE(n / ctx->frame_width);
    /* threshold */
    ctx->base->CFG1 &= ~CFG1_FTHLV(CFG1_FTHLV_M);
    // ctx->base->CFG1 |= CFG1_FTHLV(width - 1);
    /* go */
    ctx->base->CR1 |= CR1_SPE;
    ctx->base->CR1 |= CR1_CSTART;

    ctx->state = SPI_STM32H7XX_STATE_XFER;
    return -EAGAIN;
}

static int write_data(struct spi *ctx, const uint8_t *data)
{
    if ((ctx->base->SR & SR_TXP) == 0 ||
        (ctx->base->SR & SR_EOT) != 0)
        return -EAGAIN;

    switch (ctx->frame_width) {
    case sizeof(uint8_t):
        ctx->base->TXDR.U8 = *data;
        return (int)sizeof(uint8_t);

    case sizeof(uint16_t):
        ctx->base->TXDR.U16 = *(uint16_t*)data;
        return (int)sizeof(uint16_t);

    default:
        break;
    }

    ctx->base->TXDR.U32 = *(uint32_t*)data;
    return (int)sizeof(uint32_t);
}

static int read_data(struct spi *ctx, uint8_t *data)
{
    if ((ctx->base->SR & SR_RXP) == 0)
        return -EAGAIN;

    switch (ctx->frame_width) {
    case sizeof(uint8_t):
        *data = ctx->base->RXDR.U8;
        return (int)sizeof(uint8_t);

    case sizeof(uint16_t):
        *(uint16_t*)data = ctx->base->RXDR.U16;
        return (int)sizeof(uint16_t);

    default:
        break;
    }

    *(uint32_t*)data = ctx->base->RXDR.U32;
    return (int)sizeof(uint32_t);
}

static int spi_xfer_nodma_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
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
        size_t fifo_limit = ctx->fifo_depth - ctx->frame_width;

        if (tx_n < n && ctx->balance < (int)fifo_limit) {
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

    if (recv == n) {
        /* back to idle */
        ctx->base->IFCR = (uint32_t)(SR_TXTF | SR_EOT);
        ctx->state = SPI_STM32H7XX_STATE_IDLE;
    }

    return (int)recv;
}

static int spi_xfer_nodma(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    switch (ctx->state) {
    case SPI_STM32H7XX_STATE_IDLE: return spi_xfer_nodma_idle(ctx, n);
    case SPI_STM32H7XX_STATE_XFER: return spi_xfer_nodma_xfer(ctx, rx, tx, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
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

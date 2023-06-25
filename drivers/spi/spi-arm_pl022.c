#include "spi-arm_pl022.h"
#include "picoRTOS.h"

#include <stdint.h>

struct SPI_ARM_PL022 {
    volatile uint32_t SSPCR0;
    volatile uint32_t SSPCR1;
    volatile uint32_t SSPDR;
    volatile uint32_t SSPSR;
    volatile uint32_t SSPCPSR;
    volatile uint32_t SSPIMISC;
    volatile uint32_t SSPRIS;
    volatile uint32_t SSPMIS;
    volatile uint32_t SSPICR;
    volatile uint32_t SSPDMACR;
    uint32_t RESERVED0[1007];
    volatile uint32_t SSPPERIPHID0;
    volatile uint32_t SSPPERIPHID1;
    volatile uint32_t SSPPERIPHID2;
    volatile uint32_t SSPPERIPHID3;
    volatile uint32_t SSPPCELLID0;
    volatile uint32_t SSPPCELLID1;
    volatile uint32_t SSPPCELLID2;
    volatile uint32_t SSPPCELLID3;
};

#define SSPCR0_SCR_M  0xffu
#define SSPCR0_SCR(x) (((x) & SSPCR0_SCR_M) << 8)
#define SSPCR0_SPH    (1 << 7)
#define SSPCR0_SPO    (1 << 6)
#define SSPCR0_FRF_M  0x3u
#define SSPCR0_FRF(x) (((x) & SSPCR0_FRF_M) << 4)
#define SSPCR0_DSS_M  0xfu
#define SSPCR0_DSS(x) ((x) & SSPCR0_DSS_M)

#define SSPCR1_MS  (1 << 2)
#define SSPCR1_SSE (1 << 1)
#define SSPCR1_LBM (1 << 0)

#define SSPDR_DATA_M  0xffffu
#define SSPDR_DATA(x) ((x) & SSPDR_DATA_M)

#define SSPSR_BSY (1 << 4)
#define SSPSR_RFF (1 << 3)
#define SSPSR_RNE (1 << 2)
#define SSPSR_TNF (1 << 1)
#define SSPSR_TFE (1 << 0)

#define SSPCPSR_CPSDVSR_M  0xffu
#define SSPCPSR_CPSDVSR(x) ((x) & SSPCPSR_CPSDVSR_M)

#define SSPDMACR_TXDMAE (1 << 1)
#define SSPDMACR_RXDMAE (1 << 0)

/* Function: spi_arm_pl022_init
 * Initializes a SPI
 *
 * Parameters:
 *  ctx - The SPI to init
 *  base - The SPI base address
 *  clkid - The clock id used by the SPI
 *
 * Returns:
 * Always 0
 */
int spi_arm_pl022_init(struct spi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct SPI_ARM_PL022*)base;
    ctx->clkid = clkid;
    ctx->balance = 0;
    ctx->frame_size = 0;

    /* Motorola frame format */
    ctx->base->SSPCR0 &= ~SSPCR0_FRF(SSPCR0_FRF_M);

    return 0;
}

static int set_loopback(struct spi *ctx, bool loopback)
{
    /* disable SSP */
    ctx->base->SSPCR1 &= ~SSPCR1_SSE;

    if (loopback) ctx->base->SSPCR1 |= SSPCR1_LBM;
    else ctx->base->SSPCR1 &= ~SSPCR1_LBM;

    /* enable SSP */
    ctx->base->SSPCR1 |= SSPCR1_SSE;

    return 0;
}

/* Function: spi_arm_pl022_setup
 * Configures a ARM PL022 SPI
 *
 * Parameters:
 *  ctx - The SPI to init
 *  settings - The settings to apply
 *
 * Returns:
 * Always 0
 */
int spi_arm_pl022_setup(struct spi *ctx, struct spi_arm_pl022_settings *settings)
{
    ctx->fill = settings->fill;
    ctx->drain = settings->drain;
    ctx->threshold = settings->threshold;

    return set_loopback(ctx, settings->loopback);
}

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    /* F SSPCLK CPSDVSR * (1 + SCR) */

    unsigned long dvsr;
    unsigned long divider;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return (int)freq;

    /* This is the theoretical divider we are looking for, but...
     * As the dvsr is a pair number, there's no way we can achieve any odd
     * divider, so 1Mbit from 125Mhz clk_peri is out of the question
     * We will try to get as close as possible, as a workaround
     */
    divider = (unsigned long)freq / bitrate;

    /* freq / i * (1 + j) */
    for (dvsr = 2ul; dvsr < (unsigned long)SSPCPSR_CPSDVSR_M; dvsr += 2ul) {
        unsigned long scr;
        for (scr = 0; scr <= (unsigned long)SSPCR0_SCR_M; scr++) {
            unsigned long value = dvsr * (1ul + scr);
            /* The workaround is here */
            if (value > (divider - 2ul) &&
                value < (divider + 2ul)) {
                ctx->base->SSPCPSR = (uint32_t)SSPCPSR_CPSDVSR(dvsr);
                ctx->base->SSPCR0 &= ~SSPCR0_SCR(SSPCR0_SCR_M);
                ctx->base->SSPCR0 |= SSPCR0_SCR(scr);
                return 0;
            }
        }
    }

    /* signal error */
    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    if (!picoRTOS_assert(mode != SPI_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(mode < SPI_MODE_COUNT)) return -EINVAL;

    if (mode == SPI_MODE_MASTER) ctx->base->SSPCR1 &= ~SSPCR1_MS;
    else ctx->base->SSPCR1 |= SSPCR1_MS;

    return 0;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    if (!picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT)) return -EINVAL;

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->SSPCR0 &= ~SSPCR0_SPO;
        ctx->base->SSPCR0 &= ~SSPCR0_SPH;
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->SSPCR0 &= ~SSPCR0_SPO;
        ctx->base->SSPCR0 |= SSPCR0_SPH;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->SSPCR0 |= SSPCR0_SPO;
        ctx->base->SSPCR0 &= ~SSPCR0_SPH;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->SSPCR0 |= SSPCR0_SPO;
        ctx->base->SSPCR0 |= SSPCR0_SPH;
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
    if (!picoRTOS_assert(frame_size >= (size_t)SPI_ARM_PL022_FRAME_SIZE_MIN)) return -EINVAL;
    if (!picoRTOS_assert(frame_size <= (size_t)SPI_ARM_PL022_FRAME_SIZE_MAX)) return -EINVAL;

    ctx->base->SSPCR0 &= ~SSPCR0_DSS(SSPCR0_DSS_M);
    ctx->base->SSPCR0 |= SSPCR0_DSS(frame_size - 1);

    ctx->frame_size = frame_size;

    return 0;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res;

    /* disable SSP */
    ctx->base->SSPCR1 &= ~SSPCR1_SSE;

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

    /* enable SSP */
    ctx->base->SSPCR1 |= SSPCR1_SSE;

    return 0;
}

static int write_data(struct spi *ctx, const uint8_t *data)
{
    if ((ctx->base->SSPSR & SSPSR_TNF) == 0)
        return -EAGAIN;

    if (ctx->frame_size <= (size_t)8) {
        ctx->base->SSPDR = (uint32_t)*data;
        return (int)sizeof(uint8_t);
    }

    ctx->base->SSPDR = (uint32_t)*(uint16_t*)data;
    return (int)sizeof(uint16_t);
}

static int read_data(struct spi *ctx, uint8_t *data)
{
    if ((ctx->base->SSPSR & SSPSR_RNE) == 0)
        return -EAGAIN;

    if (ctx->frame_size <= (size_t)8) {
        *data = (uint8_t)ctx->base->SSPDR;
        return (int)sizeof(uint8_t);
    }

    *(uint16_t*)data = (uint16_t)ctx->base->SSPDR;
    return (int)sizeof(uint16_t);
}

static int spi_xfer_dma_start(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    /* null check */
    if (!picoRTOS_assert(ctx->fill != NULL)) return -EIO;
    if (!picoRTOS_assert(ctx->drain != NULL)) return -EIO;

    int res;

    /* fill xfer */
    struct dma_xfer fill = {
        (intptr_t)tx,                   /* source address */
        (intptr_t)&ctx->base->SSPDR,    /* destination addresss */
        true,                           /* incr_read */
        false,                          /* incr_write */
        ctx->frame_size / (size_t)8,    /* size */
        n                               /* byte count */
    };

    /* drain xfer */
    struct dma_xfer drain = {
        (intptr_t)&ctx->base->SSPDR,    /* source addresss */
        (intptr_t)rx,                   /* destination address */
        false,                          /* incr_read */
        true,                           /* incr_write */
        ctx->frame_size / (size_t)8,    /* size */
        n                               /* byte count */
    };

    /* register xfers */
    if ((res = dma_setup(ctx->fill, &fill)) < 0 ||
        (res = dma_setup(ctx->drain, &drain)) < 0)
        return res;

    /* trigger xfers */
    ctx->base->SSPDMACR |= (SSPDMACR_TXDMAE | SSPDMACR_RXDMAE);

    ctx->state = SPI_ARM_PL022_STATE_DMA_WAIT;
    return -EAGAIN;
}

static int spi_xfer_dma_wait(struct spi *ctx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    /* null check */
    if (!picoRTOS_assert(ctx->fill != NULL)) return -EIO;
    if (!picoRTOS_assert(ctx->drain != NULL)) return -EIO;

    if (dma_xfer_done(ctx->fill) == 0 &&
        dma_xfer_done(ctx->drain) == 0) {
        /* end xfers */
        ctx->base->SSPDMACR &= ~(SSPDMACR_TXDMAE | SSPDMACR_RXDMAE);
        /* back to normal */
        ctx->state = SPI_ARM_PL022_STATE_DMA_START;
        return (int)n;
    }

    return -EAGAIN;
}

static int spi_xfer_dma(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case SPI_ARM_PL022_STATE_DMA_START: return spi_xfer_dma_start(ctx, rx, tx, n);
    case SPI_ARM_PL022_STATE_DMA_WAIT: return spi_xfer_dma_wait(ctx, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int spi_xfer_nodma(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* 16bit check */
    if (ctx->frame_size > (size_t)8)
        if (!picoRTOS_assert((n & 0x1) == 0)) return -EINVAL;

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
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* DMA */
    if (ctx->fill != NULL && ctx->drain != NULL &&
        n >= ctx->threshold)
        return spi_xfer_dma(ctx, rx, tx, n);

    /* NO DMA */
    return spi_xfer_nodma(ctx, rx, tx, n);
}

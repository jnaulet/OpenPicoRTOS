#include "spi-pic32mx.h"
#include "picoRTOS.h"

#include <stdint.h>
#include "misc-pic32mx.h"

struct SPI_PIC32MX {
    struct PIC32MX_CLR_SET_INV SPIxCON;
    struct PIC32MX_CLR_SET_INV SPIxSTAT;
    struct PIC32MX_CLR_SET_INV SPIxBUF;
    struct PIC32MX_CLR_SET_INV SPIxBRG;
    struct PIC32MX_CLR_SET_INV SPIxCON2;
};

#define SPIxCON_FRMEN      (1 << 31)
#define SPIxCON_FRMSYNC    (1 << 30)
#define SPIxCON_FRMPOL     (1 << 29)
#define SPIxCON_MSSEN      (1 << 28)
#define SPIxCON_FRMSYPW    (1 << 27)
#define SPIxCON_FRMCNT_M   0x7u
#define SPIxCON_FRMCNT(x)  (((x) & SPIxCON_FRMCNT_M) << 24)
#define SPIxCON_MCLKSEL    (1 << 23)
#define SPIxCON_SPIFE      (1 << 17)
#define SPIxCON_ENHBUF     (1 << 16)
#define SPIxCON_ON         (1 << 15)
#define SPIxCON_SIDL       (1 << 13)
#define SPIxCON_DISSDO     (1 << 12)
#define SPIxCON_MODE32     (1 << 11)
#define SPIxCON_MODE16     (1 << 10)
#define SPIxCON_SMP        (1 << 9)
#define SPIxCON_CKE        (1 << 8)
#define SPIxCON_SSEN       (1 << 7)
#define SPIxCON_CKP        (1 << 6)
#define SPIxCON_MSTEN      (1 << 5)
#define SPIxCON_DISSDI     (1 << 4)
#define SPIxCON_STXISEL_M  0x3u
#define SPIxCON_STXISEL(x) (((x) & SPIxCON_STXISEL_M) << 2)
#define SPIxCON_SRXISEL_M  0x3u
#define SPIxCON_SRXISEL(x) ((x) & SPIxCON_SRXISEL_M)

#define SPIxSTAT_RXBUFELM_M  0x1fu
#define SPIxSTAT_RXBUFELM(x) (((x) & SPIxSTAT_RXBUFELM_M) << 24)
#define SPIxSTAT_TXBUFELM_M  0x1fu
#define SPIxSTAT_TXBUFELM(x) (((x) & SPIxSTAT_TXBUFELM_M) << 16)
#define SPIxSTAT_FRMERR      (1 << 12)
#define SPIxSTAT_SPIBUSY     (1 << 11)
#define SPIxSTAT_SPITUR      (1 << 8)
#define SPIxSTAT_SRMT        (1 << 7)
#define SPIxSTAT_SPIROV      (1 << 6)
#define SPIxSTAT_SPIRBE      (1 << 5)
#define SPIxSTAT_SPITBE      (1 << 3)
#define SPIxSTAT_SPITBF      (1 << 1)
#define SPIxSTAT_SPIRBF      (1 << 0)

#define SPIxCON2_SPISGNEXT (1 << 15)
#define SPIxCON2_FRMERREN  (1 << 12)
#define SPIxCON2_SPIROVEN  (1 << 11)
#define SPIxCON2_SPITUREN  (1 << 10)
#define SPIxCON2_IGNROV    (1 << 9)
#define SPIxCON2_IGNTUR    (1 << 8)
#define SPIxCON2_AUDEN     (1 << 7)
#define SPIxCON2_AUDMONO   (1 << 3)
#define SPIxCON2_AUDMOD_M  0x3u
#define SPIxCON2_AUDMOD(x) ((x) & SPIxCON2_AUDMOD_M)

/* Function: spi_pic32mx_init
 * Initializes a SPI
 *
 * Parameters:
 *  ctx - The SPI context to init
 *  base - The SPI module base address
 *  clkid - The SPI input clock ID
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int spi_pic32mx_init(struct spi *ctx, struct SPI_PIC32MX *base, clock_id_t clkid)
{
    ctx->base = base;
    ctx->clkid = clkid;
    ctx->frame_size = (size_t)8;
    ctx->balance = 0;

    /* turn on, enhanced mode */
    ctx->base->SPIxCON.SET = (uint32_t)(SPIxCON_ENHBUF | SPIxCON_ON);

    return 0;
}

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    unsigned long brg;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return (int)freq;

    brg = (unsigned long)freq / (bitrate * 2ul) - 1ul;
    ctx->base->SPIxBRG.REG = (uint32_t)brg;

    return 0;
}

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    if (!picoRTOS_assert(mode != SPI_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(mode < SPI_MODE_COUNT)) return -EINVAL;

    if (mode == SPI_MODE_MASTER) ctx->base->SPIxCON.SET = (uint32_t)(SPIxCON_MSSEN | SPIxCON_MSTEN);
    else ctx->base->SPIxCON.CLR = (uint32_t)(SPIxCON_MSSEN | SPIxCON_MSTEN);

    return 0;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    if (!picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT)) return -EINVAL;

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->SPIxCON.CLR = (uint32_t)(SPIxCON_CKE | SPIxCON_CKP);
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->SPIxCON.CLR = (uint32_t)SPIxCON_CKP;
        ctx->base->SPIxCON.SET = (uint32_t)SPIxCON_CKE;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->SPIxCON.SET = (uint32_t)SPIxCON_CKP;
        ctx->base->SPIxCON.CLR = (uint32_t)SPIxCON_CKE;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->SPIxCON.SET = (uint32_t)(SPIxCON_CKE | SPIxCON_CKP);
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
    ctx->base->SPIxCON.CLR = (uint32_t)(SPIxCON_MODE32 | SPIxCON_MODE16);

    switch (frame_size) {
    case 32: ctx->base->SPIxCON.SET = (uint32_t)SPIxCON_MODE32; break;
    case 16: ctx->base->SPIxCON.SET = (uint32_t)SPIxCON_MODE16; break;
    case 8: break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    ctx->frame_size = frame_size;
    return 0;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res;

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

    return 0;
}

static int write_data(struct spi *ctx, const uint8_t *data)
{
    if ((ctx->base->SPIxSTAT.REG & SPIxSTAT_SPITBF) != 0)
        return -EAGAIN;

    if (ctx->frame_size <= (size_t)8) {
        ctx->base->SPIxBUF.REG = (uint32_t)*data;
        return (int)sizeof(uint8_t);
    }

    if (ctx->frame_size <= (size_t)16) {
        ctx->base->SPIxBUF.REG = (uint32_t)*(uint16_t*)data;
        return (int)sizeof(uint16_t);
    }

    ctx->base->SPIxBUF.REG = *(uint32_t*)data;
    return (int)sizeof(uint32_t);
}

static int read_data(struct spi *ctx, uint8_t *data)
{
    if ((ctx->base->SPIxSTAT.REG & SPIxSTAT_SPIRBE) != 0)
        return -EAGAIN;

    if (ctx->frame_size <= (size_t)8) {
        *data = (uint8_t)ctx->base->SPIxBUF.REG;
        return (int)sizeof(uint8_t);
    }

    if (ctx->frame_size <= (size_t)16) {
        *data = (uint16_t)ctx->base->SPIxBUF.REG;
        return (int)sizeof(uint16_t);
    }

    *(uint32_t*)data = ctx->base->SPIxBUF.REG;
    return (int)sizeof(uint32_t);
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
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

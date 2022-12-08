#include "spi-sam.h"
#include "picoRTOS.h"

struct SPI_SAM {
    volatile uint32_t SPI_CR;
    volatile uint32_t SPI_MR;
    volatile uint32_t SPI_RDR;
    volatile uint32_t SPI_TDR;
    volatile uint32_t SPI_SR;
    volatile uint32_t SPI_IER;
    volatile uint32_t SPI_IDR;
    volatile uint32_t SPI_IMR;
    uint32_t RESERVED0[4];
    volatile uint32_t SPI_CSR[SPI_SAM_CS_COUNT];
    uint32_t RESERVED1[38];
    volatile uint32_t SPI_WPMR;
    volatile uint32_t SPI_WPSR;
};

#define SPI_CR_SWRST  (1 << 7)
#define SPI_CR_SPIDIS (1 << 1)
#define SPI_CR_SPIEN  (1 << 0)

#define SPI_MR_DLYBCS_M  0xffu
#define SPI_MR_DLYBCS(x) (((x) & SPI_MODE_DLYBCS_M) << 24)
#define SPI_MR_PCS_M     0xfu
#define SPI_MR_PCS(x)    (((x) & SPI_MR_PCS_M) << 16)
#define SPI_MR_LLB       (1 << 7)
#define SPI_MR_WDRBT     (1 << 5)
#define SPI_MR_MODFIS    (1 << 4)
#define SPI_MR_PCSDEC    (1 << 2)
#define SPI_MR_PS        (1 << 1)
#define SPI_MR_MSTR      (1 << 0)

#define SPI_RDR_PCS_M      0xfu
#define SPI_RDR_PCS_GET(x) (((x) >> 16) & SPI_RDR_PCS_M)

#define SPI_TDR_LASTXFER (1 << 24)
#define SPI_TDR_PCS_M    0xfu
#define SPI_TDR_PCS(x)   (((x) & SPI_TDR_PCS_M) << 16)

#define SPI_SR_SPIENS  (1 << 16)
#define SPI_SR_UNDES   (1 << 10)
#define SPI_SR_TXEMPTY (1 << 9)
#define SPI_SR_NSSR    (1 << 8)
#define SPI_SR_OVRES   (1 << 3)
#define SPI_SR_MODF    (1 << 2)
#define SPI_SR_TDRE    (1 << 1)
#define SPI_SR_RDRF    (1 << 0)

#define SPI_CSRn_DLYBCT_M  0xffu
#define SPI_CSRn_DLYBCT(x) (((x) & SPI_CSRn_DLYBCT_M) << 24)
#define SPI_CSRn_DLYBS_M   0xffu
#define SPI_CSRn_DLYBS(x)  (((x) & SPI_CSRn_DLYBS_M) << 16)
#define SPI_CSRn_SCBR_M    0xffu
#define SPI_CSRn_SCBR(x)   (((x) & SPI_CSRn_SCBR_M) << 8)
#define SPI_CSRn_BITS_M    0xfu
#define SPI_CSRn_BITS(x)   (((x) & SPI_CSRn_BITS_M) << 4)
#define SPI_CSRn_CSAAT     (1 << 3)
#define SPI_CSRn_CSNAAT    (1 << 2)
#define SPI_CSRn_NCPHA     (1 << 1)
#define SPI_CSRn_CPOL      (1 << 0)

/* Function: spi_sam_init
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
int spi_sam_init(struct spi *ctx, struct SPI_SAM *base, clock_id_t clkid)
{
    ctx->base = base;
    ctx->clkid = clkid;
    ctx->frame_size = 0;
    ctx->balance = 0;
    ctx->pcs = 0;

    /* software reset */
    ctx->base->SPI_CR = (uint32_t)SPI_CR_SWRST;

    /* enable */
    ctx->base->SPI_CR &= ~SPI_CR_SPIDIS;
    ctx->base->SPI_CR |= SPI_CR_SPIEN;

    /* wait data read before transfer */
    ctx->base->SPI_MR |= SPI_MR_WDRBT;

    return 0;
}

/* Function: spi_sam_set_loopback
 * Sets SPI to loopback mode for tests
 *
 * Parameters:
 *  ctx - The SPI instance
 *  loopback - true or false
 *
 * Returns:
 * Always 0
 */
int spi_sam_set_loopback(struct spi *ctx, bool loopback)
{
    if (loopback) ctx->base->SPI_MR |= SPI_MR_LLB;
    else ctx->base->SPI_MR &= ~SPI_MR_LLB;

    return 0;
}

static int set_bitrate(struct spi *ctx, size_t cs, unsigned long bitrate)
{
    if (!picoRTOS_assert(cs < (size_t)SPI_SAM_CS_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    ctx->base->SPI_CSR[cs] &= SPI_CSRn_SCBR(SPI_CSRn_SCBR_M);
    ctx->base->SPI_CSR[cs] |= SPI_CSRn_SCBR((unsigned long)freq / bitrate);

    return 0;
}

static int set_clkmode(struct spi *ctx, size_t cs, spi_clock_mode_t clkmode)
{
    if (!picoRTOS_assert(cs < (size_t)SPI_SAM_CS_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT)) return -EINVAL;

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->SPI_CSR[cs] &= ~SPI_CSRn_CPOL;
        ctx->base->SPI_CSR[cs] |= SPI_CSRn_NCPHA;
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->SPI_CSR[cs] &= ~SPI_CSRn_CPOL;
        ctx->base->SPI_CSR[cs] &= ~SPI_CSRn_NCPHA;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->SPI_CSR[cs] |= SPI_CSRn_CPOL;
        ctx->base->SPI_CSR[cs] |= SPI_CSRn_NCPHA;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->SPI_CSR[cs] |= SPI_CSRn_CPOL;
        ctx->base->SPI_CSR[cs] &= ~SPI_CSRn_NCPHA;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EINVAL;
    }

    return 0;
}

static int set_frame_size(struct spi *ctx, size_t cs, size_t frame_size)
{
    if (!picoRTOS_assert(cs < (size_t)SPI_SAM_CS_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(frame_size >= (size_t)SPI_SAM_FRAME_SIZE_MIN)) return -EINVAL;
    if (!picoRTOS_assert(frame_size <= (size_t)SPI_SAM_FRAME_SIZE_MAX)) return -EINVAL;

    size_t bits = frame_size - (size_t)SPI_SAM_FRAME_SIZE_MIN;

    ctx->base->SPI_CSR[cs] &= ~SPI_CSRn_BITS(SPI_CSRn_BITS_M);
    ctx->base->SPI_CSR[cs] |= SPI_CSRn_BITS(bits);

    ctx->frame_size = frame_size;
    return 0;
}

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    if (!picoRTOS_assert(mode != SPI_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(mode < SPI_MODE_COUNT)) return -EINVAL;

    if (mode == SPI_MODE_MASTER) ctx->base->SPI_MR |= SPI_MR_MSTR;
    else ctx->base->SPI_MR &= ~SPI_MR_MSTR;

    return 0;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    if (!picoRTOS_assert(settings->cs < (size_t)SPI_SAM_CS_COUNT)) return -EINVAL;

    int res;

    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->cs, settings->bitrate)) < 0)
        return res;

    if (settings->clkmode != SPI_CLOCK_MODE_IGNORE &&
        (res = set_clkmode(ctx, settings->cs, settings->clkmode)) < 0)
        return res;

    if (settings->frame_size != 0 &&
        (res = set_frame_size(ctx, settings->cs, settings->frame_size)) < 0)
        return res;

    if (settings->mode != SPI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    /* ignore cs_pol */

    /* cs */
    ctx->base->SPI_MR &= ~SPI_MR_PCS(SPI_MR_PCS_M);
    ctx->base->SPI_MR |= SPI_MR_PCS( ~(1 << settings->cs));

    return 0;
}

static int receive_data(struct spi *ctx, uint8_t *data)
{
    if ((ctx->base->SPI_SR & SPI_SR_RDRF) == 0)
        return -EAGAIN;

    uint32_t spi_rdr = ctx->base->SPI_RDR;

    /* ignore first shift */
    if (SPI_RDR_PCS_GET(spi_rdr) == SPI_RDR_PCS_M)
        return -EAGAIN;

    if (ctx->frame_size <= (size_t)8) {
        *data = (uint8_t)spi_rdr;
        return (int)sizeof(uint8_t);
    }

    *(uint16_t*)data = (uint16_t)spi_rdr;
    return (int)sizeof(uint16_t);
}

static int transmit_data(struct spi *ctx, const uint8_t *data)
{
    if ((ctx->base->SPI_SR & SPI_SR_TDRE) == 0)
        return -EAGAIN;

    if (ctx->frame_size <= (size_t)8) {
        ctx->base->SPI_TDR = (uint32_t)*data;
        return (int)sizeof(uint8_t);
    }

    ctx->base->SPI_TDR = (uint32_t)*(uint16_t*)data;
    return (int)sizeof(uint16_t);
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

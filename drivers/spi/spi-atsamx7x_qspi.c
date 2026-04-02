#include "spi-atsamx7x_qspi.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

struct SPI_ATSAMX7X_QSPI {
    volatile uint32_t QSPI_CR;
    volatile uint32_t QSPI_MR;
    volatile uint32_t QSPI_RDR;
    volatile uint32_t QSPI_TDR;
    volatile uint32_t QSPI_SR;
    volatile uint32_t QSPI_IER;
    volatile uint32_t QSPI_IDR;
    volatile uint32_t QSPI_IMR;
    volatile uint32_t QSPI_SCR;
    uint32_t RESERVED0[3];
    volatile uint32_t QSPI_IAR;
    volatile uint32_t QSPI_ICR;
    volatile uint32_t QSPI_IFR;
    uint32_t RESERVED1[1];
    volatile uint32_t QSPI_SMR;
    volatile uint32_t QSPI_SKR;
    uint32_t RESERVED2[39];
    volatile uint32_t QSPI_WPMR;
    volatile uint32_t QSPI_WPSR;
};

#define QSPI_CR_LASTXFER (1 << 24)
#define QSPI_CR_SWRST    (1 << 7)
#define QSPI_CR_QSPIDIS  (1 << 1)
#define QSPI_CR_QSPIEN   (1 << 0)

#define QSPI_MR_DLYCS_M   0xffu
#define QSPI_MR_DLYCS(x)  (((x) & QSPI_MR_DLYCS_M) << 24)
#define QSPI_MR_DLYBCT_M  0xffu
#define QSPI_MR_DLYBCT(x) (((x) & QSPI_MR_DLYBCT_M) << 16)
#define QSPI_MR_NBBITS_M  0xfu
#define QSPI_MR_NBBITS(x) (((x) & QSPI_MR_NBBITS_M) << 8)
#define QSPI_MR_TAMPCLR   (1 << 7)
#define QSPI_MR_CSMODE_M  0x3u
#define QSPI_MR_CSMODE(x) (((x) & QSPI_MR_CSMODE_M) << 4)
#define QSPI_MR_WDRBT     (1 << 2)
#define QSPI_MR_LLB       (1 << 1)
#define QSPI_MR_SMM       (1 << 0)

#define QSPI_SR_QSPIENS (1 << 24)
#define QSPI_SR_INSTRE  (1 << 10)
#define QSPI_SR_CSS     (1 << 9)
#define QSPI_SR_CSR     (1 << 8)
#define QSPI_SR_OVRES   (1 << 3)
#define QSPI_SR_TXEMPTY (1 << 2)
#define QSPI_SR_TDRE    (1 << 1)
#define QSPI_SR_RDRF    (1 << 0)

#define QSPI_SCR_DLYBS_M  0xffu
#define QSPI_SCR_DLYBS(x) (((x) & QSPI_SCR_DLYBS_M) << 16)
#define QSPI_SCR_SCBR_M   0xffu
#define QSPI_SCR_SCBR(x)  (((x) & QSPI_SCR_SCBR_M) << 8)
#define QSPI_SCR_CPHA     (1 << 1)
#define QSPI_SCR_CPOL     (1 << 0)

#define QSPI_ICR_OPT_M   0xffu
#define QSPI_ICR_OPT(x)  (((x) & QSPI_ICR_OPT_M) << 16)
#define QSPI_ICR_INST_M  0xffu
#define QSPI_ICR_INST(x) ((x) & QSPI_ICR_INST_M)

#define QSPI_IFR_DDRCMDEN  (1 << 26)
#define QSPI_IFR_APBTFRTYP (1 << 24)
#define QSPI_IFR_NBDUM_M   0x1fu
#define QSPI_IFR_NBDUM(x)  (((x) & QSPI_IFR_NBDUM_M) << 16)
#define QSPI_IFR_DDREN     (1 << 15)
#define QSPI_IFR_CRM       (1 << 14)
#define QSPI_IFR_TFRTYP_M  0x3u
#define QSPI_IFR_TFRTYP(x) (((x) & QSPI_IFR_TFRTYP_M) << 12)
#define QSPI_IFR_ADDRL     (1 << 10)
#define QSPI_IFR_OPTL_M    0x3u
#define QSPI_IFR_OPTL(x)   (((x) & QSPI_IFR_OPTL_M) << 8)
#define QSPI_IFR_DATAEN    (1 << 7)
#define QSPI_IFR_OPTEN     (1 << 6)
#define QSPI_IFR_ADDREN    (1 << 5)
#define QSPI_IFR_INSTEN    (1 << 4)
#define QSPI_IFR_WIDTH_M   0x7u
#define QSPI_IFR_WIDTH(x)  ((x) & QSPI_IFR_WIDTH_M)

#define QSPI_SMR_RVDIS (1 << 1)
#define QSPI_SMR_SCREN (1 << 0)

#define QSPI_WPMR_WPKEY_M  0xffffffu
#define QSPI_WPMR_WPKEY(x) (((x) & QSPI_WPMR_WPKEY_M) << 8)
#define QSPI_WPMR_WPEN     (1 << 0)

#define QSPI_WPSR_WPVSRC_M  0xffu
#define QSPI_WPSR_WPVSRC(x) (((x) & QSPI_WPSR_WPVSRC_M) << 8)
#define QSPI_WPSR_WPVS      (1 << 0)

/* Function: spi_atsamx7x_qspi_init
 * Initializes a SPI/QSPI
 *
 * Parameters:
 *  ctx - The SPI/QSPI to init
 *  base - The QSPI base address
 *  clkid - The clock id for this QSPI
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int spi_atsamx7x_qspi_init(struct spi *ctx, int base, clock_id_t clkid)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    ctx->base = (struct SPI_ATSAMX7X_QSPI*)base; // NOLINT
    ctx->clkid = clkid;
    ctx->frame_nbits = (size_t)8;
    ctx->frame_width = (size_t)1;
    ctx->balance = 0;

    /* reset and enable */
    ctx->base->QSPI_CR = (uint32_t)QSPI_CR_SWRST;
    ctx->base->QSPI_CR = (uint32_t)QSPI_CR_QSPIEN;

    while ((ctx->base->QSPI_SR & QSPI_SR_QSPIENS) == 0 &&
           deadlock-- != 0) arch_delay_us(10ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    /* wait data read before transfer */
    ctx->base->QSPI_MR = (uint32_t)(QSPI_MR_WDRBT |
                                    QSPI_MR_CSMODE(1)); /* LASTXFER */

    return 0;
}

int spi_atsamx7x_qspi_smm(struct spi *ctx, const struct spi_atsamx7x_qspi_smm_settings *settings)
{
    ctx->base->QSPI_MR |= QSPI_MR_SMM;
    ctx->base->QSPI_IAR = settings->addr;
    ctx->base->QSPI_ICR = (uint32_t)(QSPI_ICR_OPT(settings->opt) | QSPI_ICR_INST(settings->inst));
    ctx->base->QSPI_IFR = (uint32_t)(QSPI_IFR_NBDUM(settings->dummy_cycles) |
                                     QSPI_IFR_CRM | QSPI_IFR_TFRTYP(1) |
                                     QSPI_IFR_OPTL(3) | QSPI_IFR_DATAEN |
                                     QSPI_IFR_OPTEN | QSPI_IFR_ADDREN |
                                     QSPI_IFR_INSTEN | QSPI_IFR_WIDTH(4));
    return 0;
}

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return -EIO);

    ctx->base->QSPI_SCR &= QSPI_SCR_SCBR(QSPI_SCR_SCBR_M);
    ctx->base->QSPI_SCR |= QSPI_SCR_SCBR((unsigned long)freq / bitrate);

    return 0;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT, return -EINVAL);

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->QSPI_SCR &= ~QSPI_SCR_CPOL;
        ctx->base->QSPI_SCR &= ~QSPI_SCR_CPHA;
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->QSPI_SCR &= ~QSPI_SCR_CPOL;
        ctx->base->QSPI_SCR |= QSPI_SCR_CPHA;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->QSPI_SCR |= QSPI_SCR_CPOL;
        ctx->base->QSPI_SCR &= ~QSPI_SCR_CPHA;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->QSPI_SCR |= QSPI_SCR_CPOL;
        ctx->base->QSPI_SCR |= QSPI_SCR_CPHA;
        break;

    default:
        picoRTOS_assert(false, return -EINVAL);
    }

    return 0;
}

static int set_frame_nbits(struct spi *ctx, size_t frame_nbits)
{
#define div_ceil(x, y) (((x) + ((y) - 1)) / (y))
    picoRTOS_assert(frame_nbits >= (size_t)SPI_ATSAMX7X_QSPI_FRAME_NBITS_MIN, return -EINVAL);
    picoRTOS_assert(frame_nbits <= (size_t)SPI_ATSAMX7X_QSPI_FRAME_NBITS_MAX, return -EINVAL);

    size_t nbbits = frame_nbits - (size_t)SPI_ATSAMX7X_QSPI_FRAME_NBITS_MIN;

    ctx->base->QSPI_MR &= ~QSPI_MR_NBBITS(QSPI_MR_NBBITS_M);
    ctx->base->QSPI_MR |= QSPI_MR_NBBITS(nbbits);

    ctx->frame_nbits = frame_nbits;
    ctx->frame_width = div_ceil(frame_nbits, (size_t)8);

#undef div_ceil
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

    if (settings->frame_nbits != 0 &&
        (res = set_frame_nbits(ctx, settings->frame_nbits)) < 0)
        return res;

    /* ignore mode */
    /* ignore cs_pol */
    /* ignore cs */

    return 0;
}

static int receive_data(struct spi *ctx, uint8_t *data)
{
    if ((ctx->base->QSPI_SR & QSPI_SR_RDRF) == 0)
        return -EAGAIN;

    uint32_t qspi_rdr = ctx->base->QSPI_RDR;

    switch (ctx->frame_width) {
    case sizeof(uint8_t):
        *data = (uint8_t)qspi_rdr;
        return (int)sizeof(uint8_t);

    default:
        break;
    }

    *(uint16_t*)data = (uint16_t)qspi_rdr;
    return (int)sizeof(uint16_t);
}

static int transmit_data(struct spi *ctx, const uint8_t *data)
{
    if ((ctx->base->QSPI_SR & QSPI_SR_TXEMPTY) == 0)
        return -EAGAIN;

    switch (ctx->frame_width) {
    case sizeof(uint8_t):
        ctx->base->QSPI_TDR = (uint32_t)*data;
        return (int)sizeof(uint8_t);

    default:
        break;
    }

    ctx->base->QSPI_TDR = (uint32_t)*(uint16_t*)data;
    return (int)sizeof(uint16_t);
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
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
            if ((res = transmit_data(ctx, &tx8[tx_n])) > 0) {
                ctx->balance += res;
                xfer_occurred = true;
            }
        }

        if ((res = receive_data(ctx, &rx8[recv])) > 0) {
            recv += (size_t)res;
            ctx->balance -= res;
            xfer_occurred = true;
            /* de-assert CS as fast as possible */
            if (recv == n)
                ctx->base->QSPI_CR = (uint32_t)QSPI_CR_LASTXFER;
        }

        /* nothing was xfered, out */
        if (!xfer_occurred)
            break;
    }

    if (recv == 0)
        return -EAGAIN;

    return (int)recv;
}

#include "spi-nxp_dspi.h"
#include "picoRTOS.h"

#include <stdint.h>

struct SPI_NXP_DSPI {
    volatile uint32_t MCR;
    uint32_t RESERVED0;
    volatile uint32_t TCR;
    volatile uint32_t CTAR[6]; /* up to 8 ? */
    uint32_t RESERVED1[2];
    volatile uint32_t SR;
    volatile uint32_t RSER;
    volatile uint32_t PUSHR;
    volatile uint32_t SLAVE;
    volatile uint32_t POPR;
    volatile uint32_t TXFR[4];  /* up to 16 ? */
    uint32_t RESERVED2[12];
    volatile uint32_t RXFR[4];  /* up to 16 ? */
    uint32_t RESERVED3[12];
    volatile uint32_t DSICR0;
    volatile uint32_t SDR0;
    volatile uint32_t ASDR0;
    volatile uint32_t COMPR0;
    volatile uint32_t DDR0;
    volatile uint32_t DSICR1;
    volatile uint32_t SSR0;
    volatile uint32_t PISR0;
    volatile uint32_t PISR1;
    volatile uint32_t PISR2;
    volatile uint32_t PISR3;
    volatile uint32_t DIMR0;
    volatile uint32_t DPIR0;
    volatile uint32_t SDR1;
    volatile uint32_t ASDR1;
    volatile uint32_t COMPR1;
    volatile uint32_t DDR1;
    volatile uint32_t SSR1;
    volatile uint32_t PISR4;
    volatile uint32_t PISR5;
    volatile uint32_t PISR6;
    volatile uint32_t PISR7;
    volatile uint32_t DIMR1;
    volatile uint32_t DPIR1;
    volatile uint32_t CTARE[6]; /* up to 8 ? */
    uint32_t RESERVED4[2];
    volatile uint32_t SREX;
    volatile uint32_t TRIG;
    uint32_t RESERVED5[3];
    volatile uint32_t TSL;
    volatile uint32_t TS_CONF;
};

#define MCR_MSTR     (1u << 31)
#define MCR_DCONF_M  0x3u
#define MCR_DCONF(x) (((x) & MCR_DCONF_M) << 28)
#define MCR_PCSIS_M  0x3fu
#define MCR_PCSIS(x) (((x) & MCR_PCSIS_M) << 16)
#define MCR_MDIS     (1 << 14)
#define MCR_CLR_TXF  (1 << 11)
#define MCR_CLR_RXF  (1 << 10)
#define MCR_HALT     (1 << 0)

#define CTAR_DBR_M   0x1u
#define CTAR_DBR(x)  (((x) & CTAR_DBR_M) << 31)
#define CTAR_FMSZ_M  0xfu
#define CTAR_FMSZ(x) (((x) & CTAR_FMSZ_M) << 27)
#define CTAR_CPOL_M  0x1u
#define CTAR_CPOL(x) (((x) & CTAR_CPOL_M) << 26)
#define CTAR_CPHA_M  0x1u
#define CTAR_CPHA(x) (((x) & CTAR_CPHA_M) << 25)
#define CTAR_PBR_M   0x3u
#define CTAR_PBR(x)  (((x) & CTAR_PBR_M) << 16)
#define CTAR_BR_M    0xfu
#define CTAR_BR(x)   ((x) & CTAR_BR_M)

#define SR_TFFF (1 << 25)
#define SR_RFDF (1 << 17)

#define PUSHR_PCS_M  0x3fu
#define PUSHR_PCS(x) (((x) & PUSHR_PCS_M) << 16)

int spi_nxp_dspi_init(struct spi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct SPI_NXP_DSPI*)base;
    ctx->clkid = clkid;
    ctx->cs = 0;
    ctx->frame_size = (size_t)16;
    ctx->balance = 0;

    ctx->base->MCR |= MCR_HALT;                 /* stop if running */
    ctx->base->MCR &= ~MCR_MDIS;                /* enable clock */
    ctx->base->MCR &= ~MCR_DCONF(MCR_DCONF_M);  /* spi mode */

    /* flush FIFOs */
    ctx->base->MCR |= (MCR_CLR_TXF | MCR_CLR_RXF);

    /* deactivate all interrupts */
    ctx->base->RSER = (uint32_t)0;
    ctx->base->SR = (uint32_t)-1;

    /* run module */
    ctx->base->MCR &= ~MCR_HALT;

    return 0;
}

static int set_bitrate(struct spi *ctx, unsigned long bitrate)
{
#define PBR_COUNT (sizeof(pbr) / sizeof(*pbr))
#define BR_COUNT (sizeof(br) / sizeof(*br))

    static const unsigned long pbr[] = { 2ul, 3ul, 5ul, 7ul };
    static const unsigned long br[] = { 2ul,    4ul,    6ul,     8ul,
                                        16ul,   32ul,   64ul,    128ul,
                                        256ul,  512ul,  1024ul,  2048ul,
                                        4096ul, 8192ul, 16384ul, 32768ul };

    picoRTOS_assert(bitrate > 0, return -EINVAL);

    /* SCK baud rate = (fP / PBR) x [(1 + DBR) / BR] */

    clock_freq_t freq;

    if ((freq = clock_get_freq(ctx->clkid)) < 0)
        return (int)freq;

    size_t i, j;

    unsigned long clk = (unsigned long)freq;
    unsigned long dbr = (bitrate > (clk / 4ul)) ? 2ul : 1ul;

    for (i = 0; i < PBR_COUNT; i++) {
        for (j = 0; j < BR_COUNT; j++) {
            unsigned long x = clk / (pbr[i] * dbr) / br[j];
            unsigned long delta = bitrate - x;

            if (delta == 0) {
                ctx->base->CTAR[0] = (uint32_t)0;
                ctx->base->CTAR[0] |= CTAR_DBR(dbr - 1);
                ctx->base->CTAR[0] |= CTAR_PBR(i);
                ctx->base->CTAR[0] |= CTAR_BR(j);
                return 0;
            }
        }
    }

    /* can't compute bitrate */
    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int set_clock_mode(struct spi *ctx, spi_clock_mode_t mode)
{
    picoRTOS_assert(mode > SPI_CLOCK_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < SPI_CLOCK_MODE_COUNT, return -EINVAL);

    int cpol = 0;
    int cpha = 0;

    switch (mode) {
    case SPI_CLOCK_MODE_0: cpol = 0; cpha = 0; break;
    case SPI_CLOCK_MODE_1: cpol = 0; cpha = 1; break;
    case SPI_CLOCK_MODE_2: cpol = 1; cpha = 0; break;
    case SPI_CLOCK_MODE_3: cpol = 1; cpha = 1; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* cpol */
    ctx->base->CTAR[0] &= ~CTAR_CPOL(CTAR_CPOL_M);
    ctx->base->CTAR[0] |= CTAR_CPOL(cpol);
    /* cpha */
    ctx->base->CTAR[0] &= ~CTAR_CPHA(CTAR_CPHA_M);
    ctx->base->CTAR[0] |= CTAR_CPHA(cpha);

    return 0;
}

static int set_frame_size(struct spi *ctx, size_t nbits)
{
    picoRTOS_assert(nbits >= (size_t)4, return -EINVAL);
    picoRTOS_assert(nbits <= (size_t)16, return -EINVAL);

    ctx->base->CTAR[0] &= ~CTAR_FMSZ(CTAR_FMSZ_M);
    ctx->base->CTAR[0] |= CTAR_FMSZ(nbits - 1);

    ctx->frame_size = nbits;
    return (int)nbits;
}

static int set_mode(struct spi *ctx, spi_mode_t mode)
{
    picoRTOS_assert(mode > SPI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < SPI_MODE_COUNT, return -EINVAL);

    if (mode == SPI_MODE_MASTER) ctx->base->MCR |= MCR_MSTR;
    else ctx->base->MCR &= ~MCR_MSTR;

    return 0;
}

static int set_cs_pol(struct spi *ctx, spi_cs_pol_t pol, size_t cs)
{
    picoRTOS_assert(pol > SPI_CS_POL_IGNORE, return -EINVAL);
    picoRTOS_assert(pol < SPI_CS_POL_COUNT, return -EINVAL);
    /* TODO: check CS (devices ?) */

    if (pol == SPI_CS_POL_ACTIVE_LOW)
        ctx->base->MCR |= MCR_PCSIS(1 << cs);
    else
        ctx->base->MCR &= ~MCR_PCSIS(1 << cs);

    return 0;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res;

    /* halt module */
    ctx->base->MCR |= MCR_HALT;

    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if (settings->clkmode != SPI_CLOCK_MODE_IGNORE &&
        (res = set_clock_mode(ctx, settings->clkmode)) < 0)
        return res;

    if (settings->frame_size != 0 &&
        (res = set_frame_size(ctx, settings->frame_size)) < 0)
        return res;

    if (settings->mode != SPI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    if (settings->cs_pol != SPI_CS_POL_IGNORE &&
        (res = set_cs_pol(ctx, settings->cs_pol, settings->cs)) < 0)
        return res;

    /* cs */
    ctx->cs = settings->cs;

    /* run module */
    ctx->base->MCR &= ~MCR_HALT;

    return 0;
}

static int push_frame(struct spi *ctx, const uint8_t *frame)
{
    int res;

    /* if tx fifo full */
    if ((ctx->base->SR & SR_TFFF) == 0)
        return -EAGAIN;

    /* push data */
    if (ctx->frame_size > (size_t)8) {
        ctx->base->PUSHR = (uint32_t)(PUSHR_PCS(1 << ctx->cs) | *(uint16_t*)frame);
        res = (int)sizeof(uint16_t);
    }else{
        ctx->base->PUSHR = (uint32_t)(PUSHR_PCS(1 << ctx->cs) | *frame);
        res = (int)sizeof(uint8_t);
    }

    /* ack flag */
    ctx->base->SR |= SR_TFFF;
    return res;
}

static int pop_frame(struct spi *ctx, uint8_t *frame)
{
    int res;

    /* if rx data available */
    if ((ctx->base->SR & SR_RFDF) == 0)
        return -EAGAIN;

    /* pop data */
    if (ctx->frame_size > (size_t)8) {
        *(uint16_t*)frame = (uint16_t)ctx->base->POPR;
        res = (int)sizeof(uint16_t);
    }else{
        *frame = (uint8_t)ctx->base->POPR;
        res = (int)sizeof(uint8_t);
    }

    /* ack flag */
    ctx->base->SR |= SR_RFDF;
    return res;
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    /* 16bit check */
    if (ctx->frame_size > (size_t)8)
        picoRTOS_assert((n & 0x1) == 0, return -EINVAL);

    size_t recv = 0;
    uint8_t *rx8 = rx;
    const uint8_t *tx8 = tx;

    while (recv != n) {

        int res;
        bool xfer_occurred = false;
        size_t tx_n = recv + ctx->balance;

        if (tx_n < n) {
            /* fill up TX FIFO */
            if ((res = push_frame(ctx, &tx8[tx_n])) > 0) {
                ctx->balance += res;
                xfer_occurred = true;
            }
        }

        if ((res = pop_frame(ctx, &rx8[recv])) > 0) {
            recv += (size_t)res;
            ctx->balance--;
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

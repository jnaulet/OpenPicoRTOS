#include "spi-avr-usart.h"
#include "picoRTOS.h"

#include <stdint.h>

struct MSPIM_AVR_USART {
    volatile uint8_t UCSRnA;
    volatile uint8_t UCSRnB;
    volatile uint8_t UCSRnC;
    volatile uint8_t RESERVED;
    volatile uint16_t UBRRn;
    volatile uint8_t UDRn;
};

#define UCSRnA_RXC  (1 << 7)
#define UCSRnA_TXC  (1 << 6)
#define UCSRnA_UDRE (1 << 5)

#define UCSRnB_RXEN (1 << 4)
#define UCSRnB_TXEN (1 << 3)

#define UCSRnC_UMSELn_M   0x3u
#define UCSRnC_UMSELn(x) (((x) & UCSRnC_UMSELn_M) << 6)
#define UCSRnC_UDORDn    (1 << 2)
#define UCSRnC_UCPHAn    (1 << 1)
#define UCSRnC_UCPOLn    (1 << 0)

static int set_baudrate(struct spi *ctx, unsigned long baud)
{
    if (!picoRTOS_assert(baud > 0)) return -EINVAL;

    clock_freq_t freq = clock_get_freq(ctx->clkid);
    unsigned long ubrr = (unsigned long)freq / (2 * baud) - 1;

    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    ctx->base->UBRRn = (uint16_t)ubrr;

    /* TODO: check freq match */
    return 0;
}

/* public */

/* Function: spi_avr_usart_init
 * Inits an USART in SPI master mode
 *
 * Parameters:
 *  ctx - The SPI to init
 *  base - The USART base address
 *  clkid - The USART clock ID
 *
 * Returns:
 * Always 0
 */
int spi_avr_usart_init(struct spi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct MSPIM_AVR_USART*)base;
    ctx->clkid = clkid;
    ctx->state = SPI_AVR_USART_STATE_START;

    /* set to MSPIM mode */
    ctx->base->UCSRnC |= UCSRnC_UMSELn(UCSRnC_UMSELn_M);

    /* turn on */
    ctx->base->UCSRnB = (uint8_t)(UCSRnB_TXEN | UCSRnB_RXEN);
    return 0;
}

static int set_clkmode(struct spi *ctx, spi_clock_mode_t clkmode)
{
    if (!picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT)) return -EINVAL;

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        ctx->base->UCSRnC &= ~UCSRnC_UCPOLn;
        ctx->base->UCSRnC &= ~UCSRnC_UCPHAn;
        break;

    case SPI_CLOCK_MODE_1:
        ctx->base->UCSRnC &= ~UCSRnC_UCPOLn;
        ctx->base->UCSRnC |= UCSRnC_UCPHAn;
        break;

    case SPI_CLOCK_MODE_2:
        ctx->base->UCSRnC |= UCSRnC_UCPOLn;
        ctx->base->UCSRnC &= ~UCSRnC_UCPHAn;
        break;

    case SPI_CLOCK_MODE_3:
        ctx->base->UCSRnC |= UCSRnC_UCPOLn;
        ctx->base->UCSRnC |= UCSRnC_UCPHAn;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EINVAL;
    }

    return 0;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res = 0;

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_baudrate(ctx, settings->bitrate)) < 0)
        return res;

    /* clock mode */
    if (settings->clkmode != SPI_CLOCK_MODE_IGNORE &&
        (res = set_clkmode(ctx, settings->clkmode)) < 0)
        return res;

    /* mode: ignore */
    /* frame_size: ignore */
    /* cs_pol: ignore */
    /* cs: ignore */

    return res;
}

static int run_state_start(struct spi *ctx, const void *tx)
{
    ctx->base->UDRn = *(uint8_t*)tx;
    ctx->state = SPI_AVR_USART_STATE_XFER;
    return -EAGAIN;
}

static int run_state_xfer(struct spi *ctx, void *rx,
                          const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    if ((ctx->base->UCSRnA & UCSRnA_RXC) == 0 ||
        (ctx->base->UCSRnA & UCSRnA_TXC) == 0)
        return -EAGAIN;

    *(uint8_t*)rx = ctx->base->UDRn;            /* UCSRnA_RXC moves to 0 */
    ctx->base->UCSRnA = (uint8_t)UCSRnA_TXC;    /* clear UCSRnA_TXC */

    if (n > (size_t)1)
        /* continue xfer */
        ctx->base->UDRn = *((uint8_t*)tx + 1);
    else
        ctx->state = SPI_AVR_USART_STATE_START;

    return 1;
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case SPI_AVR_USART_STATE_START: return run_state_start(ctx, tx);
    case SPI_AVR_USART_STATE_XFER: return run_state_xfer(ctx, rx, tx, n);
    default:
        break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

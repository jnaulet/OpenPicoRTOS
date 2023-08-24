#include "uart-tinyavr_usart.h"
#include "picoRTOS.h"

#include <stdint.h>

struct USART_TINYAVR_UART {
    volatile uint8_t RXDATAL;
    volatile uint8_t RXDATAH;
    volatile uint8_t TXDATAL;
    volatile uint8_t TXDATAH;
    volatile uint8_t STATUS;
    volatile uint8_t CTRLA;
    volatile uint8_t CTRLB;
    volatile uint8_t CTRLC;
    volatile uint16_t BAUD;
};

#define RXDATAH_RXCIF  (1 << 7)
#define RXDATAH_BUFOVF (1 << 6)
#define RXDATAH_FERR   (1 << 2)
#define RXDATAH_PERR   (1 << 1)
#define RXDATAH_DATA8  (1 << 0)

#define TXDATAH_DATA8  (1 << 0)

#define STATUS_RXCIF (1 << 7)
#define STATUS_TXCIF (1 << 6)
#define STATUS_DREIF (1 << 5)
#define STATUS_RXSIF (1 << 4)
#define STATUS_ISFIF (1 << 3)
#define STATUS_BDF   (1 << 1)
#define STATUS_WFB   (1 << 0)

#define CTRLA_RXCIE    (1 << 7)
#define CTRLA_TXCIE    (1 << 6)
#define CTRLA_DREIE    (1 << 5)
#define CTRLA_RXSIE    (1 << 4)
#define CTRLA_LBME     (1 << 3)
#define CTRLA_ABEIE    (1 << 2)
#define CTRLA_RS485_M  0x3u
#define CTRLA_RS485(x) ((x) & CTRLA_RS485_M)

#define CTRLB_RXEN      (1 << 7)
#define CTRLB_TXEN      (1 << 6)
#define CTRLB_SFDEN     (1 << 4)
#define CTRLB_ODME      (1 << 3)
#define CTRLB_RXMODE_M  0x3u
#define CTRLB_RXMODE(x) (((x) & CTRLB_RXMODE_M) << 1)
#define CTRLB_MPCM      (1 << 0)

#define CTRLC_CMODE_M   0x3u
#define CTRLC_CMODE(x)  (((x) & CTRLC_CMODE_M) << 6)
#define CTRLC_PMODE_M   0x3u
#define CTRLC_PMODE(x)  (((x) & CTRLC_PMODE_M) << 4)
#define CTRLC_SBMODE    (1 << 3)
#define CTRLC_CHSIZE_M  0x7u
#define CTRLC_CHSIZE(x) ((x) & CTRLC_CHSIZE_M)
#define CTRLC_UDORD     (1 << 2)
#define CTRLC_UCPHA     (1 << 1)

/* Function: uart_tinyavr_usart_init
 * Initializes an USART as UART
 *
 * Parameters:
 *  ctx - The UART to init
 *  base - The USART base address
 *  clkid - The USART clock ID
 *
 * Returns:
 * Always 0
 */
int uart_tinyavr_usart_init(struct uart *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct USART_TINYAVR_UART*)base;
    ctx->clkid = clkid;

    /* turn on (double-speed mode) */
    ctx->base->CTRLB = (uint8_t)(CTRLB_RXEN | CTRLB_TXEN);
    /* async mode */
    ctx->base->CTRLC &= ~CTRLC_CMODE(CTRLC_CMODE_M);

    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baud)
{
    picoRTOS_assert(baud > 0, return -EINVAL);

    clock_freq_t freq = clock_get_freq(ctx->clkid);
    unsigned long ipart = (unsigned long)freq / 16ul / baud;

    /* TODO: compute fractional part */

    picoRTOS_assert(freq > 0, return -EIO);

    ctx->base->BAUD = (uint16_t)(ipart << 6);
    return 0;
}

static int set_cs(struct uart *ctx, size_t cs)
{
    picoRTOS_assert(cs >= (size_t)USART_TINYAVR_CS_MIN, return -EINVAL);
    picoRTOS_assert(cs <= (size_t)USART_TINYAVR_CS_MAX, return -EINVAL);

    ctx->base->CTRLC &= ~CTRLC_CHSIZE(CTRLC_CHSIZE_M);

    switch (cs) {
    case 5: ctx->base->CTRLC |= CTRLC_CHSIZE(0); break;
    case 6: ctx->base->CTRLC |= CTRLC_CHSIZE(1); break;
    case 7: ctx->base->CTRLC |= CTRLC_CHSIZE(2); break;
    case 8: ctx->base->CTRLC |= CTRLC_CHSIZE(3); break;
    // case 9: ctrlc |= CTRLC_CHSIZE(6); break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return 0;
}

static int set_parity(struct uart *ctx, uart_par_t par)
{
    picoRTOS_assert(par != UART_PAR_IGNORE, return -EINVAL);
    picoRTOS_assert(par < UART_PAR_COUNT, return -EINVAL);

    ctx->base->CTRLC &= ~CTRLC_PMODE(CTRLC_PMODE_M);
    if (par == UART_PAR_NONE)
        return 0;

    if (par == UART_PAR_ODD) ctx->base->CTRLC |= CTRLC_PMODE(0x3);
    else ctx->base->CTRLC |= CTRLC_PMODE(0x2);

    return 0;
}

static int set_cstopb(struct uart *ctx, uart_cstopb_t cstopb)
{
    picoRTOS_assert(cstopb != UART_CSTOPB_IGNORE, return -EINVAL);
    picoRTOS_assert(cstopb < UART_CSTOPB_COUNT, return -EINVAL);

    if (cstopb == UART_CSTOPB_2BIT) ctx->base->CTRLC |= CTRLC_SBMODE;
    else ctx->base->CTRLC &= ~CTRLC_SBMODE;

    return 0;
}

int uart_setup(struct uart *ctx, const struct uart_settings *settings)
{
    int res;

    /* baudrate */
    if (settings->baudrate != 0 &&
        (res = set_baudrate(ctx, settings->baudrate)) < 0)
        return res;

    /* cs */
    if (settings->cs != 0 &&
        (res = set_cs(ctx, settings->cs)) < 0)
        return res;

    /* parity */
    if (settings->par != UART_PAR_IGNORE &&
        (res = set_parity(ctx, settings->par)) < 0)
        return res;

    /* cstopb */
    if (settings->cstopb != UART_CSTOPB_IGNORE &&
        (res = set_cstopb(ctx, settings->cstopb)) < 0)
        return res;

    return 0;
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    size_t nwritten = 0;

    while (nwritten != n) {

        if ((ctx->base->STATUS & STATUS_DREIF) == (uint8_t)0)
            break;

        ctx->base->TXDATAL = (uint8_t)*buf++;
        nwritten++;
    }

    if (nwritten == 0)
        return -EAGAIN;

    return (int)nwritten;
}

int uart_read(struct uart *ctx, char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    size_t nread = 0;

    while (nread != n) {

        if ((ctx->base->STATUS & STATUS_RXCIF) == 0 ||
            (ctx->base->RXDATAH & RXDATAH_FERR) != 0)
            break;

        *buf++ = (char)ctx->base->RXDATAL;
        nread++;
    }

    if (nread == 0)
        return -EAGAIN;

    return (int)nread;
}

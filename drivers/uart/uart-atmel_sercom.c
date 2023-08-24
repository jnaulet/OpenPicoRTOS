#include "uart-atmel_sercom.h"
#include "picoRTOS.h"

#include <stdint.h>

struct UART_ATMEL_SERCOM {
    volatile uint32_t CTRLA;
    volatile uint32_t CTRLB;
    volatile uint32_t CTRLC;
    volatile uint16_t BAUD;
    volatile uint16_t RXPL;
    uint32_t RESERVED0;
    volatile uint16_t INTENCLR;
    volatile uint16_t INTENSET;
    volatile uint16_t INTFLAG;
    volatile uint16_t STATUS;
    volatile uint32_t SYNCBUSY;
    volatile uint16_t RXERRCNT;
    volatile uint16_t LENGTH;
    uint32_t RESERVED1;
    volatile uint32_t DATA;
    uint32_t RESERVED2;
    volatile uint8_t DBGCTRL;
};

#define CTRLA_DORD     (1 < 30)
#define CTRLA_CPOL     (1 < 29)
#define CTRLA_CMODE    (1 < 28)
#define CTRLA_FORM_M   0xfu
#define CTRLA_FORM(x)  (((x) & CTRLA_FORM_M) << 24)
#define CTRLA_SAMPA_M  0x3u
#define CTRLA_SAMPA(x) (((x) & CTRLA_SAMPA_M) << 22)
#define CTRLA_RXPO_M   0x3u
#define CTRLA_RXPO(x)  (((x) & CTRLA_RXPO_M) << 20)
#define CTRLA_TXPO_M   0x3u
#define CTRLA_TXPO(x)  (((x) & CTRLA_TXPO_M) << 16)
#define CTRLA_SAMPR_M  0x7u
#define CTRLA_SAMPR(x) (((x) & CTRLA_SAMPR_M) << 13)
#define CTRLA_RXINV    (1 << 10)
#define CTRLA_TXINV    (1 << 9)
#define CTRLA_IBON     (1 << 8)
#define CTRLA_RUNSTBY  (1 << 7)
#define CTRLA_MODE_M   0x7u
#define CTRLA_MODE(x)  (((x) & CTRLA_MODE_M) << 2)
#define CTRLA_ENABLE   (1 << 1)
#define CTRLA_SWRST    (1 << 0)

#define CTRLB_LINCMD_M  0x3u
#define CTRLB_LINCMD(x) (((x) & CTRLB_LINCMD_M) << 24)
#define CTRLB_RXEN      (1 << 17)
#define CTRLB_TXEN      (1 << 16)
#define CTRLB_PMODE     (1 << 13)
#define CTRLB_ENC       (1 << 10)
#define CTRLB_SFDE      (1 << 9)
#define CTRLB_COLDEN    (1 << 8)
#define CTRLB_SBMODE    (1 << 6)
#define CTRLB_CHSIZE_M  0x7u
#define CTRLB_CHSIZE(x) ((x) & CTRLB_CHSIZE_M)

#define CTRLC_DATA32B_M  0x3u
#define CTRLC_DATA32B(x) (((x) & CTRLC_DATA32B_M) << 24)
#define CTRLC_MAXITER_M  0x7u
#define CTRLC_MAXITER(x) (((x) & CTRLC_MAXITER_M) << 20)
#define CTRLC_DSNACK     (1 << 17)
#define CTRLC_INACK      (1 << 16)
#define CTRLC_HDRDLY_M   0x3u
#define CTRLC_HDRDLY(x)  (((x) & CTRLC_HDRDLY_M) << 10)
#define CTRLC_BRKLEN_M   0x3u
#define CTRLC_BRKLEN(x)  (((x) & CTRLC_BRKLEN_M) << 8)
#define CTRLC_GTIME_M    0x7u
#define CTRLC_GTIME(x)   ((x) & CTRLC_GTIME_M)

#define RXPL_RXPL_M  0xffu
#define RXPL_RXPL(x) ((x) & RXPL_RXPL_M)

#define INTFLAG_ERROR (1 << 7)
#define INTFLAG_RXBRK (1 << 5)
#define INTFLAG_CTSIC (1 << 4)
#define INTFLAG_RXS   (1 << 3)
#define INTFLAG_RXC   (1 << 2)
#define INTFLAG_TXC   (1 << 1)
#define INTFLAG_DRE   (1 << 0)

#define STATUS_ITER   (1 << 7)
#define STATUS_TXE    (1 << 6)
#define STATUS_COLL   (1 << 5)
#define STATUS_ISF    (1 << 4)
#define STATUS_CTS    (1 << 3)
#define STATUS_BUFOVF (1 << 2)
#define STATUS_FERR   (1 << 1)
#define STATUS_PERR   (1 << 0)

#define SYNCBUSY_LENGTH   (1 << 4)
#define SYNCBUSY_RXERRCNT (1 << 3)
#define SYNCBUSY_CTRLB  (1 << 2)
#define SYNCBUSY_ENABLE (1 << 1)
#define SYNCBUSY_SWRST  (1 << 0)

#define LENGTH_LENEN  (1 << 8)
#define LENGTH_LEN_M  0xffu
#define LENGTH_LEN(x) ((x) & LENGTH_LEN_M)

#define DBGCTRL_DBGSTOP (1 << 0)

static int sync_busywait(struct uart *ctx, uint32_t mask)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((ctx->base->SYNCBUSY & mask) == 0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

/* Function: uart_atmel_sercom_init
 * Initializes a SERCOM UART
 *
 * Parameters:
 *  ctx - The UART to init
 *  base - The SERCOM base address
 *  clkid - The clock id for this UART
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int uart_atmel_sercom_init(struct uart *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct UART_ATMEL_SERCOM*)base;
    ctx->clkid = clkid;

    /* uart basic setup */
    ctx->base->CTRLA &= ~(CTRLA_CMODE | CTRLA_MODE(CTRLA_MODE_M));
    ctx->base->CTRLA |= CTRLA_MODE(0x1);

    ctx->base->CTRLB |= (CTRLB_RXEN | CTRLB_TXEN);
    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baudrate)
{
    picoRTOS_assert(baudrate > 0, return -EINVAL);

    unsigned long baud;
    unsigned long factor1 = baudrate * 16ul;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return (int)freq);
    picoRTOS_assert(factor1 <= (unsigned long)freq, return -EINVAL);

    /* According to datasheet, BAUD = 65536 . (1 - 16 . fBAUD / fref) */
    baud = 0x10000ul * (1024ul - (factor1 * 1024ul) / freq) / 1024ul;
    picoRTOS_assert(baud < 0x10000ul, return -EINVAL);

    ctx->base->BAUD = (uint16_t)baud;
    return 0;
}

static int set_cs(struct uart *ctx, size_t cs)
{
    picoRTOS_assert(cs >= (size_t)UART_ATMEL_SERCOM_CS_MIN, return -EINVAL);
    picoRTOS_assert(cs <= (size_t)UART_ATMEL_SERCOM_CS_MAX, return -EINVAL);

    ctx->base->CTRLB &= ~CTRLB_CHSIZE(CTRLB_CHSIZE_M);
    ctx->base->CTRLB |= CTRLB_CHSIZE(cs % (size_t)8);

    return 0;
}

static int set_parity(struct uart *ctx, uart_par_t par)
{
    picoRTOS_assert(par != UART_PAR_IGNORE, return -EINVAL);
    picoRTOS_assert(par < UART_PAR_COUNT, return -EINVAL);

    ctx->base->CTRLA &= ~CTRLA_FORM(CTRLA_FORM_M);
    if (par == UART_PAR_NONE)
        return 0;

    ctx->base->CTRLA |= CTRLA_FORM(0x1);
    if (par == UART_PAR_ODD) ctx->base->CTRLB |= CTRLB_PMODE;
    else ctx->base->CTRLB &= ~CTRLB_PMODE;

    return 0;
}

static int set_cstopb(struct uart *ctx, uart_cstopb_t cstopb)
{
    picoRTOS_assert(cstopb != UART_CSTOPB_IGNORE, return -EINVAL);
    picoRTOS_assert(cstopb < UART_CSTOPB_COUNT, return -EINVAL);

    if (cstopb == UART_CSTOPB_2BIT) ctx->base->CTRLB |= CTRLB_SBMODE;
    else ctx->base->CTRLB &= ~CTRLB_SBMODE;

    return 0;
}

int uart_setup(struct uart *ctx, const struct uart_settings *settings)
{
    int res;

    ctx->base->CTRLA &= ~CTRLA_ENABLE;

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

    ctx->base->CTRLA |= CTRLA_ENABLE;
    return sync_busywait(ctx, (uint32_t)SYNCBUSY_ENABLE);
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int sent = 0;

    while ((size_t)sent != n) {
        if ((ctx->base->INTFLAG & INTFLAG_DRE) == 0)
            break;

        ctx->base->DATA = (uint32_t)buf[sent++];
    }

    if (sent == 0)
        return -EAGAIN;

    return sent;
}

int uart_read(struct uart *ctx, char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int recv = 0;

    while ((size_t)recv != n) {
        if ((ctx->base->INTFLAG & INTFLAG_RXC) == 0)
            break;

        buf[recv++] = (char)ctx->base->DATA;
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

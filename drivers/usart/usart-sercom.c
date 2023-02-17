#include "usart-sercom.h"
#include "picoRTOS.h"

#include <stdint.h>

struct USART_SERCOM {
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

static int sync_busywait(struct usart *ctx, uint32_t mask)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((ctx->base->SYNCBUSY & mask) == 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

int usart_sercom_init(struct usart *ctx, struct USART_SERCOM *base, clock_id_t clkid)
{
    ctx->base = base;
    ctx->clkid = clkid;
    ctx->balance = 0;
    ctx->frame_size = 0;

    /* usart generic setup */
    ctx->base->CTRLA &= ~CTRLA_MODE(CTRLA_MODE_M);
    ctx->base->CTRLA |= (CTRLA_CMODE | CTRLA_MODE(0x1));

    ctx->base->CTRLB |= (CTRLB_RXEN | CTRLB_TXEN);
    return sync_busywait(ctx, (uint32_t)SYNCBUSY_CTRLB);
}

static int set_bitrate(struct usart *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    unsigned long baud;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return (int)freq;

    /* According to datasheet, BAUD = fref / (2 * fBAUD) - 1 */
    baud = (unsigned long)freq / (2ul * bitrate) - 1ul;
    if (!picoRTOS_assert(baud < 0x100ul))
        return -EINVAL;

    ctx->base->BAUD = (uint16_t)baud;
    return 0;
}

static int set_frame_size(struct usart *ctx, size_t frame_size)
{
    if (!picoRTOS_assert(frame_size >= (size_t)USART_SERCOM_FRAME_SIZE_MIN)) return -EINVAL;
    if (!picoRTOS_assert(frame_size <= (size_t)USART_SERCOM_FRAME_SIZE_MAX)) return -EINVAL;

    ctx->base->CTRLC &= ~CTRLC_DATA32B(CTRLC_DATA32B_M);

    if (frame_size > (size_t)8) {
        size_t len = ((frame_size - 1) >> 3) + 1;
        ctx->base->LENGTH = (uint16_t)(LENGTH_LENEN | LENGTH_LEN(len));
        ctx->base->CTRLC |= CTRLC_DATA32B(0x3);
    }

    ctx->frame_size = frame_size;
    return sync_busywait(ctx, (uint32_t)SYNCBUSY_LENGTH);
}

int usart_sercom_setup(struct usart *ctx, const struct usart_sercom_settings *settings)
{
    int res;

    ctx->base->CTRLA &= ~CTRLA_ENABLE;

    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if (settings->frame_size != 0 &&
        (res = set_frame_size(ctx, settings->frame_size)) < 0)
        return res;

    ctx->base->CTRLA |= CTRLA_ENABLE;
    return sync_busywait(ctx, (uint32_t)SYNCBUSY_ENABLE);
}

static int write_data(struct usart *ctx, const uint8_t *data)
{
    if ((ctx->base->INTFLAG & INTFLAG_DRE) == 0)
        return -EAGAIN;

    if (ctx->frame_size <= (size_t)8) {
        ctx->base->DATA = (uint32_t)*data;
        return (int)sizeof(uint8_t);
    }

    if (ctx->frame_size <= (size_t)16) {
        ctx->base->DATA = (uint32_t)*(uint16_t*)data;
        return (int)sizeof(uint16_t);
    }

    ctx->base->DATA = *(uint32_t*)data;
    return (int)sizeof(uint32_t);
}

static int read_data(struct usart *ctx, uint8_t *data)
{
    if ((ctx->base->INTFLAG & INTFLAG_RXC) == 0)
        return -EAGAIN;

    if (ctx->frame_size <= (size_t)8) {
        *data = (uint8_t)ctx->base->DATA;
        return (int)sizeof(uint8_t);
    }

    if (ctx->frame_size <= (size_t)16) {
        *data = (uint16_t)ctx->base->DATA;
        return (int)sizeof(uint16_t);
    }

    *(uint32_t*)data = ctx->base->DATA;
    return (int)sizeof(uint32_t);
}

int usart_xfer(struct usart *ctx, void *rx, const void *tx, size_t n)
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

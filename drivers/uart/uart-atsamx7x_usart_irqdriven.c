#include "uart-atsamx7x_usart_irqdriven.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct UART_ATSAMX7X_USART {
    volatile uint32_t US_CR;
    volatile uint32_t US_MR;
    volatile uint32_t US_IER;
    volatile uint32_t US_IDR;
    volatile uint32_t US_IMR;
    volatile uint32_t US_CSR;
    volatile uint32_t US_RHR;
    volatile uint32_t US_THR;
    volatile uint32_t US_BRGR;
    volatile uint32_t US_RTOR;
    volatile uint32_t US_TTGR;
    uint32_t RESERVED0[5];
    volatile uint32_t US_FIDI;
    volatile uint32_t US_NER;
    uint32_t RESERVED1[1];
    volatile uint32_t US_IF;
    volatile uint32_t US_MAN;
    volatile uint32_t US_LINMR;
    volatile uint32_t US_LIMIR;
    volatile uint32_t US_LIMBR;
    volatile uint32_t US_LONMR;
    volatile uint32_t US_LONPR;
    volatile uint32_t US_LONDL;
    volatile uint32_t US_LONL2HDR;
    volatile uint32_t US_LONBL;
    volatile uint32_t US_LONB1TX;
    volatile uint32_t US_LONB1RX;
    volatile uint32_t US_LONPRIO;
    volatile uint32_t US_IDTTX;
    volatile uint32_t US_IDTRX;
    volatile uint32_t US_ICDIFF;
    uint32_t RESERVED2[22];
    volatile uint32_t US_WMPR;
    volatile uint32_t US_WPSR;
};

#define US_CR_LINWKUP (1 << 21)
#define US_CR_LINABT  (1 << 20)
#define US_CR_RTSDIS  (1 << 19)
#define US_CR_RTSEN   (1 << 18)
#define US_CR_DTRDIS  (1 << 17)
#define US_CR_DTREN   (1 << 16)
#define US_CR_RETTO   (1 << 15)
#define US_CR_RSTNACK (1 << 14)
#define US_CR_RSTIT   (1 << 13)
#define US_CR_SENDA   (1 << 12)
#define US_CR_STTTO   (1 << 11)
#define US_CR_STPBRK  (1 << 10)
#define US_CR_STTBRK  (1 << 9)
#define US_CR_RSTSTA  (1 << 8)
#define US_CR_TXDIS   (1 << 7)
#define US_CR_TXEN    (1 << 6)
#define US_CR_RXDIS   (1 << 5)
#define US_CR_RXEN    (1 << 4)
#define US_CR_RSTTX   (1 << 3)
#define US_CR_RSTRX   (1 << 2)

#define US_MR_ONEBIT           (1u << 31)
#define US_MR_MODSYNC          (1 << 30)
#define US_MR_MAN              (1 << 29)
#define US_MR_FILTER           (1 << 28)
#define US_MR_MAX_ITERATION_M  0x7u
#define US_MR_MAX_ITERATION(x) (((x) & US_MR_MAX_ITERATION_M) << 24)
#define US_MR_INVDATA          (1 << 23)
#define US_MR_VAR_SYNC         (1 << 22)
#define US_MR_DSNACK           (1 << 21)
#define US_MR_INACK            (1 << 20)
#define US_MR_OVER             (1 << 19)
#define US_MR_CLKO             (1 << 18)
#define US_MR_MODE9            (1 << 17)
#define US_MR_MSBF             (1 << 16)
#define US_MR_CHMODE_M         0x3u
#define US_MR_CHMODE(x)        (((x) & US_MR_CHMODE_M) << 14)< 14)
#define US_MR_NBSTOP_M         0x3u
#define US_MR_NBSTOP(x)        (((x) & US_MR_NBSTOP_M) << 12)
#define US_MR_PAR_M            0x7u
#define US_MR_PAR(x)           (((x) & US_MR_PAR_M) << 9)
#define US_MR_SYNC             (1 << 8)
#define US_MR_CHRL_M           0x3u
#define US_MR_CHRL(x)          (((x) & US_MR_CHRL_M) << 6)
#define US_MR_USCLKS_M         0x3u
#define US_MR_USCLKS(x)        (((x) & US_MR_USCLKS_M) << 4)
#define US_MR_USART_MODE_M     0xfu
#define US_MR_USART_MODE(x)    ((x) & US_MR_USART_MODE_M)

#define US_CSR_MANERR  (1 << 24)
#define US_CSR_CTS     (1 << 23)
#define US_CSR_DCD     (1 << 22)
#define US_CSR_DSR     (1 << 21)
#define US_CSR_RI      (1 << 20)
#define US_CSR_CTSIC   (1 << 19)
#define US_CSR_DCDIC   (1 << 18)
#define US_CSR_DSRIC   (1 << 17)
#define US_CSR_RIIC    (1 << 15)
#define US_CSR_NACK    (1 << 13)
#define US_CSR_ITER    (1 << 10)
#define US_CSR_TXEMPTY (1 << 9)
#define US_CSR_TIMEOUT (1 << 8)
#define US_CSR_PARE    (1 << 7)
#define US_CSR_FRAME   (1 << 6)
#define US_CSR_OVRE    (1 << 5)
#define US_CSR_RXBRK   (1 << 3)
#define US_CSR_TXRDY   (1 << 1)
#define US_CSR_RXRDY   (1 << 0)

#define US_RHR_RXSYNH (1 << 15)

#define US_THR_TXSYNH (1 << 15)

#define US_BRGR_FP_M  0x7u
#define US_BRGR_FP(x) (((x) & US_BRGR_FP_M) << 16)
#define US_BRGR_CD_M  0xffffu
#define US_BRGR_CD(x) ((x) & US_BRGR_CD_M)

#define US_WPMR_WPKEY_M  0xffffffu
#define US_WPMR_WPKEY(x) (((x) & US_WPMR_WPKEY_M) << 8)
#define US_WPMR_WPEN     (1 << 0)

static void uart_isr(void *priv)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;
    struct uart *ctx = (struct uart*)priv;

    /* rx */
    while ((ctx->base->US_CSR & US_CSR_RXRDY) != 0 &&
           deadlock-- != 0) {
        /* store data if possible */
        if (fifo_head_is_writable(&ctx->rx_fifo)) {
            ctx->rx_buf[ctx->rx_fifo.w] = (char)ctx->base->US_RHR;
            fifo_head_push(&ctx->rx_fifo);
        }else{ /* drop */
            /*@i@*/ (void)ctx->base->US_RHR;
            break;
        }
    }

    /* check */
    picoRTOS_assert_void(deadlock != -1);

    /* tx */
    while ((ctx->base->US_CSR & US_CSR_TXEMPTY) != 0 &&
           deadlock-- != 0) {
        /* unstack data if possible */
        if (fifo_head_is_readable(&ctx->tx_fifo)) {
            fifo_head_pop(&ctx->tx_fifo);
            ctx->base->US_THR = (uint32_t)ctx->tx_buf[ctx->tx_fifo.r];
        }else{
            /* disable */
            ctx->base->US_IDR = (uint32_t)US_CSR_TXEMPTY;
            break;
        }
    }

    /* check */
    picoRTOS_assert_void(deadlock != -1);
}

/* Function: uart_atsamx7x_usart_irqdriven_init
 * Initializes an irqdriven UART/USART
 *
 * Parameters:
 *  ctx - The UART to init
 *  base - The UART base address
 *  clkid - the UART clock ID
 *  irq - the UART-related IRQ
 *
 * Returns:
 * Always 0
 */
int uart_atsamx7x_usart_irqdriven_init(struct uart *ctx, int base, clock_id_t clkid, picoRTOS_irq_t irq)
{
    picoRTOS_assert(irq <= (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return -EINVAL);

    ctx->base = (struct UART_ATSAMX7X_USART*)base; // NOLINT
    ctx->clkid = clkid;

    fifo_head_init(&ctx->rx_fifo, sizeof(ctx->rx_buf) - 1);
    fifo_head_init(&ctx->tx_fifo, sizeof(ctx->tx_buf) - 1);

    /* register interrupt */
    picoRTOS_register_interrupt(irq, uart_isr, ctx);

    /* reset & enable */
    ctx->base->US_CR = (uint32_t)(US_CR_RSTSTA | US_CR_RSTTX | US_CR_RSTRX);
    ctx->base->US_CR = (uint32_t)(US_CR_TXEN | US_CR_RXEN);

    /* enable rx int */
    ctx->base->US_IER = (uint32_t)US_CSR_RXRDY;
    picoRTOS_enable_interrupt(irq);

    return 0;
}

int uart_atsamx7x_usart_set_usclks(struct uart *ctx, uart_atsamx7x_usart_usclks_t usclks)
{
    picoRTOS_assert(usclks < UART_ATSAMX7X_USART_USCLKS_COUNT, return -EINVAL);

    ctx->base->US_MR &= ~US_MR_USCLKS(US_MR_USCLKS_M);
    ctx->base->US_MR |= US_MR_USCLKS(usclks);

    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baudrate)
{
#define div_round(a, b) (((a) + ((b)/2)) / (b))
    picoRTOS_assert(baudrate > 0, return -EINVAL);

    unsigned long cd;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return (int)freq);

    cd = div_round((unsigned long)freq, (baudrate * 16ul));
    ctx->base->US_BRGR = (uint32_t)US_BRGR_CD(cd);

#undef div_round
    return 0;
}

static int set_cs(struct uart *ctx, size_t cs)
{
    picoRTOS_assert(cs >= (size_t)UART_ATSAMX7X_USART_CS_MIN, return -EINVAL);
    picoRTOS_assert(cs <= (size_t)UART_ATSAMX7X_USART_CS_MAX, return -EINVAL);

    ctx->base->US_MR &= ~US_MR_CHRL(US_MR_CHRL_M);
    ctx->base->US_MR |= US_MR_CHRL(cs - (size_t)5);

    return 0;
}

static int set_parity(struct uart *ctx, uart_par_t par)
{
    picoRTOS_assert(par != UART_PAR_IGNORE, return -EINVAL);
    picoRTOS_assert(par < UART_PAR_COUNT, return -EINVAL);

    ctx->base->US_MR &= ~US_MR_PAR(US_MR_PAR_M);

    if (par == UART_PAR_NONE) {
        ctx->base->US_MR |= US_MR_PAR(4);
        return 0;
    }

    if (par == UART_PAR_ODD) ctx->base->US_MR |= US_MR_PAR(1);
    else ctx->base->US_MR |= US_MR_PAR(0);

    return 0;
}

static int set_cstopb(struct uart *ctx, uart_cstopb_t cstopb)
{
    picoRTOS_assert(cstopb != UART_CSTOPB_IGNORE, return -EINVAL);
    picoRTOS_assert(cstopb < UART_CSTOPB_COUNT, return -EINVAL);

    ctx->base->US_MR &= ~US_MR_NBSTOP(US_MR_NBSTOP_M);
    if (cstopb == UART_CSTOPB_2BIT) ctx->base->US_MR |= US_MR_NBSTOP(2);

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
    if ((res = set_cs(ctx, settings->cs)) < 0)
        return res;

    /* parity */
    if (settings->par != UART_PAR_IGNORE &&
        (res = set_parity(ctx, settings->par)) < 0)
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

    int sent = 0;

    while ((size_t)sent != n) {
        if (fifo_head_is_writable(&ctx->tx_fifo)) {
            ctx->tx_buf[ctx->tx_fifo.w] = buf[sent++];
            fifo_head_push(&ctx->tx_fifo);
        }else
            break;
    }

    if (sent == 0)
        return -EAGAIN;

    /* trigger tx isr */
    ctx->base->US_IER = (uint32_t)US_CSR_TXEMPTY;
    return sent; /* FIXME */
}

int uart_read(struct uart *ctx, char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int recv = 0;

    while ((size_t)recv != n) {
        if (fifo_head_is_readable(&ctx->rx_fifo)) {
            fifo_head_pop(&ctx->rx_fifo);
            buf[recv++] = ctx->rx_buf[ctx->rx_fifo.r];
        }else
            break;
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

#include "uart-8051_irqdriven.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#ifndef S_SPLINT_S
__sfr __at(ADDR_PCON) PCON;
__sfr __at(ADDR_SCON) SCON;
__sfr __at(ADDR_TMOD) TMOD;
__sfr __at(ADDR_TCON) TCON;
__sfr __at(ADDR_TH1)  TH1;
__sfr __at(ADDR_SBUF) SBUF;
__sfr __at(ADDR_IP)   IP;
__sfr __at(ADDR_IPH)  IPH;
#else
static uint8_t PCON;
static uint8_t SCON;
static uint8_t TMOD;
static uint8_t TCON;
static uint8_t TH1;
static uint8_t SBUF;
static uint8_t IP;
static uint8_t IPH;
#endif

#define PCON_SMOD  (1 << 7)
#define PCON_SMOD0 (1 << 6)

#define SCON_SM_M  0x7u
#define SCON_SM(x) (((x) & SCON_SM_M) << 5)
#define SCON_REN   (1 << 4)
#define SCON_TB8   (1 << 3)
#define SCON_RB8   (1 << 2)
#define SCON_TI    (1 << 1)
#define SCON_RI    (1 << 0)

#define TMOD_GATE1 (1 << 7)
#define TMOD_CT1   (1 << 6)
#define TMOD_M1_M  0x3u
#define TMOD_M1(x) (((x) & TMOD_M1_M) << 4)

#define TCON_TR1 (1 << 6)

/* TODO: check if this is std */
#define IP_PS   (1 << 4)
#define IPH_PSH (1 << 4)

/* UART */

static void uart_isr(void *priv)
{
    struct uart *ctx = (struct uart*)priv;

    picoRTOS_assert(ctx->tx_buf != NULL, return );
    picoRTOS_assert(ctx->rx_buf != NULL, return );

    do {
        /* RX */
        if ((SCON & SCON_RI) != 0) {
            /* ack */
            SCON &= ~SCON_RI;
            /* read */
            if (fifo_head_is_writable(&ctx->rx_fifo)) {
                ctx->rx_buf[ctx->rx_fifo.w] = (char)SBUF;
                fifo_head_push(&ctx->rx_fifo);
            }
        }
        /* TX */
        if ((SCON & SCON_TI) != 0) {
            /* ack */
            SCON &= ~SCON_TI;
            /* write */
            if (fifo_head_is_readable(&ctx->tx_fifo)) {
                fifo_head_pop(&ctx->tx_fifo);
                SBUF = (uint8_t)ctx->tx_buf[ctx->tx_fifo.r];
            }else
                ctx->tx_trig = true;
        }
        /* double-check */
    } while((SCON & (SCON_TI | SCON_RI)) != 0);
}

int uart_8051_init(struct uart *ctx, clock_id_t clkid)
{
    ctx->clkid = clkid;
    ctx->mode = UART_8051_MODE_NORMAL;

    ctx->tx_trig = true;
    ctx->rx_buf = NULL;
    ctx->tx_buf = NULL;

    return 0;
}

int uart_8051_setup_irqdriven(struct uart *ctx, struct uart_8051_irqdriven_settings *settings)
{
    picoRTOS_assert((settings->rx_mask & (settings->rx_mask + 1)) == 0, return -EINVAL);
    picoRTOS_assert((settings->tx_mask & (settings->tx_mask + 1)) == 0, return -EINVAL);
    picoRTOS_assert(settings->prio < (picoRTOS_priority_t)4, return -EINVAL);
    picoRTOS_assert(settings->irq_ri_ti < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT,
                    return -EINVAL);

    /* buffers */
    fifo_head_init(&ctx->rx_fifo, settings->rx_mask);
    fifo_head_init(&ctx->tx_fifo, settings->tx_mask);
    ctx->rx_buf = settings->rx_buf;
    ctx->tx_buf = settings->tx_buf;

    /* register interrupt & enable */
    picoRTOS_register_interrupt(settings->irq_ri_ti, uart_isr, ctx);
    picoRTOS_enable_interrupt(settings->irq_ri_ti);

    /* set priority */
    if ((settings->prio & 0x1) != 0) IP |= IP_PS;
    else IP &= ~IP_PS;
    if ((settings->prio & 0x2) != 0) IPH |= IPH_PSH;
    else IPH &= ~IPH_PSH;

    ctx->mode = UART_8051_MODE_IRQDRIVEN;
    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baudrate)
{
#define div_round_closest(a, b) (((a) + ((b) >> 1)) / (b))

    clock_freq_t fsys = clock_get_freq(ctx->clkid);
    unsigned long num = (unsigned long)fsys / 16ul;

    picoRTOS_assert(baudrate > 0, return -EINVAL);
    picoRTOS_assert(fsys > 0, return -EIO);

    SCON = (uint8_t)(SCON_SM(2) | SCON_REN);
    PCON |= PCON_SMOD; /* double rate */

    /* t1, mode 2 */
    TMOD &= ~TMOD_M1(TMOD_M1_M);
    TMOD |= TMOD_M1(2);

    /*
     * (2^SMOD)/32 * Fsys / (256 - TH1)
     */
    TH1 = (uint8_t)(256ul - div_round_closest(num, baudrate));
    TCON |= TCON_TR1;   /* start timer */
    return 0;
}

int uart_setup(struct uart *ctx, const struct uart_settings *settings)
{
    int res;

    /* baudrate */
    if (settings->baudrate != 0 &&
        (res = set_baudrate(ctx, settings->baudrate)) < 0)
        return res;

    /* ignore parity */
    /* ignore cstopb */
    /* ignore cs */

    return 0;
}

static int uart_write_normal(struct uart *ctx, const char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int sent = 0;

    while ((size_t)sent != n) {
        if ((SCON & SCON_TI) == 0 && !ctx->tx_trig)
            break;

        SCON &= ~SCON_TI;
        SBUF = (uint8_t)buf[sent++];
        ctx->tx_trig = false;
    }

    if (sent == 0)
        return -EAGAIN;

    return sent;
}

static int uart_write_irqdriven(struct uart *ctx, const char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(ctx->tx_buf != NULL, return -EIO);

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

    /* deliberately trigger the isr */
    if (ctx->tx_trig) {
        ctx->tx_trig = false;
        SCON |= SCON_TI;
    }

    return sent;
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    switch (ctx->mode) {
    case UART_8051_MODE_NORMAL: return uart_write_normal(ctx, buf, n);
    case UART_8051_MODE_IRQDRIVEN: return uart_write_irqdriven(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int uart_read_normal(struct uart *ctx, char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    /*@i@*/ (void)ctx;

    int recv = 0;

    while ((size_t)recv != n) {
        if ((SCON & SCON_RI) == 0)
            break;

        SCON &= ~SCON_RI;
        buf[recv++] = (char)SBUF;
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

static int uart_read_irqdriven(struct uart *ctx, char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(ctx->rx_buf != NULL, return -EIO);

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

int uart_read(struct uart *ctx, char *buf, size_t n)
{
    switch (ctx->mode) {
    case UART_8051_MODE_NORMAL: return uart_read_normal(ctx, buf, n);
    case UART_8051_MODE_IRQDRIVEN: return uart_read_irqdriven(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

#include "uart-nxp_linflexd.h"
#include "picoRTOS.h"

#include <stdint.h>

struct UART_NXP_LINFLEXD {
    volatile uint32_t LINCR1;
    volatile uint32_t LINIER;
    volatile uint32_t LINSR;
    volatile uint32_t LINESR;
    volatile uint32_t UARTCR;
    volatile uint32_t UARTSR;
    volatile uint32_t LINTCSR;
    volatile uint32_t LINOCR;
    volatile uint32_t LINTOCR;
    volatile uint32_t LINFBRR;
    volatile uint32_t LINIBRR;
    volatile uint32_t LINCFR;
    volatile uint32_t LINCR2;
    volatile uint32_t BIDR;
    volatile uint32_t BDRL;
    volatile uint32_t BDRM;
    volatile uint32_t IFER;
    volatile uint32_t IFMI;
    volatile uint32_t IFMR;
    volatile uint32_t IFCR[16];
    volatile uint32_t GCR;
    volatile uint32_t UARTPTO;
    volatile uint32_t UARTCTO;
    volatile uint32_t DMATXE;
    volatile uint32_t DMARXE;
};

#define LINCR1_SLEEP  (1 << 1)
#define LINCR1_INIT   (1 << 0)

#define LINSR_LINS_M      0xfu
#define LINSR_LINS(x)     (((x) & LINSR_LINS_M) << 12)

#define LINFBRR_FBR_M  0xfu
#define LINFBRR_FBR(x) ((x) & LINFBRR_FBR_M)

#define LINIBRR_IBR_M  0xffffu
#define LINIBRR_IBR(x) ((x) & LINIBRR_IBR_M)

#define UARTCR_SBUR_M  0x3u
#define UARTCR_SBUR(x) (((x) & UARTCR_SBUR_M) << 17)
#define UARTCR_TDFL_TFC (1 << 13)
#define UARTCR_RDFL_RFC (1 << 10)
#define UARTCR_WL1      (1 << 7)
#define UARTCR_PC1      (1 << 6)
#define UARTCR_RXEN     (1 << 5)
#define UARTCR_TXEN     (1 << 4)
#define UARTCR_PC0      (1 << 3)
#define UARTCR_PCE      (1 << 2)
#define UARTCR_WL0      (1 << 1)
#define UARTCR_UART     (1 << 0)

#define UARTSR_RFNE   (1 << 4)
#define UARTSR_DRFRFE (1 << 2)
#define UARTSR_DTFTFF (1 << 1)

static int request_init_mode(struct uart *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* init */
    ctx->base->LINCR1 &= ~LINCR1_SLEEP;
    ctx->base->LINCR1 |= LINCR1_INIT;

    while (deadlock-- != 0)
        if ((ctx->base->LINSR & LINSR_LINS(LINSR_LINS_M)) == LINSR_LINS(1))
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

int uart_nxp_linflexd_init(struct uart *ctx, int base, clock_id_t clkid)
{
    int res;

    ctx->base = (struct UART_NXP_LINFLEXD*)base;
    ctx->clkid = clkid;
    ctx->state = UART_NXP_LINFLEXD_STATE_START;

    if ((res = request_init_mode(ctx)) < 0)
        return res;

    /* uart mode */
    ctx->base->UARTCR |= UARTCR_UART;

    /* 1-byte xfers */
    ctx->base->UARTCR &= ~UARTCR_TDFL_TFC;
    ctx->base->UARTCR &= ~UARTCR_RDFL_RFC;

    /* enable */
    ctx->base->UARTCR |= UARTCR_RXEN;
    ctx->base->UARTCR |= UARTCR_TXEN;

    /* exit init mode */
    ctx->base->LINCR1 &= ~LINCR1_INIT;

    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baudrate)
{
#define div_round_closest(a, b) (((a) + ((b) >> 1)) / (b))

    if (!picoRTOS_assert(baudrate > 0)) return -EINVAL;

    clock_freq_t freq;

    if ((freq = clock_get_freq(ctx->clkid)) < 0)
        return (int)freq;

    uint32_t divider = (uint32_t)(16u * baudrate);
    uint32_t ibr = (uint32_t)(freq / divider);
    uint32_t fbr = div_round_closest(((uint32_t)freq % divider) * 16, divider);

    /* integer part */
    ctx->base->LINIBRR &= ~LINIBRR_IBR(LINIBRR_IBR_M);
    ctx->base->LINIBRR |= LINIBRR_IBR(ibr);
    /* fractional part */
    ctx->base->LINFBRR = (uint32_t)LINFBRR_FBR(fbr);

    return 0;
}

static int set_parity(struct uart *ctx, bool parenb, bool parodd)
{
    if (parenb) ctx->base->UARTCR |= UARTCR_PCE;
    else ctx->base->UARTCR &= ~UARTCR_PCE;

    if (parodd) ctx->base->UARTCR |= UARTCR_PC0;
    else ctx->base->UARTCR &= ~UARTCR_PC0;

    return 0;
}

static int set_cs(struct uart *ctx, size_t cs, bool parenb)
{
    if (!picoRTOS_assert(cs > (size_t)6)) return -EINVAL;
    if (!picoRTOS_assert(cs < (size_t)9)) return -EINVAL;

    ctx->base->UARTCR &= ~UARTCR_WL1;

    switch (cs) {
    case 8:
        ctx->base->UARTCR |= UARTCR_WL0;
        break;

    case 7:
        if (!picoRTOS_assert(!parenb))
            return -EINVAL;

        ctx->base->UARTCR &= ~UARTCR_WL0;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return 0;
}

static int set_stopb(struct uart *ctx, bool stopb)
{
    ctx->base->UARTCR &= ~UARTCR_SBUR(UARTCR_SBUR_M);

    if (stopb)
        ctx->base->UARTCR |= UARTCR_SBUR(1);

    return 0;
}

int uart_setup(struct uart *ctx, const struct uart_settings *settings)
{
    int res;

    if ((res = request_init_mode(ctx)) < 0)
        return res;

    if ((res = set_baudrate(ctx, settings->baudrate)) < 0 ||
        (res = set_parity(ctx, settings->parenb, settings->parodd)) < 0 ||
        (res = set_cs(ctx, settings->cs, settings->parenb)) < 0 ||
        (res = set_stopb(ctx, settings->cstopb)) < 0)
        return res;

    ctx->base->LINCR1 &= ~LINCR1_INIT;
    return 0;
}

static int uart_write_start(struct uart *ctx, char c)
{
    ctx->base->BDRL = (uint32_t)0xffu & c;
    ctx->state = UART_NXP_LINFLEXD_STATE_XFER;

    return -EAGAIN;
}

static int uart_write_xfer(struct uart *ctx, const char *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    if ((ctx->base->UARTSR & UARTSR_DTFTFF) == 0)
        return -EAGAIN;

    ctx->base->UARTSR |= UARTSR_DTFTFF;
    ctx->base->BDRL = (uint32_t)0xffu & *buf;

    if (n == (size_t)1) {
        ctx->state = UART_NXP_LINFLEXD_STATE_STOP;
        return -EAGAIN;
    }

    return 1;
}

static int uart_write_stop(struct uart *ctx)
{
    if ((ctx->base->UARTSR & UARTSR_DTFTFF) == 0)
        return -EAGAIN;

    ctx->base->UARTSR |= UARTSR_DTFTFF;
    ctx->state = UART_NXP_LINFLEXD_STATE_START;

    return 1;
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case UART_NXP_LINFLEXD_STATE_START: return uart_write_start(ctx, *buf);
    case UART_NXP_LINFLEXD_STATE_XFER: return uart_write_xfer(ctx, buf, n);
    case UART_NXP_LINFLEXD_STATE_STOP: return uart_write_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int uart_read(struct uart *ctx, char *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    size_t recv = 0;

    while (recv != n) {
        if ((ctx->base->UARTSR & UARTSR_RFNE) == 0)
            break;

        buf[recv++] = (char)0xff & ctx->base->BDRL;
    }

    if (recv == 0)
        return -EAGAIN;

    return (int)recv;
}
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

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

/* Function: uart_nxp_linflexd_init
 * Initializes an UART on a LINFLexD interface
 *
 * Parameters:
 *  ctx - The UART to init
 *  base - The UART base address
 *  clkid - The UART clock ID
 *
 * Returns:
 * 0 if success, <0 error code otherwise
 */
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

    picoRTOS_assert(baudrate > 0, return -EINVAL);

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

static int set_parity(struct uart *ctx, uart_par_t par)
{
    picoRTOS_assert(par != UART_PAR_IGNORE, return -EINVAL);
    picoRTOS_assert(par < UART_PAR_COUNT, return -EINVAL);

    if (par != UART_PAR_NONE) ctx->base->UARTCR |= UARTCR_PCE;
    else ctx->base->UARTCR &= ~UARTCR_PCE;

    if (par == UART_PAR_ODD) ctx->base->UARTCR |= UARTCR_PC0;
    else ctx->base->UARTCR &= ~UARTCR_PC0;

    return 0;
}

static int set_cs(struct uart *ctx, size_t cs)
{
    picoRTOS_assert(cs > (size_t)6, return -EINVAL);
    picoRTOS_assert(cs < (size_t)9, return -EINVAL);

    ctx->base->UARTCR &= ~UARTCR_WL1;

    switch (cs) {
    case 8:
        ctx->base->UARTCR |= UARTCR_WL0;
        break;

    case 7:
        ctx->base->UARTCR &= ~UARTCR_WL0;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return 0;
}

static int set_cstopb(struct uart *ctx, uart_cstopb_t cstopb)
{
    picoRTOS_assert(cstopb != UART_CSTOPB_IGNORE, return -EINVAL);
    picoRTOS_assert(cstopb < UART_CSTOPB_COUNT, return -EINVAL);

    ctx->base->UARTCR &= ~UARTCR_SBUR(UARTCR_SBUR_M);

    if (cstopb == UART_CSTOPB_2BIT)
        ctx->base->UARTCR |= UARTCR_SBUR(1);

    return 0;
}

int uart_setup(struct uart *ctx, const struct uart_settings *settings)
{
    int res;

    if ((res = request_init_mode(ctx)) < 0)
        return res;

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

    ctx->base->LINCR1 &= ~LINCR1_INIT;
    return 0;
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    size_t sent = 0;

    while (sent != n) {
        /* first xfer exception */
        if (ctx->state == UART_NXP_LINFLEXD_STATE_START) {
            ctx->state = UART_NXP_LINFLEXD_STATE_XFER;
            ctx->base->BDRL = (uint32_t)0xffu & buf[sent++];
            continue;
        }

        if ((ctx->base->UARTSR & UARTSR_DTFTFF) == 0)
            return -EAGAIN;

        ctx->base->UARTSR |= UARTSR_DTFTFF;
        ctx->base->BDRL = (uint32_t)0xffu & buf[sent++];
    }

    if (sent == 0)
        return -EAGAIN;

    return (int)sent;
}

int uart_read(struct uart *ctx, char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    size_t recv = 0;

    while (recv != n) {
        if ((ctx->base->UARTSR & UARTSR_DRFRFE) == 0)
            break;

        ctx->base->UARTSR = (uint32_t)UARTSR_DRFRFE;
        buf[recv++] = (char)0xff & ctx->base->BDRM;
    }

    if (recv == 0)
        return -EAGAIN;

    return (int)recv;
}

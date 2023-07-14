#include "uart-sam3x.h"
#include "picoRTOS.h"

#include <stdint.h>

struct UART_SAM3X {
    volatile uint32_t UART_CR;
    volatile uint32_t UART_MR;
    volatile uint32_t UART_IER;
    volatile uint32_t UART_IDR;
    volatile uint32_t UART_IMR;
    volatile uint32_t UART_SR;
    volatile uint32_t UART_RHR;
    volatile uint32_t UART_THR;
    volatile uint32_t UART_BRGR;
};

#define UART_CR_RSTSTA (1 << 8)
#define UART_CR_TXDIS  (1 << 7)
#define UART_CR_TXEN   (1 << 6)
#define UART_CR_RXDIS  (1 << 5)
#define UART_CR_RXEN   (1 << 4)
#define UART_CR_RSTTX  (1 << 3)
#define UART_CR_RSTRX  (1 << 2)

#define UART_MR_CHMODE_M  0x3u
#define UART_MR_CHMODE(x) (((x) & UART_MR_CHMODE_M) << 14)
#define UART_MR_PAR_M     0x7u
#define UART_MR_PAR(x)    (((x) & UART_MR_PAR_M) << 9)

#define UART_SR_RXBUFF  (1 << 12)
#define UART_SR_TXBUFE  (1 << 11)
#define UART_SR_TXEMPTY (1 << 9)
#define UART_SR_PARE    (1 << 7)
#define UART_SR_FRAME   (1 << 6)
#define UART_SR_OVRE    (1 << 5)
#define UART_SR_ENDTX   (1 << 4)
#define UART_SR_ENDRX   (1 << 3)
#define UART_SR_TXRDY   (1 << 1)
#define UART_SR_RXRDY   (1 << 0)

#define UART_BRGR_CD_M  0xffffu
#define UART_BRGR_CD(x) ((x) & UART_BRGR_CD_M)

/* Function: uart_sam3x_init
 * Initializes an UART
 *
 * Parameters:
 *  ctx - The UART to init
 *  base - The UART base address
 *  clkid - the UART clock ID
 *
 * Returns:
 * Always 0
 */
int uart_sam3x_init(struct uart *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct UART_SAM3X*)base;
    ctx->clkid = clkid;

    /* reset */
    ctx->base->UART_CR = (uint32_t)(UART_CR_RSTSTA |
                                    UART_CR_RSTTX |
                                    UART_CR_RSTRX);
    /* enable */
    ctx->base->UART_CR = (uint32_t)(UART_CR_TXEN | UART_CR_RXEN);

    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baudrate)
{
    if (!picoRTOS_assert(baudrate > 0)) return -EINVAL;

    unsigned long cd;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return (int)freq;

    cd = (unsigned long)freq / (baudrate * 16ul);
    ctx->base->UART_BRGR = (uint32_t)UART_BRGR_CD(cd);

    return 0;
}

static int set_parity(struct uart *ctx, uart_par_t par)
{
    if (!picoRTOS_assert(par != UART_PAR_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(par < UART_PAR_COUNT)) return -EINVAL;

    ctx->base->UART_MR &= ~UART_MR_PAR(UART_MR_PAR_M);

    if (par == UART_PAR_NONE) {
        ctx->base->UART_MR |= UART_MR_PAR(4);
        return 0;
    }

    if (par == UART_PAR_ODD) ctx->base->UART_MR |= UART_MR_PAR(1);
    else ctx->base->UART_MR |= UART_MR_PAR(0);

    return 0;
}

int uart_setup(struct uart *ctx, const struct uart_settings *settings)
{
    int res;

    /* baudrate */
    if (settings->baudrate != 0 &&
        (res = set_baudrate(ctx, settings->baudrate)) < 0)
        return res;

    /* parity */
    if (settings->par != UART_PAR_IGNORE &&
        (res = set_parity(ctx, settings->par)) < 0)
        return res;

    /* ignore cs and stopb */

    return 0;
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    int sent = 0;

    while ((size_t)sent != n) {
        if ((ctx->base->UART_SR & UART_SR_TXRDY) == 0)
            break;

        ctx->base->UART_THR = (uint32_t)buf[sent++];
    }

    if (sent == 0)
        return -EAGAIN;

    return sent;
}

int uart_read(struct uart *ctx, char *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    int recv = 0;

    while ((size_t)recv != n) {
        if ((ctx->base->UART_SR & UART_SR_RXRDY) == 0)
            break;

        buf[recv++] = (char)ctx->base->UART_RHR;
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

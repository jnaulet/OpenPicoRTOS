#include "uart-arm_pl011.h"
#include "picoRTOS.h"

#include <stdint.h>

struct UART_ARM_PL011 {
    volatile uint32_t UARTDR;
    volatile uint32_t UARTRSR;
    uint32_t RESERVED0[5];
    volatile uint32_t UARTFR;
    volatile uint32_t UARTILPR;
    volatile uint32_t UARTIBRD;
    volatile uint32_t UARTFBRD;
    volatile uint32_t UARTLCR_H;
    volatile uint32_t UARTCR;
    volatile uint32_t UARTIFLS;
    volatile uint32_t UARTIMSC;
    volatile uint32_t UARTRIS;
    volatile uint32_t UARTMIS;
    volatile uint32_t UARTICR;
    volatile uint32_t UARTDMACR;
    uint32_t RESERVED1[998];
    volatile uint32_t UARTPERIPHID0;
    volatile uint32_t UARTPERIPHID1;
    volatile uint32_t UARTPERIPHID2;
    volatile uint32_t UARTPERIPHID3;
    volatile uint32_t UARTPCELLID0;
    volatile uint32_t UARTPCELLID1;
    volatile uint32_t UARTPCELLID2;
    volatile uint32_t UARTPCELLID3;
};

#define UARTDR_OE      (1 << 11)
#define UARTDR_BE      (1 << 10)
#define UARTDR_PE      (1 << 9)
#define UARTDR_FE      (1 << 8)
#define UARTDR_DATA_M  0xffu
#define UARTDR_DATA(x) ((x) & UARTDR_DATA_M)

#define UARTSR_OE (1 << 3)
#define UARTSR_BE (1 << 2)
#define UARTSR_PE (1 << 1)
#define UARTSR_FE (1 << 0)

#define UARTFR_RI   (1 << 8)
#define UARTFR_TXFE (1 << 7)
#define UARTFR_RXFF (1 << 6)
#define UARTFR_TXFF (1 << 5)
#define UARTFR_RXFE (1 << 4)
#define UARTFR_BUSY (1 << 3)
#define UARTFR_DCD  (1 << 2)
#define UARTFR_DSR  (1 << 1)
#define UARTFR_CTS  (1 << 0)

#define UARTILPR_ILPDVSR_M  0xffu
#define UARTILPR_ILPDVSR(x) ((x) & UARTILPR_ILPDVSR_M)

#define UARTIBRD_BAUD_DIVINT_M 0xffffu
#define UARTIBRD_BAUD_DIVINT(x) ((x) & UARTIBRD_BAUD_DIVINT_M)

#define UARTFBRD_BAUD_DIVFRAC_M  0x3fu
#define UARTFBRD_BAUD_DIVFRAC(x) ((x) & UARTFBRD_BAUD_DIVFRAC_M)

#define UARTLCR_H_SPS     (1 << 7)
#define UARTLCR_H_WLEN_M  0x3u
#define UARTLCR_H_WLEN(x) (((x) & UARTLCR_H_WLEN_M) << 5)
#define UARTLCR_H_FEN     (1 << 4)
#define UARTLCR_H_STP2    (1 << 3)
#define UARTLCR_H_EPS     (1 << 2)
#define UARTLCR_H_PEN     (1 << 1)
#define UARTLCR_H_BRK     (1 << 0)

#define UARTCR_CTSEN  (1 << 15)
#define UARTCR_RTSEN  (1 << 14)
#define UARTCR_OUT2   (1 << 13)
#define UARTCR_OUT1   (1 << 12)
#define UARTCR_RTS    (1 << 11)
#define UARTCR_DTR    (1 << 10)
#define UARTCR_RXE    (1 << 9)
#define UARTCR_TXE    (1 << 8)
#define UARTCR_LBE    (1 << 7)
#define UARTCR_SIRLP  (1 << 2)
#define UARTCR_SIREN  (1 << 1)
#define UARTCR_UARTEN (1 << 0)

/* Function: uart_arm_pl011_init
 * Initializes an UART
 *
 * Parameters:
 *  ctx - The UART to init
 *  base - The UART base address
 *  clkid - The UART clock ID
 *
 * Returns:
 * Always 0
 */
int uart_arm_pl011_init(struct uart *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct UART_ARM_PL011*)base;
    ctx->clkid = clkid;

    /* turn on fifos */
    ctx->base->UARTLCR_H = (uint32_t)UARTLCR_H_FEN;

    /* turn on transmission */
    ctx->base->UARTCR = (uint32_t)(UARTCR_RXE | UARTCR_TXE | UARTCR_UARTEN);

    return 0;
}

/* Function: uart_arm_pl011_set_loopback
 * Sets UART for loopback mode (test)
 *
 * Parameters:
 *  ctx - An UART instance
 *  loopback - true or false
 *
 * Returns:
 * Always 0
 */
int uart_arm_pl011_set_loopback(struct uart *ctx, bool loopback)
{
    if (loopback) ctx->base->UARTCR |= UARTCR_LBE;
    else ctx->base->UARTCR &= ~UARTCR_LBE;

    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baudrate)
{
    if (!picoRTOS_assert(baudrate > 0)) return -EINVAL;

    unsigned long modulo;
    unsigned long divider = 16ul * baudrate;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return (int)freq;

    modulo = (unsigned long)freq % divider;
    ctx->base->UARTIBRD = (uint32_t)UARTIBRD_BAUD_DIVINT((unsigned long)freq / divider);
    ctx->base->UARTFBRD = (uint32_t)UARTFBRD_BAUD_DIVFRAC((modulo * 100) / divider);

    return 0;
}

static int set_cs(struct uart *ctx, size_t cs)
{
    if (!picoRTOS_assert(cs >= (size_t)UART_ARM_PL011_CS_MIN)) return -EINVAL;
    if (!picoRTOS_assert(cs <= (size_t)UART_ARM_PL011_CS_MAX)) return -EINVAL;

    ctx->base->UARTLCR_H &= ~UARTLCR_H_WLEN(UARTLCR_H_WLEN_M);
    ctx->base->UARTLCR_H |= UARTLCR_H_WLEN(cs - (size_t)5);

    return 0;
}

static int set_parity(struct uart *ctx, uart_par_t par)
{
    if (!picoRTOS_assert(par != UART_PAR_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(par < UART_PAR_COUNT)) return -EINVAL;

    /* no parity */
    if (par == UART_PAR_NONE) {
        ctx->base->UARTLCR_H &= ~UARTLCR_H_PEN;
        return 0;
    }

    /* parity */
    ctx->base->UARTLCR_H |= UARTLCR_H_PEN;
    if (par == UART_PAR_ODD) ctx->base->UARTLCR_H &= ~UARTLCR_H_EPS;
    else ctx->base->UARTLCR_H |= UARTLCR_H_EPS;

    return 0;
}

static int set_cstopb(struct uart *ctx, uart_cstopb_t cstopb)
{
    if (!picoRTOS_assert(cstopb != UART_CSTOPB_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(cstopb < UART_CSTOPB_COUNT)) return -EINVAL;

    if (cstopb == UART_CSTOPB_2BIT) ctx->base->UARTLCR_H |= UARTLCR_H_STP2;
    else ctx->base->UARTLCR_H &= ~UARTLCR_H_STP2;

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
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    int sent = 0;

    while ((size_t)sent != n) {
        if ((ctx->base->UARTFR & UARTFR_TXFF) != 0)
            break;

        ctx->base->UARTDR = (uint32_t)UARTDR_DATA(buf[sent++]);
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
        if ((ctx->base->UARTFR & UARTFR_RXFE) != 0)
            break;

        buf[recv++] = (char)(0xff & ctx->base->UARTDR);
    }

    return recv;
}

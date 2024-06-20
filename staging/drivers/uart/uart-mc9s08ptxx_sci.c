#include "uart-mc9s08ptxx_sci.h"
#include "picoRTOS.h"

#include <stdint.h>

struct UART_MC9S08PTXX_SCI {
    volatile uint8_t BDH;
    volatile uint8_t BDL;
    volatile uint8_t C1;
    volatile uint8_t C2;
    volatile uint8_t S1;
    volatile uint8_t S2;
    volatile uint8_t C3;
    volatile uint8_t D;
};

#define BDH_LBKDIE  (1 << 7)
#define BDH_RXEDGIE (1 << 6)
#define BDH_SBNS    (1 << 5)
#define BDH_SBR_M   0x1fu
#define BDH_SBR(x)  ((x) & BDH_SBR_M)

#define BDL_SBR_M  0xffu
#define BDL_SBR(x) ((x) & BDL_SBR_M)

#define C1_LOOPS   (1 << 7)
#define C1_SCISWAI (1 << 6)
#define C1_RSRC    (1 << 5)
#define C1_M       (1 << 4)
#define C1_WAKE    (1 << 3)
#define C1_ILT     (1 << 2)
#define C1_PE      (1 << 1)
#define C1_PT      (1 << 0)

#define C2_TIE  (1 << 7)
#define C2_TCIE (1 << 6)
#define C2_RIE  (1 << 5)
#define C2_ILIE (1 << 4)
#define C2_TE   (1 << 3)
#define C2_RE   (1 << 2)
#define C2_RWU  (1 << 1)
#define C2_SBK  (1 << 0)

#define S1_TDRE (1 << 7)
#define S1_TC   (1 << 6)
#define S1_RDRF (1 << 5)
#define S1_IDLE (1 << 4)
#define S1_OR   (1 << 3)
#define S1_NF   (1 << 2)
#define S1_FE   (1 << 1)
#define S1_PF   (1 << 0)

#define S2_LBKDIF  (1 << 7)
#define S2_RXEDGIF (1 << 6)
#define S2_RXINV   (1 << 4)
#define S2_RWUID   (1 << 3)
#define S2_BRK13   (1 << 2)
#define S2_LBKDE   (1 << 1)
#define S2_RAF     (1 << 0)

#define C3_R8    (1 << 7)
#define C3_T8    (1 << 6)
#define C3_TXDIR (1 << 5)
#define C3_TXINV (1 << 4)
#define C3_ORIE  (1 << 3)
#define C3_NEIE  (1 << 2)
#define C3_FEIE  (1 << 1)
#define C3_PEIE  (1 << 0)

int uart_mc9s08ptxx_sci_init(struct uart *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct UART_MC9S08PTXX_SCI*)base;
    ctx->clkid = clkid;

    /* turn on */
    ctx->base->C2 |= (C2_TE | C2_RE);
    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baud)
{
    picoRTOS_assert(baud > 0, return -EINVAL);

    clock_freq_t freq = clock_get_freq(ctx->clkid);
    unsigned long sbr = (unsigned long)freq / 16ul / baud;

    picoRTOS_assert(freq > 0, return -EIO);

    ctx->base->BDH &= ~BDH_SBR(BDH_SBR_M);
    ctx->base->BDH |= BDH_SBR(sbr >> 8);
    ctx->base->BDL = (uint8_t)BDL_SBR(sbr);

    /* TODO: check freq match */
    return 0;
}

static int set_parity(struct uart *ctx, uart_par_t par)
{
    picoRTOS_assert(par != UART_PAR_IGNORE, return -EINVAL);
    picoRTOS_assert(par < UART_PAR_COUNT, return -EINVAL);

    /* disable */
    ctx->base->C1 &= ~C1_PE;
    if (par == UART_PAR_NONE)
        return 0;

    if (par == UART_PAR_ODD) ctx->base->C1 |= (C1_PE | C1_PT);
    else ctx->base->C2 |= C1_PE;

    return 0;
}

/* HAL */

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

    /* ignore cs */
    /* ignore cstopb */

    return 0;
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    size_t nwritten = 0;

    while (nwritten != n) {

        if ((ctx->base->S1 & S1_TDRE) == 0)
            break;

        ctx->base->D = (uint8_t)*buf++;
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

        if ((ctx->base->S1 & S1_RDRF) == 0)
            break;

        *buf++ = (char)ctx->base->D;
        nread++;
    }

    if (nread == 0)
        return -EAGAIN;

    return (int)nread;
}

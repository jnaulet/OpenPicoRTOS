#include "uart-pic32mx.h"
#include "picoRTOS.h"

#include <stdint.h>
#include "misc-pic32mx.h"

struct UART_PIC32MX {
    struct PIC32MX_CLR_SET_INV UxMODE;
    struct PIC32MX_CLR_SET_INV UxSTA;
    struct PIC32MX_CLR_SET_INV UxTXREG;
    struct PIC32MX_CLR_SET_INV UxRXREG;
    struct PIC32MX_CLR_SET_INV UxBRG;
};

#define UxMODE_ON       (1 << 15)
#define UxMODE_SIDL     (1 << 13)
#define UxMODE_IREN     (1 << 12)
#define UxMODE_RTSMD    (1 << 11)
#define UxMODE_UEN_M    0x3u
#define UxMODE_UEN(x)   (((x) & UxMODE_UEN_M) << 8)
#define UxMODE_WAKE     (1 << 7)
#define UxMODE_LPBACK   (1 << 6)
#define UxMODE_ABAUD    (1 << 5)
#define UxMODE_RXINV    (1 << 4)
#define UxMODE_BRGH     (1 << 3)
#define UxMODE_PDSEL_M  0x3u
#define UxMODE_PDSEL(x) (((x) & UxMODE_PDSEL_M) << 1)
#define UxMODE_STSEL    (1 << 0)

#define UxSTA_ADM_EN     (1 << 24)
#define UxSTA_ADDR_M     0xffu
#define UxSTA_ADDR(x)    (((x) & UxSTA_ADDR_M) << 16)
#define UxSTA_UTXISEL_M  0x3u
#define UxSTA_UTXISEL(x) (((x) & UxSTA_UTXISEL_M) << 14)
#define UxSTA_UTXINV     (1 << 13)
#define UxSTA_URXEN      (1 << 12)
#define UxSTA_UTXBRK     (1 << 11)
#define UxSTA_UTXEN      (1 << 10)
#define UxSTA_UTXBF      (1 << 9)
#define UxSTA_UTRMT      (1 << 8)
#define UxSTA_URXISEL_M  0x3u
#define UxSTA_URXISEL(x) (((x) & UxSTA_URXISEL_M) << 6)
#define UxSTA_ADDEN      (1 << 5)
#define UxSTA_RIDLE      (1 << 4)
#define UxSTA_PERR       (1 << 3)
#define UxSTA_FERR       (1 << 2)
#define UxSTA_OERR       (1 << 1)
#define UxSTA_URXDA      (1 << 0)

/* Function: uart_pic32mx_init
 * Initializes an UART
 *
 * Parameters:
 *  ctx - The UART context to init
 *  base - The UART module base address
 *  clkid - The UART input clock ID
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int uart_pic32mx_init(struct uart *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct UART_PIC32MX*)base;
    ctx->clkid = clkid;

    /* turn on */
    ctx->base->UxMODE.SET = (uint32_t)UxMODE_ON;
    ctx->base->UxSTA.SET = (uint32_t)(UxSTA_URXEN | UxSTA_UTXEN);

    /* debug */
    /* ctx->base->UxMODE.SET = (uint32_t)UxMODE_LPBACK; */

    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baudrate)
{
    if (!picoRTOS_assert(baudrate > 0)) return -EINVAL;

    unsigned long brg;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return (int)freq;

    brg = (unsigned long)freq / (baudrate * 16ul) - 1ul;
    ctx->base->UxBRG.REG = (uint32_t)brg;

    return 0;
}

static int set_parity(struct uart *ctx, bool parenb, bool parodd)
{
    ctx->base->UxMODE.CLR = (uint32_t)UxMODE_PDSEL(UxMODE_PDSEL_M);

    if (parenb) {
        if (parodd) ctx->base->UxMODE.SET = (uint32_t)UxMODE_PDSEL(0x2);
        else ctx->base->UxMODE.SET = (uint32_t)UxMODE_PDSEL(0x1);
    }

    return 0;
}

static int set_stopb(struct uart *ctx, bool cstopb)
{
    if (cstopb) ctx->base->UxMODE.SET = (uint32_t)UxMODE_STSEL;
    else ctx->base->UxMODE.CLR = (uint32_t)UxMODE_STSEL;

    return 0;
}

int uart_setup(struct uart *ctx, const struct uart_settings *settings)
{
    int res;

    if ((res = set_baudrate(ctx, settings->baudrate)) < 0 ||
        (res = set_parity(ctx, settings->parenb, settings->parodd)) < 0 ||
        (res = set_stopb(ctx, settings->cstopb)) < 0)
        return res;

    /* ignore cs */

    return 0;
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    int sent = 0;

    while ((size_t)sent != n) {
        if ((ctx->base->UxSTA.REG & UxSTA_UTXBF) != 0)
            break;

        ctx->base->UxTXREG.REG = (uint32_t)buf[sent++];
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
        if ((ctx->base->UxSTA.REG & UxSTA_URXDA) == 0)
            break;

        buf[recv++] = (char)ctx->base->UxRXREG.REG;
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

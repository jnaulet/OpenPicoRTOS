#include "uart-8051_irqdriven.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_SCON)   SCON;
__sfr __at(ADDR_TMOD)   TMOD;
__sfr __at(ADDR_TCON)   TCON;
__sfr __at(ADDR_TH1)    TH1;
__sfr __at(ADDR_SBUF)   SBUF;
#else
static unsigned char SCON;
static unsigned char TMOD;
static unsigned char TCON;
static unsigned char TH1;
static unsigned char SBUF;
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

int uart_8051_init(struct uart *ctx, clock_id_t clkid)
{
    ctx->clkid = clkid;
    ctx->mode = UART_8051_MODE_NORMAL;
    ctx->tx_trig = true;

    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baudrate)
{
#define div_round_closest(a, b) (((a) + ((b) >> 1)) / (b))

    clock_freq_t fsys = clock_get_freq(ctx->clkid);
    unsigned long num = (unsigned long)fsys / 32ul;

    picoRTOS_assert(baudrate > 0, return -EINVAL);
    picoRTOS_assert(fsys > 0, return -EIO);

    SCON = (unsigned char)(SCON_SM(2) | SCON_REN);

    /* t1, mode 2 */
    TMOD &= ~TMOD_M1(TMOD_M1_M);
    TMOD |= TMOD_M1(2);

    /*
     * (2^SMOD)/32 * Fsys / (256 - TH1)
     */
    TH1 = (unsigned char)(256ul - div_round_closest(num, baudrate));
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

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int sent = 0;

    while ((size_t)sent != n) {
        if ((SCON & SCON_TI) == (unsigned char)0 && !ctx->tx_trig)
            break;

        SCON &= ~SCON_TI;
        SBUF = (unsigned char)*buf++;

        ctx->tx_trig = false;
        sent++;
    }

    if (sent == 0)
        return -EAGAIN;

    return sent;
}

int uart_read(struct uart *ctx, char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    /*@i@*/ (void)ctx;

    int recv = 0;

    while ((size_t)recv != n) {

        if ((SCON & SCON_RI) == (unsigned char)0)
            break;

        *buf++ = (char)SBUF;
        recv++;

        /* ack */
        SCON &= ~SCON_RI;
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

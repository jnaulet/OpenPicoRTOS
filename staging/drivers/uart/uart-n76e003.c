#include "uart-n76e003.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_PCON)   PCON;
__sfr __at(ADDR_SCON)   SCON;
__sfr __at(ADDR_SCON_1) SCON_1;
__sfr __at(ADDR_TMOD)   TMOD;
__sfr __at(ADDR_TCON)   TCON;
__sfr __at(ADDR_CKCON)  CKCON;
__sfr __at(ADDR_TH1)    TH1;
__sfr __at(ADDR_T3CON)  T3CON;
__sfr __at(ADDR_RL3)    RL3;
__sfr __at(ADDR_RH3)    RH3;
__sfr __at(ADDR_SBUF)   SBUF;
__sfr __at(ADDR_SBUF_1) SBUF_1;
#else
static unsigned char PCON;
static unsigned char SCON;
# ifdef CONFIG_UART_N76E003_UART1
static unsigned char SCON_1;
# endif
static unsigned char TMOD;
static unsigned char TCON;
static unsigned char CKCON;
static unsigned char TH1;
static unsigned char T3CON;
static unsigned char RL3;
static unsigned char RH3;
static unsigned char SBUF;
# ifdef CONFIG_UART_N76E003_UART1
static unsigned char SBUF_1;
# endif
#endif

#define PCON_SMOD  (1 << 7)
#define PCON_SMOD0 (1 << 6)

#define SCONx_SM_M  0x7u
#define SCONx_SM(x) (((x) & SCONx_SM_M) << 5)
#define SCONx_REN   (1 << 4)
#define SCONx_TB8   (1 << 3)
#define SCONx_RB8   (1 << 2)
#define SCONx_TI    (1 << 1)
#define SCONx_RI    (1 << 0)

#define TMOD_T1_GATE (1 << 7)
#define TMOD_T1_CT   (1 << 6)
#define TMOD_T1_M1   (1 << 5)
#define TMOD_T1_M0   (1 << 4)

#define TCON_TR1 (1 << 6)

#define CKCON_T1M (1 << 4)
#define CKCON_T0M (1 << 3)

#define T3CON_SMOD_1  (1 << 7)
#define T3CON_SMOD0_1 (1 << 6)
#define T3CON_BRCK    (1 << 5)
#define T3CON_TF3     (1 << 4)
#define T3CON_TR3     (1 << 3)
#define T3CON_T3PS_M  0x7u
#define T3CON_T3PS(x) ((x) & T3CON_T3PS_M)

int uart_n76e003_init(/*@out@*/ struct uart *ctx, uart_n76e003_t uart)
{
    picoRTOS_assert(uart < UART_N76E003_COUNT, return -EINVAL);

    ctx->uart = uart;
    ctx->first_tx = true;

    return 0;
}

static int set_baudrate(const struct uart *ctx, unsigned long baudrate)
{
    picoRTOS_assert(baudrate > 0, return -EINVAL);

    clock_freq_t freq = clock_get_freq(CLOCK_SYSCLK);
    unsigned long num = (unsigned long)freq / 16ul;

    picoRTOS_assert(freq > 0, return (int)freq);
    picoRTOS_assert(num > 0, return -EIO);

    /* UART0 */

    if (ctx->uart < UART_N76E003_UART1_T3) {
        SCON = (unsigned char)(SCONx_SM(2) | SCONx_REN);
        PCON |= PCON_SMOD;                /* double rate */
        /* if T1 */
        if (ctx->uart == UART_N76E003_UART0_T1) {
            TMOD |= TMOD_T1_M1;     /* mode 2 */
            TMOD &= ~TMOD_T1_M0;
            CKCON |= CKCON_T1M;     /* t1 is sysclk */
            T3CON &= ~T3CON_BRCK;   /* t1 is baudrate clock source */
            /* for timer1:
             * (2^SMOD)/32 * Fsys / (12 * 256 - TH1) or
             * (2^SMOD)/32 * Fsys / (256 - TH1)
             */
            TH1 = (unsigned char)(256ul - (num / baudrate));
            TCON |= TCON_TR1;   /* start timer */
            return 0;
        }
        /* if T3 */
        if (ctx->uart == UART_N76E003_UART0_T3) {
            T3CON |= T3CON_BRCK;
            T3CON &= ~T3CON_T3PS(T3CON_T3PS_M); /* prescaler = 0 */
            /* for timer3:
             * (2^SMOD)/32 * Fsys / (prescale * (65536 - P3))
             */
            RH3 = (unsigned char)((65536ul - (num / baudrate)) >> 8);
            RL3 = (unsigned char)(65536ul - (num / baudrate));
            T3CON |= T3CON_TR3;             /* start timer */
            return 0;
        }

        /* shouldn't end up here */
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

#ifdef CONFIG_UART_N76E003_UART1
    /* UART 1 */

    SCON_1 = (unsigned char)(SCONx_SM(2) | SCONx_REN);
    T3CON = (unsigned char)T3CON_TF3;
    /* for timer3:
     * (2^SMOD)/32 * Fsys / (prescale * (65536 - P3))
     */
    RH3 = (unsigned char)((65536ul - (num / baudrate)) >> 8);
    RL3 = (unsigned char)(65536ul - (num / baudrate));
    T3CON |= T3CON_TR3; /* start timer */
#endif
    return 0;
}

/* cppcheck-suppress [constParameterPointer] */
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

static int uart0_write(struct uart *ctx, char c)
{
    /* if tx succeeded or it's the first frame we send */
    if ((SCON & SCONx_TI) != (unsigned char)0 || ctx->first_tx) {
        ctx->first_tx = false;
        SCON &= ~SCONx_TI; /* ack */
        SBUF = (unsigned char)c;
        return 1;
    }

    return -EAGAIN;
}

#ifdef CONFIG_UART_N76E003_UART1
static int uart1_write(struct uart *ctx, char c)
{
    /* if tx succeeded or it's the first frame we send */
    if ((SCON_1 & SCONx_TI) != (unsigned char)0 || ctx->first_tx) {
        ctx->first_tx = false;
        SCON_1 &= ~SCONx_TI; /* ack */
        SBUF_1 = (unsigned char)c;
        return 1;
    }

    return -EAGAIN;
}
#endif

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int sent = 0;

    while (sent != (int)n) {
        int res;
        /* according to ctx */
        switch (ctx->uart) {
        case UART_N76E003_UART0_T1: /*@fallthrough@*/
        case UART_N76E003_UART0_T3: res = uart0_write(ctx, *buf); break;
#ifdef CONFIG_UART_N76E003_UART1
        case UART_N76E003_UART1_T3: res = uart1_write(ctx, *buf); break;
#endif
        default:
            picoRTOS_break();
            /*@notreached@*/ return -EIO;
        }

        if (res < 0)
            break;

        sent++;
    }

    if (sent == 0)
        return -EAGAIN;

    return sent;
}

static int uart0_read(char *c)
{
    if ((SCON & SCONx_RI) != (unsigned char)0) {
        *c = (char)SBUF;
        SCON &= ~SCONx_RI; /* ack */
        return 1;
    }

    return -EAGAIN;
}

#ifdef CONFIG_UART_N76E003_UART1
static int uart1_read(char *c)
{
    if ((SCON_1 & SCONx_RI) != (unsigned char)0) {
        *c = (char)SBUF_1;
        SCON_1 &= ~SCONx_RI; /* ack */
        return 1;
    }

    return -EAGAIN;
}
#endif

/* cppcheck-suppress [constParameterPointer] */
int uart_read(struct uart *ctx, char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int recv = 0;

    while (recv != (int)n) {
        int res;
        /* according to ctx */
        switch (ctx->uart) {
        case UART_N76E003_UART0_T1: /*@fallthrough@*/
        case UART_N76E003_UART0_T3: res = uart0_read(buf); break;
#ifdef CONFIG_UART_N76E003_UART1
        case UART_N76E003_UART1_T3: res = uart1_read(buf); break;
#endif
        default:
            picoRTOS_break();
            /*@notreached@*/ return -EIO;
        }

        if (res < 0)
            break;

        recv++;
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

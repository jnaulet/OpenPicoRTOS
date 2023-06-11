#include "uart-gd32vf103.h"
#include "picoRTOS.h"

#include <stdint.h>

struct UART_GD32VF103_USART {
    volatile uint32_t USART_STAT;
    volatile uint32_t USART_DATA;
    volatile uint32_t USART_BAUD;
    volatile uint32_t USART_CTL0;
    volatile uint32_t USART_CTL1;
    volatile uint32_t USART_CTL2;
    volatile uint32_t USART_GP;
};

#define USART_STAT_CTSF  (1 << 9)
#define USART_STAT_LBDF  (1 << 8)
#define USART_STAT_TBE   (1 << 7)
#define USART_STAT_TC    (1 << 6)
#define USART_STAT_RBNE  (1 << 5)
#define USART_STAT_ILDEF (1 << 4)
#define USART_STAT_ORERR (1 << 3)
#define USART_STAT_NERR  (1 << 2)
#define USART_STAT_FERR  (1 << 1)
#define USART_STAT_PERR  (1 << 0)

#define USART_BAUD_INTDIV_M  0xfffu
#define USART_BAUD_INTDIV(x) (((x) & USART_BAUD_INTDIV_M) << 4)
#define USART_BAUD_FRADIV_M  0xfu
#define USART_BAUD_FRADIV(x) ((x) & USART_BAUD_FRADIV_M)

#define USART_CTL0_UEN    (1 << 13)
#define USART_CTL0_WL     (1 << 12)
#define USART_CTL0_WM     (1 << 11)
#define USART_CTL0_PCEN   (1 << 10)
#define USART_CTL0_PM     (1 << 9)
#define USART_CTL0_PERRIE (1 << 8)
#define USART_CTL0_TBEIE  (1 << 7)
#define USART_CTL0_TCIE   (1 << 6)
#define USART_CTL0_RBNEIE (1 << 5)
#define USART_CTL0_IDLEIE (1 << 4)
#define USART_CTL0_TEN    (1 << 3)
#define USART_CTL0_REN    (1 << 2)
#define USART_CTL0_RWU    (1 << 1)
#define USART_CTL0_SBKCMD (1 << 0)

#define USART_CTL1_LMEN    (1 << 14)
#define USART_CTL1_STB_M   0x3u
#define USART_CTL1_STB(x)  (((x) & USART_CTL1_STB_M) << 12)
#define USART_CTL1_CKEN    (1 << 11)
#define USART_CTL1_CPL     (1 << 10)
#define USART_CTL1_CPH     (1 << 9)
#define USART_CTL1_CLEN    (1 << 8)
#define USART_CTL1_LBDIE   (1 << 6)
#define USART_CTL1_LBLEN   (1 << 5)
#define USART_CTL1_ADDR_M  0xfu
#define USART_CTL1_ADDR(x) ((x) & USART_CTL1_ADDR_M)

#define USART_CTL2_CTSIE (1 << 10)
#define USART_CTL2_CTSEN (1 << 9)
#define USART_CTL2_RTSEN (1 << 8)
#define USART_CTL2_DENT  (1 << 7)
#define USART_CTL2_CENR  (1 << 6)
#define USART_CTL2_SCEN  (1 << 5)
#define USART_CTL2_NKEN  (1 << 4)
#define USART_CTL2_HDEN  (1 << 3)
#define USART_CTL2_IRLP  (1 << 2)
#define USART_CTL2_IREN  (1 << 1)
#define USART_CTL2_ERRIE (1 << 0)

#define USART_GP_GUAT_M  0xffu
#define USART_GP_GUAT(x) (((x) & USART_GP_GUAT_M) << 8)
#define USART_GP_PSC_M   0xffu
#define USART_GP_PSC(x)  ((x) & USART_GP_PSC_M)

/* Function: uart_gd32vf103_init
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
int uart_gd32vf103_init(struct uart *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct UART_GD32VF103_USART*)base;
    ctx->clkid = clkid;

    /* turn on */
    ctx->base->USART_CTL0 = (uint32_t)(USART_CTL0_UEN |
                                       USART_CTL0_TEN |
                                       USART_CTL0_REN);

    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baudrate)
{
    if (!picoRTOS_assert(baudrate > 0)) return -EINVAL;

    unsigned long intdiv;
    unsigned long modulo;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return (int)freq;

    intdiv = (unsigned long)freq / (16ul * baudrate);
    modulo = (unsigned long)freq % (16ul * baudrate);
    ctx->base->USART_BAUD = (uint32_t)(USART_BAUD_INTDIV(intdiv) |
                                       USART_BAUD_FRADIV((modulo * 16) / intdiv));

    return 0;
}

static int set_cs(struct uart *ctx, size_t cs)
{
    if (!picoRTOS_assert(cs >= (size_t)UART_GD32VF103_CS_MIN)) return -EINVAL;
    if (!picoRTOS_assert(cs <= (size_t)UART_GD32VF103_CS_MAX)) return -EINVAL;

    if (cs == (size_t)UART_GD32VF103_CS_MAX) ctx->base->USART_CTL0 |= USART_CTL0_WL;
    else ctx->base->USART_CTL0 &= ~USART_CTL0_WL;

    return 0;
}

static int set_parity(struct uart *ctx, bool parenb, bool parodd)
{
    ctx->base->USART_CTL0 &= ~USART_CTL0_PCEN;

    if (parenb) {
        ctx->base->USART_CTL0 |= USART_CTL0_PCEN;
        if (parodd) ctx->base->USART_CTL0 |= USART_CTL0_PM;
        else ctx->base->USART_CTL0 &= ~USART_CTL0_PM;
    }

    return 0;
}

static int set_stopb(struct uart *ctx, bool cstopb)
{
    ctx->base->USART_CTL1 &= ~USART_CTL1_STB(USART_CTL1_STB_M);

    if (cstopb)
        ctx->base->USART_CTL1 |= USART_CTL1_STB(2);

    return 0;
}

int uart_setup(struct uart *ctx, const struct uart_settings *settings)
{
    int res;

    if ((res = set_baudrate(ctx, settings->baudrate)) < 0 ||
        (res = set_cs(ctx, settings->cs)) < 0 ||
        (res = set_parity(ctx, settings->parenb, settings->parodd)) < 0 ||
        (res = set_stopb(ctx, settings->cstopb)) < 0)
        return res;

    return 0;
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    int sent = 0;

    while ((size_t)sent != n) {
        if ((ctx->base->USART_STAT & USART_STAT_TBE) == 0)
            break;

        ctx->base->USART_DATA = (uint32_t)buf[sent++];
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
        if ((ctx->base->USART_STAT & USART_STAT_RBNE) == 0)
            break;

        buf[recv++] = (char)ctx->base->USART_DATA;
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

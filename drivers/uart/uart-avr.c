#include "uart-avr.h"
#include "picoRTOS.h"

#include <stdint.h>

struct USART_AVR {
    volatile uint8_t UCSRnA;
    volatile uint8_t UCSRnB;
    volatile uint8_t UCSRnC;
    volatile uint8_t RESERVED;
    volatile uint16_t UBRRn;
    volatile uint8_t UDRn;
};

#define UCSRnA_RXC  (1 << 7)
#define UCSRnA_TXC  (1 << 6)
#define UCSRnA_UDRE (1 << 5)
#define UCSRnA_U2X  (1 << 1)

#define UCSRnB_RXEN (1 << 4)
#define UCSRnB_TXEN (1 << 3)

#define UCSRnC_UPM1 (1 << 5)
#define UCSRnC_UPM0 (1 << 4)
#define UCSRnC_USBS (1 << 3)

static int set_baudrate(struct uart *ctx, unsigned long baud)
{
    if (!picoRTOS_assert(baud > 0)) return -EINVAL;

    clock_freq_t freq = clock_get_freq(ctx->clkid);
    unsigned long ubrr = (unsigned long)freq / 8ul / baud - 1;

    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    ctx->base->UBRRn = (uint16_t)ubrr;

    /* TODO: check freq match */
    return 0;
}

/* public */

/* Function: uart_avr_init
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
int uart_avr_init(struct uart *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct USART_AVR*)base;
    ctx->clkid = clkid;

    /* turn on */
    ctx->base->UCSRnA = (uint8_t)UCSRnA_U2X;
    ctx->base->UCSRnB = (uint8_t)(UCSRnB_TXEN | UCSRnB_RXEN);
    return 0;
}

/* HAL */

int uart_setup(struct uart *ctx, const struct uart_settings *settings)
{
    int res;
    uint8_t ucsrxc = (uint8_t)0;

    if ((res = set_baudrate(ctx, settings->baudrate)) < 0)
        return res;

    if (settings->cstopb) ucsrxc |= UCSRnC_USBS;
    if (settings->parenb) {
        ucsrxc = (uint8_t)UCSRnC_UPM0;
        if (settings->parodd) ucsrxc |= UCSRnC_UPM0;
    }

    switch (settings->cs) {
    case 5: break;
    case 6: ucsrxc |= 0x2; break;
    case 7: ucsrxc |= 0x4; break;
    case 8: ucsrxc |= 0X6; break;
    default:
        arch_break();
        /*@notreached@*/
        return -EIO;
    }

    ctx->base->UCSRnC = ucsrxc;
    return 0;
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    size_t nwritten = 0;

    while (nwritten != n) {

        if ((ctx->base->UCSRnA & UCSRnA_UDRE) == (uint8_t)0)
            break;

        ctx->base->UDRn = (uint8_t)*buf++;
        nwritten++;
    }

    if (nwritten == 0)
        return -EAGAIN;

    return (int)nwritten;
}

int uart_read(struct uart *ctx, char *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    size_t nread = 0;

    while (nread != n) {

        if ((ctx->base->UCSRnA & UCSRnA_RXC) == 0)
            break;

        *buf++ = (char)ctx->base->UDRn;
        nread++;
    }

    if (nread == 0)
        return -EAGAIN;

    return (int)nread;
}

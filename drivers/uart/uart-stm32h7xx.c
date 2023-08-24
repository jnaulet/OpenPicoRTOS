#include "uart-stm32h7xx.h"
#include "picoRTOS.h"

#include <stdint.h>

struct UART_STM32H7XX {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t GTPR;
    volatile uint32_t RTOR;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
    volatile uint32_t PRESC;
};

#define CR1_RXFIE   (1u << 31)
#define CR1_TXFEIE  (1 << 30)
#define CR1_FIFOEN  (1 << 29)
#define CR1_M1      (1 << 28)
#define CR1_EOBIE   (1 << 27)
#define CR1_RTOIE   (1 << 26)
#define CR1_DEAT_M  0x1fu
#define CR1_DEAT(x) (((x) & CR1_DEAT_M) << 21)
#define CR1_DEDT_M  0x1fu
#define CR1_DEDT(x) (((x) & CR1_DEDT_M) << 16)
#define CR1_OVER8   (1 << 15)
#define CR1_CMIE    (1 << 14)
#define CR1_MME     (1 << 13)
#define CR1_M0      (1 << 12)
#define CR1_WAKE    (1 << 11)
#define CR1_PCE     (1 << 10)
#define CR1_PS      (1 << 9)
#define CR1_PEIE    (1 << 8)
#define CR1_TXFNFIE (1 << 7)
#define CR1_TCIE    (1 << 6)
#define CR1_RXFNEIE (1 << 5)
#define CR1_IDLEIE  (1 << 4)
#define CR1_TE      (1 << 3)
#define CR1_RE      (1 << 2)
#define CR1_UESM    (1 << 1)
#define CR1_UE      (1 << 0)

#define CR2_ADD_M     0xffu
#define CR2_ADD(x)    (((x) & CR2_ADD_M) << 24)
#define CR2_RTOEN     (1 << 23)
#define CR2_ABRMOD_M  0x3u
#define CR2_ABRMOD(x) (((x) & CR2_ABRMOD_M) << 21)
#define CR2_ABREN     (1 << 20)
#define CR2_MSBFIRST  (1 << 19)
#define CR2_DATAINV   (1 << 18)
#define CR2_TXINV     (1 << 17)
#define CR2_RXINV     (1 << 16)
#define CR2_SWAP      (1 << 15)
#define CR2_LINEN     (1 << 14)
#define CR2_STOP_M    0x3u
#define CR2_STOP(x)   (((x) & CR2_STOP_M) << 12)
#define CR2_CLKEN     (1 << 11)
#define CR2_CPOL      (1 << 10)
#define CR2_CPHA      (1 << 9)
#define CR2_LBCL      (1 << 8)
#define CR2_LBDIE     (1 << 6)
#define CR2_LBDL      (1 << 5)
#define CR2_ADDM7     (1 << 4)
#define CR2_DIS_NSS   (1 << 3)
#define CR2_SLVEN     (1 << 0)

#define CR3_TXFTCFG_M  0x7u
#define CR3_TXFTCFG(x) (((x) & CR3_TXFTCFG_M) << 29)
#define CR3_RXFTIE     (1 << 28)
#define CR3_RXFTCFG_M  0x7u
#define CR3_RXFTCFG(x) (((x) & CR3_RXFTCFG_M) << 25)
#define CR3_TCBGTIE    (1 << 24)
#define CR3_TXFTIE     (1 << 23)
#define CR3_WUFIE      (1 << 22)
#define CR3_WUS_M      0x3u
#define CR3_WUS(x)     (((x) & CR3_WUS_M) << 20)
#define CR3_SCARCNT_M  0x7u
#define CR3_SCARCNT(x) (((x) & CR3_SCARCNT_M) << 17)
#define CR3_DEP        (1 << 15)
#define CR3_DEM        (1 << 14)
#define CR3_DDRE       (1 << 13)
#define CR3_OVRDIS     (1 << 12)
#define CR3_ONEBIT     (1 << 11)
#define CR3_CTSIE      (1 << 10)
#define CR3_CTSE       (1 << 9)
#define CR3_RTSE       (1 << 8)
#define CR3_DMAT       (1 << 7)
#define CR3_DMAR       (1 << 6)
#define CR3_SCEN       (1 << 5)
#define CR3_NACK       (1 << 4)
#define CR3_HDSEL      (1 << 3)
#define CR3_IRLP       (1 << 2)
#define CR3_IREN       (1 << 1)
#define CR3_EIE        (1 << 0)

#define BRR_BRR_M  0xffffu
#define BRR_BRR(x) ((x) & BRR_BRR_M)

#define GTPR_GT_M   0xffu
#define GPTR_GT(x)  (((x) & GTPR_GT_M) << 8)
#define GPTR_PSC_M  0xffu
#define GPTR_PSC(x) ((x) & GPTR_PSC_M)

#define RTOR_BLEN_M 0xffu
#define RTOR_BLEN(x) (((x) & RTOR_BLEN_M) << 24)
#define RTOR_RTO_M 0xffffffu
#define RTOR_RTO(x) ((x) & RTOR_RTO_M)

#define RQR_TXFRQ (1 << 4)
#define RQR_RXFRQ (1 << 3)
#define RQR_MMRQ  (1 << 2)
#define RQR_SBKRQ (1 << 1)
#define RQR_ABRRQ (1 << 0)

#define ISR_TXFT  (1 << 27)
#define ISR_RXFT  (1 << 26)
#define ISR_TCBGT (1 << 25)
#define ISR_RXFF  (1 << 24)
#define ISR_TXFE  (1 << 23)
#define ISR_REACK (1 << 22)
#define ISR_TXACK (1 << 21)
#define ISR_WUF   (1 << 20)
#define ISR_RWU   (1 << 19)
#define ISR_SBKF  (1 << 18)
#define ISR_CMF   (1 << 17)
#define ISR_BUSY  (1 << 16)
#define ISR_ABRF  (1 << 15)
#define ISR_ABRE  (1 << 14)
#define ISR_UDR   (1 << 13)
#define ISR_EOBF  (1 << 12)
#define ISR_RTOF  (1 << 11)
#define ISR_CTS   (1 << 10)
#define ISR_CTSIF (1 << 9)
#define ISR_LBDF  (1 << 8)
#define ISR_TXFNF (1 << 7)
#define ISR_TC    (1 << 6)
#define ISR_RXFNE (1 << 5)
#define ISR_IDLE  (1 << 4)
#define ISR_ORE   (1 << 3)
#define ISR_NE    (1 << 2)
#define ISR_FE    (1 << 1)
#define ISR_PE    (1 << 0)

#define RDR_RDR_M  0xffu
#define RDR_RDR(x) ((x) & RDR_RDR_M)

#define TDR_TDR_M  0xffu
#define TDR_TDR(x) ((x) & TDR_TDR_M)

#define PRESC_PRESCALER_M  0xfu
#define PRESC_PRESCALER(x) ((x) & PRESC_PRESCALER_M)

/* Function: uart_stm32h7xx_init
 * Initializes an UART/USART
 *
 * Parameters:
 *  ctx - The UART to init
 *  base - The UART base address
 *  clkid - The UART clock ID
 *
 * Returns:
 * Always 0
 */
int uart_stm32h7xx_init(struct uart *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct UART_STM32H7XX*)base;
    ctx->clkid = clkid;

    /* disable uart */
    ctx->base->CR1 &= ~CR1_UE;
    /* force fifo mode */
    ctx->base->CR1 |= CR1_FIFOEN;
    /* enable transmission */
    ctx->base->CR1 |= (CR1_TE | CR1_RE);

    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baudrate)
{
    picoRTOS_assert(baudrate > 0, return -EINVAL);

    static const unsigned long prescaler[] =
    { 1ul, 2ul, 4ul, 6ul, 8ul, 10ul, 12ul, 16ul, 32ul, 64ul, 128ul, 256ul };

    size_t p;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return (int)freq);

    /* oversampling by 16 only, yet */
    for (p = 0; p < (size_t)12; p++) {
        unsigned long usart_ker_ckpres = (unsigned long)freq / prescaler[p];
        unsigned long usartdiv = usart_ker_ckpres / baudrate;

        if (usartdiv <= (unsigned long)BRR_BRR_M) {
            ctx->base->PRESC = (uint32_t)PRESC_PRESCALER(p);
            ctx->base->BRR = (uint32_t)BRR_BRR(usartdiv);
            return 0;
        }
    }

    picoRTOS_break();
    /*@notreached@*/ return -EINVAL;
}

static int set_cs(struct uart *ctx, size_t cs)
{
    picoRTOS_assert(cs >= (size_t)UART_STM32H7XX_CS_MIN, return -EINVAL);
    picoRTOS_assert(cs <= (size_t)UART_STM32H7XX_CS_MAX, return -EINVAL);

    ctx->base->CR1 &= ~CR1_M1;

    switch (cs) {
    case 7:
        ctx->base->CR1 &= ~CR1_M0;
        ctx->base->CR1 |= CR1_M1;
        break;

    case 8:
        ctx->base->CR1 &= ~CR1_M0;
        ctx->base->CR1 &= ~CR1_M1;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

static int set_parity(struct uart *ctx, uart_par_t par)
{
    picoRTOS_assert(par != UART_PAR_IGNORE, return -EINVAL);
    picoRTOS_assert(par < UART_PAR_COUNT, return -EINVAL);

    if (par == UART_PAR_NONE) {
        ctx->base->CR1 &= ~CR1_PCE;
        return 0;
    }

    ctx->base->CR1 |= CR1_PCE;
    if (par == UART_PAR_ODD) ctx->base->CR1 |= CR1_PS;
    else ctx->base->CR1 &= ~CR1_PS;

    return 0;
}

static int set_cstopb(struct uart *ctx, uart_cstopb_t cstopb)
{
    picoRTOS_assert(cstopb != UART_CSTOPB_IGNORE, return -EINVAL);
    picoRTOS_assert(cstopb < UART_CSTOPB_COUNT, return -EINVAL);

    ctx->base->CR2 &= ~CR2_STOP(CR2_STOP_M);

    if (cstopb == UART_CSTOPB_2BIT)
        ctx->base->CR2 |= CR2_STOP(2);

    return 0;
}

int uart_setup(struct uart *ctx, const struct uart_settings *settings)
{
    int res;

    /* disable uart */
    ctx->base->CR1 &= ~CR1_UE;

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

    /* enable uart */
    ctx->base->CR1 |= CR1_UE;

    return 0;
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int sent = 0;

    while ((size_t)sent != n) {
        if ((ctx->base->ISR & ISR_TXFNF) == 0)
            break;

        ctx->base->TDR = (uint32_t)TDR_TDR(buf[sent++]);
    }

    if (sent == 0)
        return -EAGAIN;

    return sent;
}

int uart_read(struct uart *ctx, char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int recv = 0;

    while ((size_t)recv != n) {
        if ((ctx->base->ISR & ISR_RXFNE) == 0)
            break;

        buf[recv++] = (char)(RDR_RDR_M & ctx->base->RDR);
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

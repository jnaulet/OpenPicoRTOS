#include "uart-renesas_ra4_sci.h"
#include "picoRTOS.h"

#include <stdint.h>

struct UART_RENESAS_RA4_SCI {
    volatile uint8_t SMR;       /* 0000h */
    volatile uint8_t BRR;       /* 0001h */
    volatile uint8_t SCR;       /* 0002h */
    volatile uint8_t TDR;       /* 0003h */
    volatile uint8_t SSR;       /* 0004h */
    volatile uint8_t RDR;       /* 0005h */
    volatile uint8_t SCMR;      /* 0006h */
    volatile uint8_t SEMR;      /* 0007h */
    volatile uint8_t SNFR;      /* 0008h */
    volatile uint8_t SIMR1;     /* 0009h */
    volatile uint8_t SIMR2;     /* 000ah */
    volatile uint8_t SIMRB;     /* 000bh */
    volatile uint8_t SISR;      /* 000ch */
    volatile uint8_t SPMR;      /* 000dh */
    volatile uint16_t FTDRHL;   /* 000eh */
    volatile uint16_t FRDRHL;   /* 0010h */
    volatile uint8_t MDDR;      /* 0012h */
    volatile uint8_t DCCR;      /* 0013h */
    volatile uint16_t FCR;      /* 0014h */
    volatile uint16_t FDR;      /* 0016h */
    volatile uint16_t LSR;      /* 0018h */
    volatile uint16_t CDR;      /* 001ah */
    volatile uint8_t SPTR;      /* 001ch */
};

#define SMR_CM     (1 << 7)
#define SMR_CHR    (1 << 6)
#define SMR_PE     (1 << 5)
#define SMR_PM     (1 << 4)
#define SMR_STOP   (1 << 3)
#define SMR_MP     (1 << 2)
#define SMR_CKS_M  0x3u
#define SMR_CKS(x) ((x) & SMR_CKS_M)

#define BRR_BRR_M  0xffu
#define BRR_BRR(x) ((x) & BRR_BRR_M)

#define SCR_TIE    (1 << 7)
#define SCR_RIE    (1 << 6)
#define SCR_TE     (1 << 5)
#define SCR_RE     (1 << 4)
#define SCR_MPIE   (1 << 3)
#define SCR_TEIE   (1 << 2)
#define SCR_CKE_M  0x3u
#define SCR_CKE(x) ((x) & SCR_CKE_M)

#define SSR_TDRE (1 << 7)
#define SSR_RDRF (1 << 6)
#define SSR_ORER (1 << 5)
#define SSR_FER  (1 << 4)
#define SSR_PER  (1 << 3)
#define SSR_TEND (1 << 2)
#define SSR_MPB  (1 << 1)
#define SSR_MPBT (1 << 0)

#define SCMR_BCP2 (1 << 7)
#define SCMR_CHR1 (1 << 4)
#define SCMR_SDIR (1 << 3)
#define SCMR_SINV (1 << 2)
#define SCMR_SMIF (1 << 0)

#define SEMR_RXDESEL (1 << 7)
#define SEMR_BGDM    (1 << 6)
#define SEMR_NFEN    (1 << 5)
#define SEMR_ABCS    (1 << 4)
#define SEMR_ABCSE   (1 << 3)
#define SEMR_BRME    (1 << 2)

#define SNFR_NFSCS_M  0x7u
#define SNFR_NFSCS(x) ((x) & SNFR_NFSCS_M)

#define FCR_RSTRG_M  0xfu
#define FCR_RSTRG(x) (((x) & FCR_RSTRG_M) << 12)
#define FCR_RTRG_M   0xfu
#define FCR_RTRG(x)  (((x) & FCR_RTRG_M) << 8)
#define FCR_TTRG_M   0xfu
#define FCR_TTRG(x)  (((x) & FCR_TTRG_M) << 4)
#define FCR_DRES     (1 << 3)
#define FCR_TFRST    (1 << 2)
#define FCR_RFRST    (1 << 1)
#define FCR_FM       (1 << 0)

#define SPTR_SPB2IO (1 << 2)
#define SPTR_SPB2DT (1 << 1)
#define SPTR_RXDMON (1 << 0)

int uart_renesas_ra4_sci_init(/*@out@*/ struct uart *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct UART_RENESAS_RA4_SCI*)base;
    ctx->clkid = clkid;

    /* setup for uart */
    ctx->base->SMR &= ~SMR_CM; /* async mode */

    /* TODO: FIFO mode */

    return 0;
}

static int set_baudrate(struct uart *ctx, unsigned long baudrate)
{
    picoRTOS_assert(baudrate > 0, return -EINVAL);

    size_t i;
    clock_freq_t freq = clock_get_freq(ctx->clkid);
    static const unsigned long cks[] = { 1ul, 4ul, 16ul, 64ul };

    picoRTOS_assert(freq > 0, return (int)freq);

    for (i = 0; i < (size_t)4; i++) {
        /* original formula:
         * (PCLKA * 10^6) / (64 * 2^2n-1 * B) - 1
         */
        unsigned long brr = (unsigned long)freq / ((64ul * cks[i] / 2ul) * baudrate) - 1ul;
        if (brr <= (unsigned long)BRR_BRR_M) {
            ctx->base->BRR = (uint8_t)BRR_BRR(brr);
            ctx->base->SMR &= ~SMR_CKS(SMR_CKS_M);
            ctx->base->SMR |= SMR_CKS(i);
            return 0;
        }
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int set_cs(struct uart *ctx, size_t cs)
{
    picoRTOS_assert(cs >= (size_t)UART_RENESAS_RA4_SCI_CS_MIN, return -EINVAL);
    picoRTOS_assert(cs <= (size_t)UART_RENESAS_RA4_SCI_CS_MAX, return -EINVAL);

    switch (cs) {
    case 7:
        ctx->base->SMR |= SMR_CHR;
        ctx->base->SCMR |= SCMR_CHR1;
        break;

    case 8:
        ctx->base->SMR &= ~SMR_CHR;
        ctx->base->SCMR |= SCMR_CHR1;
        break;

    case 9:
        ctx->base->SCMR &= ~SCMR_CHR1;
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

    /* no parity */
    if (par == UART_PAR_NONE) {
        ctx->base->SMR &= ~SMR_PE;
        return 0;
    }

    /* parity */
    ctx->base->SMR |= SMR_PE;
    if (par == UART_PAR_ODD) ctx->base->SMR |= SMR_PM;
    else ctx->base->SMR &= ~SMR_PM;

    return 0;
}

static int set_cstopb(struct uart *ctx, uart_cstopb_t cstopb)
{
    picoRTOS_assert(cstopb != UART_CSTOPB_IGNORE, return -EINVAL);
    picoRTOS_assert(cstopb < UART_CSTOPB_COUNT, return -EINVAL);

    if (cstopb == UART_CSTOPB_2BIT) ctx->base->SMR |= SMR_STOP;
    else ctx->base->SMR &= ~SMR_STOP;

    return 0;
}

int uart_setup(struct uart *ctx, const struct uart_settings *settings)
{
    int res;

    /* disable */
    ctx->base->SCR &= ~(SCR_TE | SCR_RE);

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

    /* enable */
    ctx->base->SCR |= (SCR_TE | SCR_RE);

    return 0;
}

int uart_write(struct uart *ctx, const char *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    size_t nwritten = 0;

    while (nwritten != n) {

        if ((ctx->base->SSR & SSR_TDRE) == 0)
            break;

        ctx->base->TDR = (uint8_t)*buf++;
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

        if ((ctx->base->SSR & SSR_RDRF) == 0)
            break;

        *buf++ = (char)ctx->base->RDR;
        nread++;
    }

    if (nread == 0)
        return -EAGAIN;

    return (int)nread;
}

#include "twi-renesas_ra4.h"
#include "picoRTOS.h"

#include <stdint.h>

struct TWI_RENESAS_RA4 {
    volatile uint8_t ICCR1;     /* 3000h */
    volatile uint8_t ICCR2;     /* 3001h */
    volatile uint8_t ICMR1;     /* 3002h */
    volatile uint8_t ICMR2;     /* 3003h */
    volatile uint8_t ICMR3;     /* 3004h */
    volatile uint8_t ICFER;     /* 3005h */
    volatile uint8_t ICSER;     /* 3006h */
    volatile uint8_t ICIER;     /* 3007h */
    volatile uint8_t ICSR1;     /* 3008h */
    volatile uint8_t ICSR2;     /* 3009h */
    struct {
        volatile uint8_t L;     /* 300ah */
        volatile uint8_t U;     /* 300bh */
    } SAR[3];
    volatile uint8_t ICBRL;     /* 3010h */
    volatile uint8_t ICBRH;     /* 3011h */
    volatile uint8_t ICDRT;     /* 3012h */
    volatile uint8_t ICDRR;     /* 3013h */
    uint8_t RESERVED0[2];
    volatile uint8_t ICWUR;     /* 3016h */
    volatile uint8_t ICWUR2;    /* 3017h */
};

#define ICCR1_ICE    (1 << 7)
#define ICCR1_IICRST (1 << 6)
#define ICCR1_CLO    (1 << 5)
#define ICCR1_SOWP   (1 << 4)
#define ICCR1_SCLO   (1 << 3)
#define ICCR1_SDAO   (1 << 2)
#define ICCR1_SCLI   (1 << 1)
#define ICCR1_SDAI   (1 << 0)

#define ICCR2_BBSY (1 << 7)
#define ICCR2_MST  (1 << 6)
#define ICCR2_TRS  (1 << 5)
#define ICCR2_SP   (1 << 3)
#define ICCR2_RS   (1 << 2)
#define ICCR2_ST   (1 << 1)

#define ICMR1_MTWP   (1 << 7)
#define ICMR1_CKS_M  0x7u
#define ICMR1_CKS(x) (((x) & ICMR1_CKS_M) << 4)
#define ICMR1_BCWP   (1 << 3)
#define ICMR1_BC_M   0x7u
#define ICMR1_BC(x)  ((x) & ICMR1_BC_M)

#define ICMR2_DLCS    (1 << 7)
#define ICMR2_SDDL_M  0x7u
#define ICMR2_SDDL(x) (((x) & ICMR2_SDDL_M) << 4)
#define ICMR2_TMOH    (1 << 2)
#define ICMR2_TMOL    (1 << 1)
#define ICMR2_TMOS    (1 << 0)

#define ICMR3_SMBS  (1 << 7)
#define ICMR3_WAIT  (1 << 6)
#define ICMR3_RDRFS (1 << 5)
#define ICMR3_ACKWP (1 << 4)
#define ICMR3_ACKBT (1 << 3)
#define ICMR3_ACKBR (1 << 2)
#define ICMR3_NF_M  0x3u
#define ICMR3_NF(x) ((x) & ICMR3_NF_M)

#define ICFER_SCLE  (1 << 6)
#define ICFER_NFE   (1 << 5)
#define ICFER_NACKE (1 << 4)
#define ICFER_SALE  (1 << 3)
#define ICFER_NALE  (1 << 2)
#define ICFER_MALE  (1 << 1)
#define ICFER_TMOE  (1 << 0)

#define ICSER_HOAE  (1 << 7)
#define ICSER_DIDE  (1 << 5)
#define ICSER_GCAE  (1 << 3)
#define ICSER_SAR2E (1 << 2)
#define ICSER_SAR1E (1 << 1)
#define ICSER_SAR0E (1 << 0)

#define ICIER_TIE   (1 << 7)
#define ICIER_TEIE  (1 << 6)
#define ICIER_RIE   (1 << 5)
#define ICIER_NAKIE (1 << 4)
#define ICIER_SPIE  (1 << 3)
#define ICIER_STIE  (1 << 2)
#define ICIER_ALIE  (1 << 1)
#define ICIER_TMOIE (1 << 0)

#define ICSR1_HOA (1 << 7)
#define ICSR1_DID (1 << 6)
#define ICSR1_GCA (1 << 3)
#define ICSR1_AAS2 (1 << 2)
#define ICSR1_AAS1 (1 << 1)
#define ICSR1_AAS0 (1 << 0)

#define ICSR2_TDRE  (1 << 7)
#define ICSR2_TEND  (1 << 6)
#define ICSR2_RDRF  (1 << 5)
#define ICSR2_NACKF (1 << 4)
#define ICSR2_STOP  (1 << 3)
#define ICSR2_START (1 << 2)
#define ICSR2_AL    (1 << 1)
#define ICSR2_TMOF  (1 << 0)

#define ICWUR_WUE   (1 << 7)
#define ICWUR_WUIE  (1 << 6)
#define ICWUR_WUF   (1 << 5)
#define ICWUR_WUACK (1 << 4)
#define ICWUR_WUAFA (1 << 0)

#define ICWUR2_WUSYF  (1 << 2)
#define ICWUR2_WUASYF (1 << 1)
#define ICWUR2_WUSEN  (1 << 0)

#define SARLy_SVA_M  0x7fu
#define SARLy_SVA(x) (((x) & SARLy_SVA_M) << 1)
#define SARLY_SVA0   (1 << 0)

#define SARUy_SVA_M  0x3u
#define SARUy_SVA(x) (((x) & SARUy_SVA_M) << 1)
#define SARUy_FS     (1 << 0)

#define ICBRL_BRL_M  0x1fu
#define ICBRL_BRL(x) ((x) & ICBRL_BRL_M)

#define ICBRH_BRH_M  0x1fu
#define ICBRH_BRH(x) ((x) & ICBRH_BRH_M)

int twi_renesas_ra4_init(struct twi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct TWI_RENESAS_RA4*)base;
    ctx->clkid = clkid;
    ctx->state = TWI_RENESAS_RA4_STATE_IDLE;
    ctx->last = 0;
    ctx->mode = TWI_MODE_SLAVE;
    ctx->slave_addr = 0;

    /* init reset */
    ctx->base->ICCR1 &= ~ICCR1_ICE;

    return 0;
}

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    size_t cks;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return -EIO);

    for (cks = 0; cks <= (size_t)ICMR1_CKS_M; cks++) {

        unsigned long d = (1ul << cks) * 2ul;
        unsigned long br = ((unsigned long)freq / bitrate) / d;

        if (br <= (unsigned long)ICBRL_BRL_M) {
            ctx->base->ICMR1 &= ~ICMR1_CKS(ICMR1_CKS_M);
            ctx->base->ICMR1 |= ICMR1_CKS(cks);
            ctx->base->ICBRL = (uint8_t)ICBRL_BRL(br);
            ctx->base->ICBRH = (uint8_t)ICBRH_BRH(br);
            return 0;
        }
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EINVAL;
}

static int set_mode(struct twi *ctx, twi_mode_t mode)
{
    picoRTOS_assert(mode != TWI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < TWI_MODE_COUNT, return -EINVAL);

    ctx->mode = mode;
    return 0;
}

int twi_setup(struct twi *ctx, struct twi_settings *settings)
{
    int res = 0;

    /* reset */
    ctx->base->ICCR1 |= ICCR1_IICRST;

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* mode */
    if (settings->mode != TWI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    /* slave_addr */
    if (settings->slave_addr != (twi_addr_t)0) {
        if (ctx->mode == TWI_MODE_SLAVE) {
            /* 7-bit only for now */
            ctx->base->ICSER |= ICSER_SAR0E;
            ctx->base->SAR[0].L = (uint8_t)SARLy_SVA(settings->slave_addr);
        }

        ctx->slave_addr = settings->slave_addr;
    }

    /* un-reset */
    ctx->base->ICCR1 &= ~ICCR1_IICRST;
    return res;
}

static int twi_write_as_master_stop(struct twi *ctx)
{
    if ((ctx->base->ICSR2 & ICSR2_STOP) == 0)
        return -EAGAIN;

    ctx->base->ICSR2 &= ~(ICSR2_NACKF | ICSR2_STOP);
    ctx->state = TWI_RENESAS_RA4_STATE_IDLE;

    return ctx->last;
}

static int twi_write_as_master_end(struct twi *ctx)
{
    if ((ctx->base->ICSR2 & ICSR2_TEND) == 0)
        return -EAGAIN;

    ctx->base->ICCR2 |= ICCR2_SP;
    ctx->state = TWI_RENESAS_RA4_STATE_STOP;

    return twi_write_as_master_stop(ctx);
}

static int twi_write_as_master_xfer(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int nwritten = 0;
    uint8_t *buf8 = (uint8_t*)buf;

    while (nwritten != (int)n) {
        /* NACK */
        if ((ctx->base->ICSR2 & ICSR2_NACKF) != 0) {
            ctx->last = 0;
            ctx->state = TWI_RENESAS_RA4_STATE_END;
            return twi_write_as_master_end(ctx);
        }

        if ((ctx->base->ICSR2 & ICSR2_TDRE) == 0)
            break;

        ctx->base->ICDRT = (uint8_t)buf8[nwritten++];
    }

    if (nwritten == 0)
        return -EAGAIN;

    if (nwritten == (int)n) {
        ctx->last = nwritten;
        ctx->state = TWI_RENESAS_RA4_STATE_END;
        return twi_write_as_master_end(ctx);
    }

    return nwritten;
}

static int twi_write_as_master_sla(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->ICSR2 & ICSR2_TDRE) == 0)
        return -EAGAIN;

    ctx->base->ICDRT = (uint8_t)(ctx->slave_addr << 1);

    ctx->state = TWI_RENESAS_RA4_STATE_XFER;
    return twi_write_as_master_xfer(ctx, buf, n);
}

static int twi_write_as_master_idle(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->ICCR2 & ICCR2_BBSY) != 0)
        return -EAGAIN;

    ctx->base->ICCR2 |= ICCR2_ST;
    ctx->state = TWI_RENESAS_RA4_STATE_SLA;

    return twi_write_as_master_sla(ctx, buf, n);
}

static int twi_write_as_master(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_RENESAS_RA4_STATE_IDLE: return twi_write_as_master_idle(ctx, buf, n);
    case TWI_RENESAS_RA4_STATE_SLA: return twi_write_as_master_sla(ctx, buf, n);
    case TWI_RENESAS_RA4_STATE_XFER: return twi_write_as_master_xfer(ctx, buf, n);
    case TWI_RENESAS_RA4_STATE_END: return twi_write_as_master_end(ctx);
    case TWI_RENESAS_RA4_STATE_STOP: return twi_write_as_master_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int twi_read_as_master_stop(struct twi *ctx)
{
    if ((ctx->base->ICSR2 & ICSR2_STOP) == 0)
        return -EAGAIN;

    ctx->base->ICSR2 &= ~(ICSR2_NACKF | ICSR2_STOP);
    ctx->state = TWI_RENESAS_RA4_STATE_IDLE;

    return ctx->last;
}

static int twi_read_as_master_xfer(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int nread = 0;
    uint8_t *buf8 = (uint8_t*)buf;

    while (nread != (int)n) {

        /* last byte */
        if (nread + 1 == (int)n)
            ctx->base->ICMR3 |= ICMR3_ACKBT; /* NACK */

        /* dummy read ICDRR register */
        /*@i@*/ (void)ctx->base->ICDRR;

        if ((ctx->base->ICSR2 & ICSR2_RDRF) == 0)
            break;

        buf8[nread++] = ctx->base->ICDRR;
    }

    if (nread == 0)
        return -EAGAIN;

    if (nread == (int)n) {
        ctx->last = nread;
        ctx->base->ICMR3 &= ~ICMR3_WAIT;
        ctx->state = TWI_RENESAS_RA4_STATE_STOP;
        return twi_read_as_master_stop(ctx);
    }

    return nread;
}

static int twi_read_as_master_wait(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->ICSR2 & ICSR2_RDRF) == 0)
        return -EAGAIN;

    if ((ctx->base->ICSR2 & ICSR2_NACKF) != 0) {
        ctx->base->ICSR2 &= ~ICSR2_STOP;
        ctx->base->ICCR2 |= ICCR2_SP;

        /* dummy read the ICDRR register */
        /*@i@*/ (void)ctx->base->ICDRR;

        /* next step */
        ctx->state = TWI_RENESAS_RA4_STATE_STOP;
        return twi_read_as_master_stop(ctx);
    }

    /* dummy read ICDRR register */
    /*@i@*/ (void)ctx->base->ICDRR;

    /* next data == last byte ? */
    if (n <= (size_t)2)
        ctx->base->ICMR3 |= ICMR3_WAIT;

    ctx->state = TWI_RENESAS_RA4_STATE_XFER;
    return twi_read_as_master_xfer(ctx, buf, n);
}

static int twi_read_as_master_sla(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->ICSR2 & ICSR2_TDRE) == 0)
        return -EAGAIN;

    ctx->base->ICDRT = (uint8_t)(ctx->slave_addr << 1) | (uint8_t)0x1;

    ctx->state = TWI_RENESAS_RA4_STATE_WAIT;
    return twi_read_as_master_xfer(ctx, buf, n);
}

static int twi_read_as_master_idle(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->ICCR2 & ICCR2_BBSY) != 0)
        return -EAGAIN;

    ctx->base->ICCR2 |= ICCR2_ST;
    ctx->state = TWI_RENESAS_RA4_STATE_SLA;

    return twi_read_as_master_sla(ctx, buf, n);
}

static int twi_read_as_master(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_RENESAS_RA4_STATE_IDLE: return twi_read_as_master_idle(ctx, buf, n);
    case TWI_RENESAS_RA4_STATE_SLA: return twi_read_as_master_sla(ctx, buf, n);
    case TWI_RENESAS_RA4_STATE_WAIT: return twi_read_as_master_wait(ctx, buf, n);
    case TWI_RENESAS_RA4_STATE_XFER: return twi_read_as_master_xfer(ctx, buf, n);
    case TWI_RENESAS_RA4_STATE_STOP: return twi_read_as_master_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int twi_write_as_slave_stop(struct twi *ctx)
{
    if ((ctx->base->ICSR2 & ICSR2_STOP) == 0)
        return -EAGAIN;

    ctx->base->ICSR2 &= ~(ICSR2_NACKF | ICSR2_STOP);
    ctx->state = TWI_RENESAS_RA4_STATE_IDLE;

    /* last byte */
    return ctx->last;
}

static int twi_write_as_slave_end(struct twi *ctx)
{
    if ((ctx->base->ICSR2 & ICSR2_TEND) == 0)
        return -EAGAIN;

    /* dummy read to release SCLn */
    /*@i@*/ (void)ctx->base->ICDRR;

    ctx->state = TWI_RENESAS_RA4_STATE_STOP;
    return twi_write_as_slave_stop(ctx);
}

static int twi_write_as_slave_xfer(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int nwritten = 0;
    uint8_t *buf8 = (uint8_t*)buf;

    while (nwritten != (int)n) {
        if ((ctx->base->ICSR2 & ICSR2_NACKF) != 0) {
            ctx->last = 0;
            /*@i@*/ (void)ctx->base->ICDRR;
            ctx->state = TWI_RENESAS_RA4_STATE_STOP;
            return twi_write_as_slave_stop(ctx);
        }

        if ((ctx->base->ICSR2 & ICSR2_TDRE) == 0)
            break;

        ctx->base->ICDRT = buf8[nwritten++];
    }

    if (nwritten == 0)
        return -EAGAIN;

    if (nwritten == (int)n) {
        ctx->last = nwritten;
        ctx->state = TWI_RENESAS_RA4_STATE_END;
        return twi_write_as_slave_end(ctx);
    }

    return nwritten;
}

static int twi_write_as_slave(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_RENESAS_RA4_STATE_IDLE: /*@fallthrough@*/
    case TWI_RENESAS_RA4_STATE_XFER: return twi_write_as_slave_xfer(ctx, buf, n);
    case TWI_RENESAS_RA4_STATE_END: return twi_write_as_slave_end(ctx);
    case TWI_RENESAS_RA4_STATE_STOP: return twi_write_as_slave_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int twi_read_as_slave_stop(struct twi *ctx)
{
    if ((ctx->base->ICSR2 & ICSR2_STOP) == 0)
        return -EAGAIN;

    ctx->base->ICSR2 &= ~ICSR2_STOP;
    ctx->state = TWI_RENESAS_RA4_STATE_IDLE;

    return ctx->last;
}

static int twi_read_as_slave_xfer(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int nread = 0;
    uint8_t *buf8 = (uint8_t*)buf;

    while (nread != (int)n) {
        if ((ctx->base->ICSR2 & ICSR2_STOP) != 0) {
            /* shortcut to idle */
            if ((ctx->base->ICSR2 & ICSR2_RDRF) == 0) {
                ctx->base->ICSR2 &= ~ICSR2_STOP;
                ctx->state = TWI_RENESAS_RA4_STATE_IDLE;
                return 0;
            }

            buf8[nread++] = ctx->base->ICDRR;
            ctx->last = nread;
            /* sm */
            ctx->state = TWI_RENESAS_RA4_STATE_STOP;
            return twi_read_as_slave_stop(ctx);
        }

        if ((ctx->base->ICSR2 & ICSR2_RDRF) == 0)
            return -EAGAIN;

        buf8[nread++] = ctx->base->ICDRR;
    }

    if (nread == 0)
        return -EAGAIN;

    ctx->last = nread;
    ctx->state = TWI_RENESAS_RA4_STATE_STOP;
    return twi_read_as_slave_stop(ctx);
}

static int twi_read_as_slave(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_RENESAS_RA4_STATE_IDLE: /*@fallthrough@*/
    case TWI_RENESAS_RA4_STATE_XFER: return twi_read_as_slave_xfer(ctx, buf, n);
    case TWI_RENESAS_RA4_STATE_STOP: return twi_read_as_slave_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int twi_poll(struct twi *ctx)
{
    if ((ctx->base->ICSR1 & ICSR1_AAS0) != 0) {
        if ((ctx->base->ICSR2 & ICSR2_TDRE) != 0) return TWI_READ;
        else return TWI_READ;
    }

    return -EAGAIN;
}

int twi_write(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->mode) {
    case TWI_MODE_MASTER: return twi_write_as_master(ctx, buf, n);
    case TWI_MODE_SLAVE: return twi_write_as_slave(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int twi_read(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->mode) {
    case TWI_MODE_MASTER: return twi_read_as_master(ctx, buf, n);
    case TWI_MODE_SLAVE: return twi_read_as_slave(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

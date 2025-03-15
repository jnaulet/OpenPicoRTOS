#include "lin-nxp_linflexd.h"
#include "picoRTOS.h"

struct LIN_NXP_LINFLEXD {
    volatile uint32_t LINCR1;
    volatile uint32_t LINIER;
    volatile uint32_t LINSR;
    volatile uint32_t LINESR;
    volatile uint32_t UARTCR;
    volatile uint32_t UARTSR;
    volatile uint32_t LINTCSR;
    volatile uint32_t LINOCR;
    volatile uint32_t LINTOCR;
    volatile uint32_t LINFBRR;
    volatile uint32_t LINIBRR;
    volatile uint32_t LINCFR;
    volatile uint32_t LINCR2;
    volatile uint32_t BIDR;
    volatile uint32_t BDRL;
    volatile uint32_t BDRM;
    volatile uint32_t IFER;
    volatile uint32_t IFMI;
    volatile uint32_t IFMR;
    volatile uint32_t IFCR[16];
    volatile uint32_t GCR;
    volatile uint32_t UARTPTO;
    volatile uint32_t UARTCTO;
    volatile uint32_t DMATXE;
    volatile uint32_t DMARXE;
};

#define LINCR1_MBL_M  0xfu
#define LINCR1_MBL(x) (((x) &  LINCR1_MBL_M) << 8)
#define LINCR1_BF     (1 << 7)
#define LINCR1_MME    (1 << 4)
#define LINCR1_SLEEP  (1 << 1)
#define LINCR1_INIT   (1 << 0)

#define LINSR_LINS_M      0xfu
#define LINSR_LINS(x)     (((x) & LINSR_LINS_M) << 12)
#define LINSR_RMB         (1 << 9)
#define LINSR_DRBNE       (1 << 8)
#define LINSR_RXBUSY      (1 << 7)
#define LINSR_DRF         (1 << 2)
#define LINSR_DTF         (1 << 1)
#define LINSR_HRF         (1 << 0)

#define LINESR_CEF (1 << 12)

#define LINFBRR_FBR_M  0xfu
#define LINFBRR_FBR(x) ((x) & LINFBRR_FBR_M)

#define LINIBRR_IBR_M  0xffffu
#define LINIBRR_IBR(x) ((x) & LINIBRR_IBR_M)

#define LINCR2_DTRQ (1 << 10)
#define LINCR2_HTRQ (1 << 8)

#define BIDR_DFL_M      0x7u
#define BIDR_DFL(x)     (((x) & BIDR_DFL_M) << 10)
#define BIDR_DIR        (1 << 9)
#define BIDR_CCS        (1 << 8)
#define BIDR_ID_M       0x3fu
#define BIDR_ID(x)      ((x) & BIDR_ID_M)

#define BDRn_DATAn_M      0xffu
#define BDRn_DATA3(x)     (((x) & BDRn_DATAn_M) << 24)
#define BDRn_DATA2(x)     (((x) & BDRn_DATAn_M) << 16)
#define BDRn_DATA1(x)     (((x) & BDRn_DATAn_M) << 8)
#define BDRn_DATA0(x)     ((x) & BDRn_DATAn_M)

static int request_init_mode_busywait(struct lin *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    ctx->base->LINCR1 &= ~LINCR1_SLEEP;
    ctx->base->LINCR1 |= LINCR1_INIT;

    while (deadlock-- != 0)
        if ((ctx->base->LINSR & LINSR_LINS(LINSR_LINS_M)) == LINSR_LINS(1))
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

int lin_nxp_linflexd_init(struct lin *ctx, int base, clock_id_t clkid)
{
    int res;

    ctx->base = (struct LIN_NXP_LINFLEXD*)base;
    ctx->clkid = clkid;
    ctx->mode = LIN_MODE_SLAVE;
    ctx->state = LIN_NXP_LINFLEXD_STATE_IDLE;

    /* ensure init mode */
    if ((res = request_init_mode_busywait(ctx)) < 0)
        return res;

    /* default break is 13bits */
    ctx->base->LINCR1 &= ~LINCR1_MBL(LINCR1_MBL_M);
    ctx->base->LINCR1 |= LINCR1_MBL(0x3);
    /* bypass filter */
    ctx->base->LINCR1 |= LINCR1_BF;
    /* no idle if timeout */
    ctx->base->LINTCSR = 0;

    /* end of init mode */
    ctx->base->LINCR1 &= ~LINCR1_INIT;
    return 0;
}

static int set_bitrate(struct lin *ctx, unsigned long bitrate)
{
#define div_round_closest(a, b) (((a) + ((b) >> 1)) / (b))

    picoRTOS_assert(bitrate > 0, return -EINVAL);

    clock_freq_t freq;

    if ((freq = clock_get_freq(ctx->clkid)) < 0)
        return (int)freq;

    unsigned long divider = bitrate * 16ul;
    unsigned long ibr = (unsigned long)freq / divider;
    unsigned long fbr = div_round_closest(((unsigned long)freq % divider) * 16ul, divider);

    /* integer part */
    ctx->base->LINIBRR &= ~LINIBRR_IBR(LINIBRR_IBR_M);
    ctx->base->LINIBRR |= LINIBRR_IBR(ibr);
    /* fractional part */
    ctx->base->LINFBRR = (uint32_t)LINFBRR_FBR(fbr);

    return 0;
}

static int set_mode(struct lin *ctx, lin_mode_t mode)
{
    picoRTOS_assert(mode != LIN_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < LIN_MODE_COUNT, return -EINVAL);

    if (mode == LIN_MODE_MASTER) ctx->base->LINCR1 |= LINCR1_MME;
    else ctx->base->LINCR1 &= ~LINCR1_MME;

    ctx->mode = mode;
    return 0;
}

int lin_setup(struct lin *ctx, const struct lin_settings *settings)
{
    int res;

    /* request init mode */
    if ((res = request_init_mode_busywait(ctx)) < 0)
        return res;

    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if (settings->mode != LIN_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    /* end of init mode */
    ctx->base->LINCR1 &= ~LINCR1_INIT;

    return 0;
}

static int copy_to_bdr(struct lin *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)LIN_FRAME_COUNT, return -EINVAL);

    const uint8_t *buf8 = (uint8_t*)buf;

    /* reset hi & lo */
    ctx->base->BDRM = 0;
    ctx->base->BDRL = 0;

    switch (n) {
    case 8: ctx->base->BDRM |= BDRn_DATA3(buf8[7]); /*@fallthrough@*/
    case 7: ctx->base->BDRM |= BDRn_DATA2(buf8[6]); /*@fallthrough@*/
    case 6: ctx->base->BDRM |= BDRn_DATA1(buf8[5]); /*@fallthrough@*/
    case 5: ctx->base->BDRM |= BDRn_DATA0(buf8[4]); /*@fallthrough@*/
    case 4: ctx->base->BDRL |= BDRn_DATA3(buf8[3]); /*@fallthrough@*/
    case 3: ctx->base->BDRL |= BDRn_DATA2(buf8[2]); /*@fallthrough@*/
    case 2: ctx->base->BDRL |= BDRn_DATA1(buf8[1]); /*@fallthrough@*/
    case 1: ctx->base->BDRL |= BDRn_DATA0(buf8[0]); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    return (int)n;
}

static int lin_write_as_master_idle(struct lin *ctx, uint8_t id, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)LIN_FRAME_COUNT, return -EINVAL);

    int res;

    /* write */
    ctx->base->BIDR |= BIDR_DIR;
    if ((res = copy_to_bdr(ctx, buf, n)) < 0)
        return res;

    /* address size */
    ctx->base->BIDR |= BIDR_ID(id);
    ctx->base->BIDR |= BIDR_DFL(n - 1);
    /* lin v1.3. TODO: add support for 2.0 */
    ctx->base->BIDR |= BIDR_CCS;

    /* header transmission request */
    ctx->base->LINCR2 |= LINCR2_HTRQ;

    ctx->state = LIN_NXP_LINFLEXD_STATE_XFER;
    return -EAGAIN;
}

static int lin_write_as_master_xfer(struct lin *ctx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)LIN_FRAME_COUNT, return -EINVAL);

    if ((ctx->base->LINSR & LINSR_DTF) == 0)
        return -EAGAIN;

    /* ack */
    ctx->base->LINSR |= LINSR_DTF;
    ctx->state = LIN_NXP_LINFLEXD_STATE_IDLE;

    return (int)n;
}

static int lin_write_as_master(struct lin *ctx, uint8_t id, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)LIN_FRAME_COUNT, return -EINVAL);

    switch (ctx->state) {
    case LIN_NXP_LINFLEXD_STATE_IDLE: return lin_write_as_master_idle(ctx, id, buf, n);
    case LIN_NXP_LINFLEXD_STATE_XFER: return lin_write_as_master_xfer(ctx, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int lin_write_as_slave(struct lin *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)LIN_FRAME_COUNT, return -EINVAL);

    int res;

    /* check hrf & rxbusy */
    if ((ctx->base->LINSR & LINSR_HRF) == 0 ||
        (ctx->base->LINSR & LINSR_RXBUSY) != 0)
        return -EAGAIN;

    /* clear flags */
    ctx->base->LINSR = (uint32_t)LINSR_HRF;

    /* copy data */
    if ((res = copy_to_bdr(ctx, buf, n)) < 0)
        return res;

    /* set size & direction */
    ctx->base->BIDR = (uint32_t)BIDR_DIR;
    ctx->base->BIDR |= BIDR_DFL(n - 1);
    /* lin v1.3. TODO: add v2.0 option */
    ctx->base->BIDR |= BIDR_CCS;

    /* data transmission request */
    ctx->base->LINCR2 |= LINCR2_DTRQ;
    return (int)n;
}

int lin_write(struct lin *ctx, uint8_t id, const void *buf, size_t n)
{
    picoRTOS_assert(id < (uint8_t)LIN_ID_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->mode) {
    case LIN_MODE_MASTER: return lin_write_as_master(ctx, id, buf, n);
    case LIN_MODE_SLAVE: return lin_write_as_slave(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int copy_from_bdr(struct lin *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)LIN_FRAME_COUNT, return -EINVAL);

    uint8_t *buf8 = (uint8_t*)buf;

    switch (n) {
    case 8: buf8[7] = (uint8_t)(ctx->base->BDRM >> 24); /*@fallthrough@*/
    case 7: buf8[6] = (uint8_t)(ctx->base->BDRM >> 16); /*@fallthrough@*/
    case 6: buf8[5] = (uint8_t)(ctx->base->BDRM >> 8);  /*@fallthrough@*/
    case 5: buf8[4] = (uint8_t)(ctx->base->BDRM >> 0);  /*@fallthrough@*/
    case 4: buf8[3] = (uint8_t)(ctx->base->BDRL >> 24); /*@fallthrough@*/
    case 3: buf8[2] = (uint8_t)(ctx->base->BDRL >> 16); /*@fallthrough@*/
    case 2: buf8[1] = (uint8_t)(ctx->base->BDRL >> 8);  /*@fallthrough@*/
    case 1: buf8[0] = (uint8_t)(ctx->base->BDRL >> 0); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    return (int)n;
}

static int lin_read_as_master_idle(struct lin *ctx, uint8_t id, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)LIN_FRAME_COUNT, return -EINVAL);

    /* request */
    ctx->base->BIDR = 0;

    /* address & size */
    ctx->base->BIDR |= BIDR_ID(id);
    ctx->base->BIDR |= BIDR_DFL(n - 1);
    /* still classic checksum */
    ctx->base->BIDR |= BIDR_CCS;

    /* header transmission request */
    ctx->base->LINCR2 |= LINCR2_HTRQ;

    ctx->state = LIN_NXP_LINFLEXD_STATE_XFER;
    return -EAGAIN;
}

static int lin_read_as_master_xfer(struct lin *ctx, void *buf, size_t n)
{
#define MIN(a, b) ((a) < (b) ? (a) : (b))

    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)LIN_FRAME_COUNT, return -EINVAL);

    int res;
    int nread;

    /* check for errors */
    if ((ctx->base->LINESR & LINESR_CEF) != 0) {
        ctx->base->LINESR |= LINESR_CEF;
        ctx->state = LIN_NXP_LINFLEXD_STATE_IDLE;
        return -EAGAIN;
    }

    /* check for data reception done */
    if ((ctx->base->LINSR & LINSR_DRF) == 0)
        return -EAGAIN;

    /* get bytes read */
    nread = (int)((ctx->base->BIDR >> 10) & BIDR_DFL_M) + 1;
    nread = MIN(nread, (int)n);

    if ((res = copy_from_bdr(ctx, buf, (size_t)nread)) < 0)
        return res;

    /* clear flags */
    ctx->base->LINSR = (uint32_t)(LINSR_RMB | LINSR_DRBNE | LINSR_DRF);

    ctx->state = LIN_NXP_LINFLEXD_STATE_IDLE;
    return nread;
}

static int lin_read_as_master(struct lin *ctx, uint8_t id, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)LIN_FRAME_COUNT, return -EINVAL);

    switch (ctx->state) {
    case LIN_NXP_LINFLEXD_STATE_IDLE: return lin_read_as_master_idle(ctx, id, n);
    case LIN_NXP_LINFLEXD_STATE_XFER: return lin_read_as_master_xfer(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int lin_read_as_slave(struct lin *ctx, uint8_t *id, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)LIN_FRAME_COUNT, return -EINVAL);

    int nread = 0;

    /* wait for header */
    if ((ctx->base->LINSR & LINSR_HRF) == 0)
        return -EAGAIN;

    /* data reception */
    if ((ctx->base->LINSR & LINSR_RXBUSY) != 0)
        return -EAGAIN;

    /* get id */
    *id = (uint8_t)(ctx->base->BIDR & BIDR_ID_M);

    /* not an empty message */
    if ((ctx->base->LINSR & LINSR_DRF) != 0) {
        int res;
        /* get bytes read */
        nread = (int)((ctx->base->BIDR >> 10) & BIDR_DFL_M) + 1;
        nread = MIN(nread, (int)n);

        if ((res = copy_from_bdr(ctx, buf, (size_t)nread)) < 0)
            return res;

        /* clear flags */
        ctx->base->LINSR = (uint32_t)(LINSR_DRF | LINSR_HRF);
    }

    return nread;
}

int lin_read(struct lin *ctx, uint8_t *id, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)LIN_FRAME_COUNT, return -EINVAL);

    switch (ctx->mode) {
    case LIN_MODE_MASTER: return lin_read_as_master(ctx, *id, buf, n);
    case LIN_MODE_SLAVE: return lin_read_as_slave(ctx, id, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

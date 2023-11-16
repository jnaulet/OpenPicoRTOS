#include "can-gd32vf103.h"
#include "picoRTOS.h"

#include <stdint.h>

#define CAN_GD32VF103_TM_COUNT     3
#define CAN_GD32VF103_RFIFO_COUNT  2
#define CAN_GD32VF103_FILTER_COUNT 28

struct CAN_GD32VF103 {
    volatile uint32_t CAN_CTL;
    volatile uint32_t CAN_STAT;
    volatile uint32_t CAN_TSTAT;
    volatile uint32_t CAN_RFIFOx[CAN_GD32VF103_RFIFO_COUNT];
    volatile uint32_t CAN_INTEN;
    volatile uint32_t CAN_ERR;
    volatile uint32_t CAN_BT;
    uint32_t RESERVED0[88];
    struct CAN_GD32VF103_TM {
        volatile uint32_t CAN_TMIx;
        volatile uint32_t CAN_TMPx;
        volatile uint32_t CAN_TMDATA0x;
        volatile uint32_t CAN_TMDATA1x;
    } TM[CAN_GD32VF103_TM_COUNT];
    struct CAN_GD32VF103_RFIFO {
        volatile uint32_t CAN_RFIFOMIx;
        volatile uint32_t CAN_RFIFOMPx;
        volatile uint32_t CAN_RFIFOMDATA0x;
        volatile uint32_t CAN_RFIFOMDATA1x;
    } RFIFO[CAN_GD32VF103_RFIFO_COUNT];
    uint32_t RESERVED2[12];
    volatile uint32_t CAN_FCTL;             /* CAN0 only */
    volatile uint32_t CAN_FMCFG;            /* CAN0 only */
    uint32_t RESERVED3;
    volatile uint32_t CAN_FSCFG;            /* CAN0 only */
    uint32_t RESERVED4;
    volatile uint32_t CAN_FAFIFO;           /* CAN0 only */
    uint32_t RESERVED5;
    volatile uint32_t CAN_FW;               /* CAN0 only */
    uint32_t RESERVED6[8];
    struct CAN_GD32VF103_FILTER {
        volatile uint32_t CAN_FxDATAy[CAN_GD32VF103_RFIFO_COUNT];
    } FILTER[CAN_GD32VF103_FILTER_COUNT];
};

#define CAN_CTL_DFZ     (1 << 16)
#define CAN_CTL_SWRST   (1 << 15)
#define CAN_CTL_TTC     (1 << 7)
#define CAN_CTL_ABOR    (1 << 6)
#define CAN_CTL_AWU     (1 << 5)
#define CAN_CTL_ARD     (1 << 4)
#define CAN_CTL_RFOD    (1 << 3)
#define CAN_CTL_TFO     (1 << 2)
#define CAN_CTL_SLPWMOD (1 << 1)
#define CAN_CTL_IWMOD   (1 << 0)

#define CAN_STAT_RXL    (1 << 11)
#define CAN_STAT_LASTRX (1 << 10)
#define CAN_STAT_RS     (1 << 9)
#define CAN_STAT_TS     (1 << 8)
#define CAN_STAT_SLPIF  (1 << 4)
#define CAN_STAT_WUIF   (1 << 3)
#define CAN_STAT_ERRIF  (1 << 2)
#define CAN_STAT_SLPWS  (1 << 1)
#define CAN_STAT_IWS    (1 << 0)

#define CAN_TSTAT_TMLS2    (1u << 31)
#define CAN_TSTAT_TMLS1    (1 << 30)
#define CAN_TSTAT_TMSL0    (1 << 29)
#define CAN_TSTAT_TME2     (1 << 28)
#define CAN_TSTAT_TME1     (1 << 27)
#define CAN_TSTAT_TME0     (1 << 26)
#define CAN_TSTAT_NUM_M    0x3u
#define CAN_TSTAT_NUM(x)   (((x) & CAN_TSTAT_NUM_M) << 24)
#define CAN_TSTAT_MST2     (1 << 23)
#define CAN_TSTAT_MTE2     (1 << 19)
#define CAN_TSTAT_MAL2     (1 << 18)
#define CAN_TSTAT_MTFNERR2 (1 << 17)
#define CAN_TSTAT_MTF2     (1 << 16)
#define CAN_TSTAT_MST1     (1 << 15)
#define CAN_TSTAT_MTE1     (1 << 11)
#define CAN_TSTAT_MAL1     (1 << 10)
#define CAN_TSTAT_MTFNERR1 (1 << 9)
#define CAN_TSTAT_MTF1     (1 << 8)
#define CAN_TSTAT_MST0     (1 << 7)
#define CAN_TSTAT_MTE0     (1 << 3)
#define CAN_TSTAT_MAL0     (1 << 2)
#define CAN_TSTAT_MTFNERR0 (1 << 1)
#define CAN_TSTAT_MTF0     (1 << 0)

#define CAN_RFIFOx_RFDx    (1 << 5)
#define CAN_RFIFOx_RFOx    (1 << 4)
#define CAN_RFIFOx_RFFx    (1 << 3)
#define CAN_RFIFOx_RFLx_M  0x3u
#define CAN_RFIFOx_RFLx(x) ((x) & CAN_RFIFOx_RFLx_M)

#define CAN_ERR_RECNT_M  0xffu
#define CAN_ERR_RECNT(x) (((x) & CAN_ERR_RECNT_M) << 24)
#define CAN_ERR_TECNT_M  0xffu
#define CAN_ERR_TECNT(x) (((x) & CAN_ERR_TECNT_M) << 16)
#define CAN_ERR_ERRN_M   0x7u
#define CAN_ERR_ERRN(x) (((x) & CAN_ERR_ERRN_M) << 4)
#define CAN_ERR_BOERR   (1 << 2)
#define CAN_ERR_PERR    (1 << 1)
#define CAN_ERR_WERR    (1 << 0)

#define CAN_BT_SCMOD      (1u << 31)
#define CAN_BT_LCMOD      (1 << 30)
#define CAN_BT_SJW_M      0x3u
#define CAN_BT_SJW(x)     (((x) & CAN_BT_SJW_M) << 24)
#define CAN_BT_BS2_M      0x7u
#define CAN_BT_BS2(x)     (((x) & CAN_BT_BS2_M) << 20)
#define CAN_BT_BS1_M      0xfu
#define CAN_BT_BS1(x)     (((x) & CAN_BT_BS1_M) << 16)
#define CAN_BT_BAUDPSC_M  0x3ffu
#define CAN_BT_BAUDPSC(x) ((x) & CAN_BT_BAUDPSC_M)

#define CAN_TMIx_SFID_M  0x7ffu
#define CAN_TMIx_SFID(x) (((x) & CAN_TMIx_SFID_M) << 21)
#define CAN_TMIx_EFID_M  0x3ffffu
#define CAN_TMIx_EFID(x) (((x) & CAN_TMIx_EFID_M) << 3)
#define CAN_TMIx_FF      (1 << 2)
#define CAN_TMIx_FT      (1 << 1)
#define CAN_TMIx_TEN     (1 << 0)

#define CAN_TMPx_TS_M     0xffffu
#define CAN_TMPx_TS(x)    (((x) & CAN_TMPx_TS_M) << 16)
#define CAN_TMPx_TSEN     (1 << 8)
#define CAN_TMPx_DLENC_M  0xfu
#define CAN_TMPx_DLENC(x) ((x) & CAN_TMPx_DLENC_M)

#define CAN_RFIFOMIx_SFID_M  0x7ffu
#define CAN_RFIFOMIx_SFID(x) (((x) & CAN_RFIFOMIx_SFID_M) << 21)
#define CAN_RFIFOMIx_EFID_M  0x3ffffu
#define CAN_RFIFOMIx_EFID(x) (((x) & CAN_RFIFOMIx_EFID_M) << 3)
#define CAN_RFIFOMIx_FF      (1 << 2)
#define CAN_RFIFOMIx_FT      (1 << 1)

#define CAN_RFIFOMPx_TS_M     0xffffu
#define CAN_RFIFOMPx_TS(x)    (((x) & CAN_RFIFOMPx_TS_M) << 16)
#define CAN_RFIFOMPx_FI_M     0xffu
#define CAN_RFIFOMPx_FI(x)    (((x) & CAN_RFIFOMPx_FI_M) << 8)
#define CAN_RFIFOMPx_DLENC_M  0xfu
#define CAN_RFIFOMPx_DLENC(x) ((x) & CAN_RFIFOMPx_DLENC_M)

#define CAN_FCTL_HBC1F_M  0x3fu
#define CAN_FCTL_HBC1F(x) (((x) & CAN_FCTL_HBC1F_M) << 8)
#define CAN_FCTL_FLD      (1 << 0)

int can_gd32vf103_init(struct can *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct CAN_GD32VF103*)base;
    ctx->clkid = clkid;
    /* filters */
    ctx->filter_count = 0;

    return 0;
}

#define TQ_MIN 8
#define TQ_MAX 25

#define BS1_MIN 1
#define BS1_MAX 16
#define BS2_MIN 1
#define BS2_MAX 8

static int set_psc_and_seg(struct can *ctx, size_t psc, size_t tq)
{
#define div_round_closest(a, b) (((a) + ((b) >> 1)) / (b))

    picoRTOS_assert(tq >= (size_t)TQ_MIN, return -EINVAL);
    picoRTOS_assert(tq <= (size_t)TQ_MAX, return -EINVAL);

    size_t bs1;
    size_t bs2;

    /* -1 (account for SYNC_SEG) */
    tq -= 1;

    /* default sample point is at 87.5% : 64 / 73 = 0.876 */
    bs1 = div_round_closest(tq * 64, 73);

    /* compute propag */
    if (bs1 > (size_t)BS1_MAX)
        return -EINVAL;

    /* compute phase2 */
    bs2 = tq - bs1;

    /* ignore out of bounds values */
    if (bs1 < (size_t)BS1_MIN || bs1 > (size_t)BS1_MAX ||
        bs2 < (size_t)BS2_MIN || bs2 > (size_t)BS2_MAX)
        return -EINVAL;

    ctx->base->CAN_BT = (uint32_t)(CAN_BT_SJW(1) |
                                   CAN_BT_BS1(bs1 - 1) |
                                   CAN_BT_BS2(bs2 - 1) |
                                   CAN_BT_BAUDPSC(psc));

    return 0;
}

static int set_bitrate(struct can *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    size_t psc;
    clock_freq_t freq;

    if ((freq = clock_get_freq(ctx->clkid)) < 0)
        return (int)freq;

    picoRTOS_assert(((unsigned long)freq % bitrate) == 0,
                    return -EIO);

    /* start iterative detection of params */
    for (psc = (size_t)1; psc <= (size_t)CAN_BT_BAUDPSC_M; psc++) {
        size_t tq_div = (psc * (size_t)bitrate);
        size_t tq = (size_t)freq / tq_div;

        /* check for accuracy */
        if (((size_t)freq % tq_div) != 0)
            continue;

        /* ignore out of bounds values */
        if (tq < (size_t)TQ_MIN || tq > (size_t)TQ_MAX)
            continue;

        /* set values */
        if (set_psc_and_seg(ctx, psc, tq) == 0)
            return 0;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EINVAL;
}

static int sleep_to_initial_working_mode(struct can *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    ctx->base->CAN_CTL |= CAN_CTL_IWMOD;
    ctx->base->CAN_CTL &= ~CAN_CTL_SLPWMOD;

    while (deadlock-- != 0)
        if ((ctx->base->CAN_STAT & CAN_STAT_IWS) != 0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

int can_setup(struct can *ctx, struct can_settings *settings)
{
    int res;

    picoRTOS_assert(settings->tx_auto_abort < CAN_TX_AUTO_ABORT_COUNT, return -EINVAL);
    picoRTOS_assert(settings->rx_overwrite < CAN_RX_OVERWRITE_COUNT, return -EINVAL);
    picoRTOS_assert(settings->loopback < CAN_LOOPBACK_COUNT, return -EINVAL);

    if ((res = sleep_to_initial_working_mode(ctx)) < 0 ||
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if (settings->tx_auto_abort != CAN_TX_AUTO_ABORT_OFF) ctx->base->CAN_CTL |= CAN_CTL_ABOR;
    else ctx->base->CAN_CTL &= ~CAN_CTL_ABOR;

    if (settings->rx_overwrite != CAN_RX_OVERWRITE_OFF) ctx->base->CAN_CTL &= ~CAN_CTL_RFOD;
    else ctx->base->CAN_CTL |= CAN_CTL_RFOD;

    /* loopback */
    if (settings->loopback != CAN_LOOPBACK_OFF) ctx->base->CAN_BT |= CAN_BT_LCMOD;
    else ctx->base->CAN_BT &= ~CAN_BT_LCMOD;

    /* ignore tx_mailboxes (set to 3 by hw) */

    /* normal mode */
    ctx->base->CAN_CTL &= ~CAN_CTL_IWMOD;

    return 0;
}

int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask)
{
    picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT, return -EINVAL);
    picoRTOS_assert(accept_mask < (can_id_t)CAN_EXTID_COUNT, return -EINVAL);
    picoRTOS_assert(ctx->filter_count < (size_t)CAN_GD32VF103_FILTER_COUNT, return -ENOMEM);

    size_t rfifo_index = (size_t)(ctx->filter_count % CAN_GD32VF103_RFIFO_COUNT);

    /* 32bit mask mode only */
    ctx->base->CAN_FMCFG &= ~(1 << ctx->filter_count);
    ctx->base->CAN_FSCFG |= (1 << ctx->filter_count);
    /* arbitratrily choose rfifo */
    ctx->base->CAN_FAFIFO |= (1 << rfifo_index);

    /* set filter, at last. FIXME */
    ctx->base->FILTER[ctx->filter_count].CAN_FxDATAy[rfifo_index] =
        (uint32_t)CAN_RFIFOMIx_SFID(accept_mask);

    /* enable filter */
    ctx->base->CAN_FW |= (1 << ctx->filter_count++);

    return 0;
}

static int transfer_tx_mailbox(struct can *ctx, size_t index,
                               can_id_t id, const void *buf, size_t n)
{
    picoRTOS_assert(index < (size_t)CAN_GD32VF103_TM_COUNT, return -EINVAL);
    picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    const uint8_t *buf8 = (const uint8_t*)buf;
    struct CAN_GD32VF103_TM *c99_tm = &ctx->base->TM[index];

    /* reset buffer */
    c99_tm->CAN_TMDATA0x = 0;
    c99_tm->CAN_TMDATA1x = 0;

    switch (n) {
    case 8: c99_tm->CAN_TMDATA1x |= (uint32_t)buf8[7] << 24;    /*@fallthrough@*/
    case 7: c99_tm->CAN_TMDATA1x |= (uint32_t)buf8[6] << 16;    /*@fallthrough@*/
    case 6: c99_tm->CAN_TMDATA1x |= (uint32_t)buf8[5] << 8;     /*@fallthrough@*/
    case 5: c99_tm->CAN_TMDATA1x |= (uint32_t)buf8[4] << 0;     /*@fallthrough@*/
    case 4: c99_tm->CAN_TMDATA0x |= (uint32_t)buf8[3] << 24;    /*@fallthrough@*/
    case 3: c99_tm->CAN_TMDATA0x |= (uint32_t)buf8[2] << 16;    /*@fallthrough@*/
    case 2: c99_tm->CAN_TMDATA0x |= (uint32_t)buf8[1] << 8;     /*@fallthrough@*/
    case 1: c99_tm->CAN_TMDATA0x |= (uint32_t)buf8[0] << 0; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    /* only 11bit can for now */
    c99_tm->CAN_TMPx = (uint32_t)CAN_TMPx_DLENC(n);
    c99_tm->CAN_TMIx = (uint32_t)(CAN_TMIx_SFID(id) | CAN_TMIx_TEN);

    return (int)n;
}

int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n)
{
    picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    if ((ctx->base->CAN_TSTAT & CAN_TSTAT_TME0) != 0)
        return transfer_tx_mailbox(ctx, (size_t)0, id, buf, n);

    if ((ctx->base->CAN_TSTAT & CAN_TSTAT_TME1) != 0)
        return transfer_tx_mailbox(ctx, (size_t)1, id, buf, n);

    if ((ctx->base->CAN_TSTAT & CAN_TSTAT_TME2) != 0)
        return transfer_tx_mailbox(ctx, (size_t)2, id, buf, n);

    /* FIFOs are full */
    return -EAGAIN;
}

static int transfer_rx_mailbox(struct can *ctx, size_t index,
                               can_id_t *id, void *buf, size_t n)
{
    picoRTOS_assert(index < (size_t)CAN_GD32VF103_RFIFO_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    uint8_t *buf8 = (uint8_t*)buf;
    struct CAN_GD32VF103_RFIFO *rfifo = &ctx->base->RFIFO[index];

    switch (n) {
    case 8: buf8[7] = (uint8_t)(rfifo->CAN_RFIFOMDATA1x >> 24);     /*@fallthrough@*/
    case 7: buf8[6] = (uint8_t)(rfifo->CAN_RFIFOMDATA1x >> 16);     /*@fallthrough@*/
    case 6: buf8[5] = (uint8_t)(rfifo->CAN_RFIFOMDATA1x >> 8);      /*@fallthrough@*/
    case 5: buf8[4] = (uint8_t)(rfifo->CAN_RFIFOMDATA1x >> 0);      /*@fallthrough@*/
    case 4: buf8[3] = (uint8_t)(rfifo->CAN_RFIFOMDATA0x >> 24);     /*@fallthrough@*/
    case 3: buf8[2] = (uint8_t)(rfifo->CAN_RFIFOMDATA0x >> 16);     /*@fallthrough@*/
    case 2: buf8[1] = (uint8_t)(rfifo->CAN_RFIFOMDATA0x >> 8);      /*@fallthrough@*/
    case 1: buf8[0] = (uint8_t)(rfifo->CAN_RFIFOMDATA0x >> 0); break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    /* get id & dlc */
    *id = (can_id_t)(rfifo->CAN_RFIFOMIx >> 21);
    n = (size_t)(rfifo->CAN_RFIFOMPx & CAN_RFIFOMPx_DLENC_M);

    /* mark as processed */
    ctx->base->CAN_RFIFOx[index] |= CAN_RFIFOx_RFDx;
    return (int)n;
}

int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    if ((ctx->base->RFIFO[0].CAN_RFIFOMIx & CAN_RFIFOx_RFLx_M) != 0)
        return transfer_rx_mailbox(ctx, (size_t)0, id, buf, n);

    if ((ctx->base->RFIFO[1].CAN_RFIFOMIx & CAN_RFIFOx_RFLx_M) != 0)
        return transfer_rx_mailbox(ctx, (size_t)1, id, buf, n);

    /* no frame */
    return -EAGAIN;
}

int can_request_frame(/*@unused@*/ struct can *ctx __attribute__((unused)),
                      /*@unused@*/ can_id_t id __attribute__((unused)))
{
    return -ENOSYS;
}

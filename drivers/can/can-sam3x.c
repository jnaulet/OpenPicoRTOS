#include "can-sam3x.h"
#include "picoRTOS.h"

#include <stdint.h>

struct CAN_SAM3X {
    volatile uint32_t CAN_MR;
    volatile uint32_t CAN_IER;
    volatile uint32_t CAN_IDR;
    volatile uint32_t CAN_IMR;
    volatile uint32_t CAN_SR;
    volatile uint32_t CAN_BR;
    volatile uint32_t CAN_TIM;
    volatile uint32_t CAN_TIMESTP;
    volatile uint32_t CAN_ECR;
    volatile uint32_t CAN_TCR;
    volatile uint32_t CAN_ACR;
    uint32_t RESERVED0[46];
    volatile uint32_t CAN_WPMR;
    volatile uint32_t CAN_WPSR;
    uint32_t RESERVED1[69];
    struct CAN_SAM3X_MB {
        volatile uint32_t CAN_MMR;
        volatile uint32_t CAN_MAM;
        volatile uint32_t CAN_MID;
        volatile uint32_t CAN_MFID;
        volatile uint32_t CAN_MSR;
        volatile uint32_t CAN_MDL;
        volatile uint32_t CAN_MDH;
        volatile uint32_t CAN_MCR;
    } MB[CAN_SAM3X_MB_COUNT];
};

#define CAN_MR_DRPT   (1 << 7)
#define CAN_MR_TIMFRZ (1 << 6)
#define CAN_MR_TTM    (1 << 5)
#define CAN_MR_TEOF   (1 << 4)
#define CAN_MR_OVL    (1 << 3)
#define CAN_MR_ABM    (1 << 2)
#define CAN_MR_LPM    (1 << 1)
#define CAN_MR_CANEN  (1 << 0)

#define CAN_BR_SMP       (1 << 24)
#define CAN_BR_BRP_M     0x7fu
#define CAN_BR_BRP(x)    (((x) & CAN_BR_BRP_M) << 16)
#define CAN_BR_SJW_M     0x3u
#define CAN_BR_SJW(x)    (((x) & CAN_BR_SJW_M) << 12)
#define CAN_BR_PROPAG_M  0x7u
#define CAN_BR_PROPAG(x) (((x) & CAN_BR_PROPAG_M) << 8)
#define CAN_BR_PHASE1_M  0x7u
#define CAN_BR_PHASE1(x) (((x) & CAN_BR_PHASE1_M) << 4)
#define CAN_BR_PHASE2_M  0x7u
#define CAN_BR_PHASE2(x) ((x) & CAN_BR_PHASE2_M)

#define CAN_MMRn_MOT_M  0x7u
#define CAN_MMRn_MOT(x) (((x) & CAN_MMRn_MOT_M) << 24)

#define CAN_MMRn_MOT_DISABLED        0
#define CAN_MMRn_MOT_MB_RX           1
#define CAN_MMRn_MOT_MB_RX_OVERWRITE 2
#define CAN_MMRn_MOT_MB_TX           3
#define CAN_MMRn_MOT_MB_CONSUMER     4
#define CAN_MMRn_MOT_MB_PRODUCER     5

#define CAN_MIDn_MIDE     (1 << 29)
#define CAN_MIDn_MIDvA_M  0x7ffu
#define CAN_MIDn_MIDvA(x) (((x) & CAN_MIDn_MIDvA_M) << 18)
#define CAN_MIDn_MIDvB_M  0x3fffu
#define CAN_MIDn_MIDvB(x) ((x) & CAN_MIDn_MIDvB_M)

#define CAN_MSRn_MRDY (1 << 23)

#define CAN_MCRn_MTRC        (1 << 23)
#define CAN_MCRn_MACR        (1 << 22)
#define CAN_MCRn_MRTR        (1 << 20)
#define CAN_MCRn_MDLC_M      0xfu
#define CAN_MCRn_MDLC(x)     (((x) & CAN_MCRn_MDLC_M) << 16)
#define CAN_MCRn_MDLC_GET(x) (((x) >> 16) & CAN_MCRn_MDLC_M)

static int set_tx_mailboxes(/*@partial@*/ struct can *ctx, size_t n)
{
    if (!picoRTOS_assert(n < (size_t)CAN_SAM3X_MB_COUNT)) return -EINVAL;

    ctx->tx_mailbox_count = n;
    ctx->rx_mailbox_count = (size_t)(CAN_SAM3X_MB_COUNT - n);

    while (n-- != 0)
        ctx->base->MB[ctx->rx_mailbox_count + n].CAN_MMR =
            (uint32_t)CAN_MMRn_MOT(CAN_MMRn_MOT_MB_TX);

    return 0;
}

int can_sam3x_init(struct can *ctx, struct CAN_SAM3X *base, clock_id_t clkid)
{
    ctx->base = base;
    ctx->clkid = clkid;
    /* mb allocation */
    ctx->rx_mailbox_index = 0;
    ctx->tx_mailbox_index = 0;
    /* default settings */
    ctx->tx_auto_abort = true;
    ctx->rx_overwrite = true;

    /* ensure can is disabled before setup */
    ctx->base->CAN_MR = (uint32_t)0;

    /* setup mailboxes */
    return set_tx_mailboxes(ctx, (size_t)1);
}

#define TQ_MIN 8
#define TQ_MAX 25

#define PROPAG_MIN 1
#define PROPAG_MAX 8
#define PHASE1_MIN 1
#define PHASE1_MAX 8
#define PHASE2_MIN 1
#define PHASE2_MAX 8

static int set_brp_and_prop(struct can *ctx, size_t brp, size_t tq)
{
#define div_round_closest(a, b) (((a) + ((b) >> 1)) / (b))

    if (!picoRTOS_assert(tq >= (size_t)TQ_MIN)) return -EINVAL;
    if (!picoRTOS_assert(tq <= (size_t)TQ_MAX)) return -EINVAL;

    size_t phase1;
    size_t phase2;
    size_t propag = 0;

    /* -1 (account for SYNC_SEG) */
    tq -= 1;

    /* default sam3xple point is at 87.5% : 64 / 73 = 0.876 */
    phase1 = div_round_closest(tq * 64, 73);

    /* compute propag */
    if (phase1 > (size_t)PHASE1_MAX) {
        propag = phase1 - (size_t)PHASE1_MAX;
        phase1 -= propag;
    }

    /* check max propag */
    if (propag > (size_t)PROPAG_MAX)
        return -EINVAL;

    /* compute phase2 */
    phase2 = tq - (propag + phase1);

    /* ignore out of bounds values */
    if (propag < (size_t)PROPAG_MIN || propag > (size_t)PROPAG_MAX ||
        phase1 < (size_t)PHASE1_MIN || phase1 > (size_t)PHASE1_MAX ||
        phase2 < (size_t)PHASE2_MIN || phase2 > (size_t)PHASE2_MAX)
        return -EINVAL;

    ctx->base->CAN_BR = (uint32_t)(CAN_BR_BRP(brp - 1) |
                                   CAN_BR_SJW(0) |
                                   CAN_BR_PROPAG(propag - 1) |
                                   CAN_BR_PHASE1(phase1 - 1) |
                                   CAN_BR_PHASE2(phase2 - 1));

    return 0;
}

static int set_bitrate(struct can *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    size_t brp;
    clock_freq_t freq;

    if ((freq = clock_get_freq(ctx->clkid)) < 0)
        return (int)freq;

    if (!picoRTOS_assert(((unsigned long)freq % bitrate) == 0))
        return -EIO;

    /* start iterative detection of params */
    for (brp = (size_t)1; brp <= (size_t)(CAN_BR_BRP_M + 1u); brp++) {
        size_t tq_div = (brp * (size_t)bitrate);
        size_t tq = (size_t)freq / tq_div;

        /* check for accuracy */
        if (((size_t)freq % tq_div) != 0)
            continue;

        /* ignore out of bounds values */
        if (tq < (size_t)TQ_MIN || tq > (size_t)TQ_MAX)
            continue;

        /* set values */
        if (set_brp_and_prop(ctx, brp, tq) == 0)
            return 0;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EINVAL;
}

int can_setup(struct can *ctx, struct can_settings *settings)
{
    int res;

    /* disable */
    ctx->base->CAN_MR &= ~CAN_MR_CANEN;

    if ((res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* set tx mailboxes */
    if ((res = set_tx_mailboxes(ctx, settings->tx_mailbox_count)) < 0)
        return res;

    ctx->tx_auto_abort = settings->tx_auto_abort;
    ctx->rx_overwrite = settings->rx_overwrite;

    /* ignore loopback */

    /* enable */
    ctx->base->CAN_MR |= CAN_MR_CANEN;
    return 0;
}

static int mailbox_config_mid(struct CAN_SAM3X_MB *mb, can_id_t id)
{
    if (!picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;

    if (id < (can_id_t)CAN_ID_COUNT)
        mb->CAN_MID = (uint32_t)CAN_MIDn_MIDvA(id);
    else{
        mb->CAN_MID = (uint32_t)CAN_MIDn_MIDE;
        mb->CAN_MID |= (CAN_MIDn_MIDvA(id >> 17) | CAN_MIDn_MIDvB(id));
    }

    return 0;
}

static int mailbox_config_mam(struct CAN_SAM3X_MB *mb, can_id_t id, can_id_t mask)
{
    if (!picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(mask < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;

    if (id < (can_id_t)CAN_ID_COUNT)
        mb->CAN_MAM = (uint32_t)CAN_MIDn_MIDvA(mask);
    else{
        mb->CAN_MAM = (uint32_t)CAN_MIDn_MIDE;
        mb->CAN_MAM |= (CAN_MIDn_MIDvA(mask >> 17) | CAN_MIDn_MIDvB(mask));
    }

    return 0;
}

int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask)
{
    if (!picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(accept_mask < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(ctx->rx_mailbox_index < ctx->rx_mailbox_count)) return -ENOMEM;

    int res;
    struct CAN_SAM3X_MB *mb = &ctx->base->MB[ctx->rx_mailbox_index];

    /* disable mailbox */
    mb->CAN_MMR &= ~CAN_MMRn_MOT(CAN_MMRn_MOT_M);

    /* set MID anyway */
    if ((res = mailbox_config_mid(mb, id)) < 0 ||
        (res = mailbox_config_mam(mb, id, accept_mask)) < 0)
        return res;

    /* re-enable mailbox */
    if (!ctx->rx_overwrite) mb->CAN_MMR |= CAN_MMRn_MOT(CAN_MMRn_MOT_MB_RX);
    else mb->CAN_MMR |= CAN_MMRn_MOT(CAN_MMRn_MOT_MB_RX_OVERWRITE);

    /* start rx */
    mb->CAN_MCR = (uint32_t)CAN_MCRn_MTRC;
    ctx->rx_mailbox_index++;

    return 0;
}

static int transfer_tx_mailbox(struct can *ctx, size_t index,
                               can_id_t id, const void *buf, size_t n)
{
    if (!picoRTOS_assert(index < (size_t)CAN_SAM3X_MB_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT)) return -EINVAL;

    int res;
    const uint8_t *buf8 = (const uint8_t*)buf;
    struct CAN_SAM3X_MB *mb = &ctx->base->MB[index];

    /* double-check */
    if ((mb->CAN_MSR & CAN_MSRn_MRDY) == 0)
        return -EAGAIN;

    /* disable mailbox & set MID */
    mb->CAN_MMR &= ~CAN_MMRn_MOT(CAN_MMRn_MOT_M);

    if ((res = mailbox_config_mid(mb, id)) < 0 ||
        (res = mailbox_config_mam(mb, id, 0)) < 0)
        return res;

    /* reset buffer */
    mb->CAN_MDL = 0;
    mb->CAN_MDH = 0;

    switch (n) {
    case 8: mb->CAN_MDH |= (uint32_t)buf8[7] << 0;      /*@fallthrough@*/
    case 7: mb->CAN_MDH |= (uint32_t)buf8[6] << 8;      /*@fallthrough@*/
    case 6: mb->CAN_MDH |= (uint32_t)buf8[5] << 16;     /*@fallthrough@*/
    case 5: mb->CAN_MDH |= (uint32_t)buf8[4] << 24;     /*@fallthrough@*/
    case 4: mb->CAN_MDL |= (uint32_t)buf8[3] << 0;      /*@fallthrough@*/
    case 3: mb->CAN_MDL |= (uint32_t)buf8[2] << 8;      /*@fallthrough@*/
    case 2: mb->CAN_MDL |= (uint32_t)buf8[1] << 16;     /*@fallthrough@*/
    case 1: mb->CAN_MDL |= (uint32_t)buf8[0] << 24; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    /* start xfer */
    mb->CAN_MMR |= CAN_MMRn_MOT(CAN_MMRn_MOT_MB_TX);
    mb->CAN_MCR = (uint32_t)(CAN_MCRn_MTRC | CAN_MCRn_MDLC(n));
    return (int)n;
}

int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n)
{
    if (!picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT)) return -EINVAL;

    /* get next index */
    size_t index = ctx->rx_mailbox_count + ctx->tx_mailbox_index;
    uint8_t mask = (uint8_t)(1 << index);

    if ((ctx->base->CAN_SR & mask) == 0) {
        if (ctx->tx_auto_abort) ctx->base->CAN_ACR |= mask;
        return -EAGAIN;
    }

    /* compute next index & transfer */
    ctx->tx_mailbox_index = (ctx->tx_mailbox_index + 1) % ctx->tx_mailbox_count;
    return transfer_tx_mailbox(ctx, index, id, buf, n);
}

static can_id_t mailbox_read_mid(struct CAN_SAM3X_MB *mb)
{
    uint32_t mid = mb->CAN_MID;

    if ((mid & CAN_MIDn_MIDE) != 0)
        return (can_id_t)(mid & ~CAN_MIDn_MIDE);

    return (can_id_t)(mid >> 18);
}

static int transfer_rx_mailbox(struct can *ctx, size_t index,
                               can_id_t *id, void *buf, size_t n)
{
    if (!picoRTOS_assert(index < (size_t)CAN_SAM3X_MB_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT)) return -EINVAL;

    uint8_t *buf8 = (uint8_t*)buf;
    struct CAN_SAM3X_MB *mb = &ctx->base->MB[index];

    if ((mb->CAN_MSR & CAN_MSRn_MRDY) == 0)
        return -EAGAIN;

    switch (n) {
    case 8: buf8[7] = (uint8_t)(mb->CAN_MDH >> 0);      /*@fallthrough@*/
    case 7: buf8[6] = (uint8_t)(mb->CAN_MDH >> 8);      /*@fallthrough@*/
    case 6: buf8[5] = (uint8_t)(mb->CAN_MDH >> 16);     /*@fallthrough@*/
    case 5: buf8[4] = (uint8_t)(mb->CAN_MDH >> 24);     /*@fallthrough@*/
    case 4: buf8[3] = (uint8_t)(mb->CAN_MDL >> 0);      /*@fallthrough@*/
    case 3: buf8[2] = (uint8_t)(mb->CAN_MDL >> 8);      /*@fallthrough@*/
    case 2: buf8[1] = (uint8_t)(mb->CAN_MDL >> 16);     /*@fallthrough@*/
    case 1: buf8[0] = (uint8_t)(mb->CAN_MDL >> 24); break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    /* get id */
    *id = mailbox_read_mid(mb);
    n = (size_t)CAN_MCRn_MDLC_GET(mb->CAN_MCR);

    /* reset xfer */
    mb->CAN_MCR = (uint32_t)CAN_MCRn_MTRC;
    return (int)n;
}

int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT)) return -EINVAL;

    size_t index = ctx->rx_mailbox_index;
    uint8_t mask = (uint8_t)((1 << ctx->rx_mailbox_count) - 1);
    uint32_t mrdy = ctx->base->CAN_SR & mask;

    if (mrdy == 0)
        return -EAGAIN;

    while (index-- != 0)
        if ((mrdy & (1 << index)) != 0)
            return transfer_rx_mailbox(ctx, index, id, buf, n);

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int can_request_frame(/*@unused@*/ struct can *ctx __attribute__((unused)),
                      /*@unused@*/ can_id_t id __attribute__((unused)))
{
    return -ENOSYS;
}

#include "can-nxp_flexcan.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct CAN_NXP_FLEXCAN {
    volatile uint32_t MCR;
    volatile uint32_t CTRL1;
    volatile uint32_t TIMER;
    uint32_t RESERVED0;
    volatile uint32_t RXMGMASK;
    volatile uint32_t RX14MASK;
    volatile uint32_t RX15MASK;
    volatile uint32_t ECR;
    volatile uint32_t ESR1;
    volatile uint32_t IMASK2;
    volatile uint32_t IMASK1;
    volatile uint32_t IFLAG2;
    volatile uint32_t IFLAG1;
    volatile uint32_t CTRL2;
    volatile uint32_t ESR2;
    uint32_t RESERVED1[2];
    volatile uint32_t CRCR;
    volatile uint32_t RXFGMASK;
    volatile uint32_t RXFIR;
    volatile uint32_t CBT;
    uint32_t RESERVED2[6];
    volatile uint32_t IMASK3;
    uint32_t RESERVED3;
    volatile uint32_t IFLAG3;
    uint32_t RESERVED4[2];
    struct CAN_NXP_FLEXCAN_MB {
        volatile uint32_t CS;
        volatile uint32_t ID;
        volatile uint32_t DATAL;
        volatile uint32_t DATAH;
    } MB[128];
    volatile uint32_t RXIMR[128];
    uint32_t RESERVED5[24];
    volatile uint32_t MECR;
    volatile uint32_t ERRIAR;
    volatile uint32_t ERRIDPR;
    volatile uint32_t ERRIPPR;
    volatile uint32_t RERRAR;
    volatile uint32_t RERRDR;
    volatile uint32_t RERRSYNR;
    volatile uint32_t ERRSR;
    volatile uint32_t CTRL1_PN;
    volatile uint32_t CTRL2_PN;
    volatile uint32_t WU_MTC;
    volatile uint32_t FLT_ID1;
    volatile uint32_t FLT_DLC;
    volatile uint32_t PL1_LO;
    volatile uint32_t PL1_HI;
    volatile uint32_t FLT_ID2_IDMASK;
    volatile uint32_t PL2_PLMASK_LO;
    volatile uint32_t PL2_PLMASK_HI;
    uint32_t RESERVED6[6];
    struct {
        volatile uint32_t WMBn_CS;
        volatile uint32_t WMBn_ID;
        volatile uint32_t WMBn_D03;
        volatile uint32_t WMBn_D47;
    } WMB[4]; /* max: 12 */
    uint32_t RESERVED7[32];
    volatile uint32_t FDCTRL;
    volatile uint32_t FDCBT;
    volatile uint32_t FDCRC;
};

#define MCR_MDIS     (1u << 31)
#define MCR_FRZ      (1 << 30)
#define MCR_HALT     (1 << 28)
#define MCR_NOTRDY   (1 << 27)
#define MCR_SOFTRST  (1 << 25)
#define MCR_FRZACK   (1 << 24)
#define MCR_SRXDIS   (1 << 17)
#define MCR_AEN      (1 << 12)
#define MCR_SUPV     (1 << 23)
#define MCR_MAXMB_M  0x7fu
#define MCR_MAXMB(x) ((x) & MCR_MAXMB_M)

#define CTRL1_PRESDIV_M  0xffu
#define CTRL1_PRESDIV(x) (((x) & CTRL1_PRESDIV_M) << 24)
#define CTRL1_RJW_M      0x3u
#define CTRL1_RJW(x)     (((x) & CTRL1_RJW_M) << 22)
#define CTRL1_PSEG1_M    0x7u
#define CTRL1_PSEG1(x)   (((x) & CTRL1_PSEG1_M) << 19)
#define CTRL1_PSEG2_M    0x7u
#define CTRL1_PSEG2(x)   (((x) & CTRL1_PSEG2_M) << 16)
#define CTRL1_CLKSRC     (1 << 13)
#define CTRL1_LPB        (1 << 12)
#define CTRL1_PROPSEG_M  0x7u
#define CTRL1_PROPSEG(x) ((x) & CTRL1_PROPSEG_M)

#define RXMGMASK_RTR  (1u << 31)
#define RXMGMASK_IDE  (1 << 30)
#define RXMGMASK_ID_M 0xfffffffu
#define RXGMASK_ID(x) ((x) & RXMGMASK_ID_M)

#define CTRL2_ECRWRE (1 << 29)
#define CTRL2_RRS    (1 << 17)

#define MECR_ECRWRDIS (1u << 31)
#define MECR_NCEFAFRZ (1 << 7)

typedef enum {
    CS_CODE_RX_INACTIVE = 0x0,
    CS_CODE_RX_EMPTY    = 0x4,
    CS_CODE_RX_FULL     = 0x2,
    CS_CODE_RX_OVERRUN  = 0x6,
    CS_CODE_RX_RANSWER  = 0xa,
    CS_CODE_RX_BUSY     = 0x1,
    CS_CODE_TX_INACTIVE = 0x8,
    CS_CODE_TX_ABORT    = 0x9,
    CS_CODE_TX_DATA     = 0xc,
    CS_CODE_TX_REMOTE   = 0xc,
    CS_CODE_TX_TANSWER  = 0xe
} CS_CODE_t;

#define CS_EDL               (1u << 31)
#define CS_BRS               (1 << 30)
#define CS_ESI               (1 << 29)
#define CS_CODE_M            0xfu
#define CS_CODE(x)           (((x) & CS_CODE_M) << 24)
#define CS_CODE_GET(x)       (((x) >> 24) & CS_CODE_M)
#define CS_SRR               (1 << 22)
#define CS_IDE               (1 << 21)
#define CS_RTR               (1 << 20)
#define CS_DLC_M             0xfu
#define CS_DLC(x)            (((x) & CS_DLC_M) << 16)
#define CS_DLC_GET(x)        (((x) >> 16) & CS_DLC_M)
#define CS_TIMESTAMP_M       0xffffu
#define CS_TIMETSTAMP_GET(x) ((x) & CS_TIMESTAMP_M)

#define ID_PRIO_M   0x3u
#define ID_PRIO(x)  (((x) & ID_PRIO_M) << 29)
#define ID_ID_M     0x7ffu
#define ID_ID(x)    (((x) & ID_ID_M) << 18)
#define ID_IDEXT_M  0x1fffffffu
#define ID_IDEXT(x) ((x) & ID_IDEXT_M)

/* flag management */

static bool nxp_flexcan_read_mb_flag(struct can *ctx, size_t index)
{
    picoRTOS_assert_void(index < ctx->mailbox_count);

    size_t iflag = (index >> 5); /* div by 32 */
    uint32_t mask = (uint32_t)(1u << (0x1fu & index));

    switch (iflag) {
    case 0: return (ctx->base->IFLAG1 & mask) != 0;
    case 1: return (ctx->base->IFLAG2 & mask) != 0;
    case 2: return (ctx->base->IFLAG3 & mask) != 0;
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return false;
}

static void nxp_flexcan_clear_mb_flag(struct can *ctx, size_t index)
{
    picoRTOS_assert_void(index < ctx->mailbox_count);

    size_t iflag = (index >> 5); /* div by 32 */
    uint32_t mask = (uint32_t)(1u << (0x1fu & index));

    switch (iflag) {
    case 0: ctx->base->IFLAG1 = mask; break;
    case 1: ctx->base->IFLAG2 = mask; break;
    case 2: ctx->base->IFLAG3 = mask; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        break;
    }
}

/* clocking system */

#define TQ_MIN      16
#define TQ_MAX      25
#define PROPSEG_MIN 1
#define PROPSEG_MAX 8
#define PSEG1_MIN   1
#define PSEG1_MAX   8
#define PSEG2_MIN   1
#define PSEG2_MAX   8
#define RJW_MAX     4

static int nxp_flexcan_set_ctrl1(struct can *ctx, size_t brp, size_t tq)
{
#define div_round_closest(a, b) (((a) + ((b) >> 1)) / (b))

    picoRTOS_assert(brp > 0, return -EINVAL);
    picoRTOS_assert(brp <= (size_t)(CTRL1_PRESDIV_M + 1u), return -EINVAL);
    picoRTOS_assert(tq >= (size_t)TQ_MIN, return -EINVAL);
    picoRTOS_assert(tq <= (size_t)TQ_MAX, return -EINVAL);

    size_t pseg1;
    size_t pseg2;
    size_t propseg = (size_t)PROPSEG_MIN;

    /* SYNC_SEG */
    tq -= 1;

    /* default sample point is at 87.5% : 64 / 73 = 0.876 */
    pseg1 = div_round_closest(tq * 64, 73);

    /* compute propseg */
    if (pseg1 > (size_t)PSEG1_MAX) {
        propseg = pseg1 - (size_t)PSEG1_MAX;
        pseg1 -= propseg;
    }

    /* check propseg */
    if (propseg > (size_t)PROPSEG_MAX)
        return -EINVAL;

    /* compute pseg2 */
    pseg2 = tq - (propseg + pseg1);

    /* ignore out of bounds values */
    if (propseg < (size_t)PROPSEG_MIN || propseg > (size_t)PROPSEG_MAX ||
        pseg1 < (size_t)PSEG1_MIN || pseg1 > (size_t)PSEG1_MAX ||
        pseg2 < (size_t)PSEG2_MIN || pseg2 > (size_t)PSEG2_MAX)
        return -EINVAL;

    /* set CTRL1 */
    ctx->base->CTRL1 &= CTRL1_CLKSRC; /* clear all except CLKSRC */
    ctx->base->CTRL1 |= (CTRL1_PRESDIV(brp - 1) |
                         CTRL1_RJW(0) |
                         CTRL1_PROPSEG(propseg - 1) |
                         CTRL1_PSEG1(pseg1 - 1) |
                         CTRL1_PSEG2(pseg2 - 1));

    return 0;
}

static int nxp_flexcan_set_bitrate(struct can *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    size_t brp;
    clock_freq_t freq;

    if ((freq = clock_get_freq(ctx->clkid)) < 0)
        return (int)freq;

    /* check if freq is a multiple of bitrate */
    picoRTOS_assert(((unsigned long)freq % bitrate) == 0,
                    return -EIO);

    /* start iterative detection of params */
    for (brp = (size_t)1; brp <= (size_t)(CTRL1_PRESDIV_M + 1u); brp++) {
        size_t tq_div = (brp * (size_t)bitrate);
        size_t tq = (size_t)freq / tq_div;

        /* check for accuracy */
        if (((size_t)freq % tq_div) != 0)
            continue;

        /* ignore out of bounds values */
        if (tq < (size_t)TQ_MIN || tq > (size_t)TQ_MAX)
            continue;

        if (nxp_flexcan_set_ctrl1(ctx, brp, tq) == 0)
            return 0;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EINVAL;
}

static int nxp_flexcan_set_tx_mailboxes(/*@partial@*/ struct can *ctx, size_t n)
{
    picoRTOS_assert(n < (size_t)ctx->mailbox_count, return -EINVAL);

    ctx->tx_mailbox_count = n;
    ctx->rx_mailbox_count = (size_t)(ctx->mailbox_count - n);

    while (n-- != 0)
        /* write the INACTIVE code to CS to enable */
        ctx->base->MB[ctx->rx_mailbox_count + n].CS =
            (uint32_t)CS_CODE(CS_CODE_TX_INACTIVE);

    return 0;
}

#ifdef CAN_NXP_FLEXCAN_DISABLE_MEM_ERR_DETECTION
static void nxp_flexcan_disable_mem_err_detection(struct can *ctx)
{
    /* unlock MECR */
    ctx->base->CTRL2 |= CTRL2_ECRWRE;
    ctx->base->MECR &= ~MECR_ECRWRDIS;

    /* disable error detection */
    ctx->base->MECR &= ~MECR_NCEFAFRZ;

    /* lock MECR */
    ctx->base->CTRL2 &= ~CTRL2_ECRWRE;
}
#endif

static int nxp_flexcan_soft_reset(struct can *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* soft reset */
    ctx->base->MCR |= MCR_SOFTRST;

    while (deadlock-- != 0)
        if ((ctx->base->MCR & MCR_SOFTRST) == 0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int nxp_flexcan_freeze(struct can *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* set frz & halt */
    ctx->base->MCR |= (MCR_FRZ | MCR_HALT);

    while (deadlock-- != 0)
        if ((ctx->base->MCR & MCR_FRZACK) != 0 &&
            (ctx->base->MCR & MCR_NOTRDY) != 0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int nxp_flexcan_unfreeze(struct can *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* clear frz & halt */
    ctx->base->MCR &= ~(MCR_FRZ | MCR_HALT);

    while (deadlock-- != 0)
        if ((ctx->base->MCR & MCR_FRZACK) == 0 &&
            (ctx->base->MCR & MCR_NOTRDY) == 0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static void nxp_flexcan_clear_ram(/*@partial@*/ struct can *ctx)
{
    size_t n = ctx->mailbox_count;

    while (n-- != 0) {
        ctx->base->MB[n].DATAL = 0;
        ctx->base->MB[n].DATAH = 0;
        ctx->base->RXIMR[n] = (uint32_t)-1;
    }
}

/* Function: can_nxp_flexcan_init
 * Init a FlexCAN interface
 *
 * Parameters:
 *  ctx - The FlexCAN interface to init
 *  base - The FlexCAN interface base address
 *  clkid - The FlexCAN interface clock id
 *  mailbox_count - The number of MBs associated with this FlexCAN interface (see doc)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int can_nxp_flexcan_init(struct can *ctx, int base, clock_id_t clkid,
                         size_t mailbox_count)
{
    picoRTOS_assert(mailbox_count > 0, return -EINVAL);
    picoRTOS_assert(mailbox_count <= (size_t)CAN_NXP_FLEXCAN_MB_COUNT, return -EINVAL);

    int res;

    ctx->base = (struct CAN_NXP_FLEXCAN*)base;
    ctx->clkid = clkid;
    ctx->mailbox_count = mailbox_count;
    /* mb allocation */
    ctx->rx_mailbox_index = 0;

    /* enable interface (will freeze) */
    ctx->base->MCR &= ~MCR_MDIS;
    nxp_flexcan_clear_ram(ctx);

    /* soft reset */
    if ((res = nxp_flexcan_soft_reset(ctx)) < 0)
        return res;

#ifdef CAN_NXP_FLEXCAN_DISABLE_MEM_ERR_DETECTION
    nxp_flexcan_disable_mem_err_detection(ctx);
#endif

    /* set maxmb */
    ctx->base->MCR &= ~MCR_MAXMB(MCR_MAXMB_M);
    ctx->base->MCR |= MCR_MAXMB(ctx->mailbox_count - 1);
    /* disable self-reception */
    ctx->base->MCR |= MCR_SRXDIS;
    /* abort enable */
    ctx->base->MCR |= MCR_AEN;
    /* no supervisor */
    ctx->base->MCR &= ~MCR_SUPV;

    /* default to oscillator clock */
    ctx->base->CTRL1 &= ~CTRL1_CLKSRC;

    /* store remote requests */
    ctx->base->CTRL2 |= CTRL2_RRS;

    /* rx global mask */
    ctx->base->RXMGMASK = (uint32_t)-1;

    /* disable all interrupts & clear flags */
    ctx->base->IMASK1 = 0;
    ctx->base->IFLAG1 = (uint32_t)-1;

    if (ctx->mailbox_count > (size_t)32) {
        ctx->base->IMASK2 = 0;
        ctx->base->IFLAG2 = (uint32_t)-1;
    }

    if (ctx->mailbox_count > (size_t)64) {
        ctx->base->IMASK3 = 0;
        ctx->base->IFLAG3 = (uint32_t)-1;
    }

    /* clear all error flags */
    ctx->base->ESR1 = (uint32_t)-1;

    /* default to 4 tx mailboxes */
    if ((res = nxp_flexcan_set_tx_mailboxes(ctx, (size_t)4)) < 0)
        return res;

    return nxp_flexcan_unfreeze(ctx);
}

/* Function: can_nxp_flexcan_setup
 * Configures a FlexCAN interface
 *
 * Parameters:
 *  ctx - The FlexCAN interface to init
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int can_nxp_flexcan_setup(struct can *ctx, const struct can_nxp_flexcan_settings *settings)
{
    int res;

    if ((res = nxp_flexcan_freeze(ctx)) < 0)
        return res;

    if (settings->clksrc_is_per_clock) ctx->base->CTRL1 |= CTRL1_CLKSRC;
    else ctx->base->CTRL1 &= ~CTRL1_CLKSRC;

    return nxp_flexcan_unfreeze(ctx);
}

int can_setup(struct can *ctx, const struct can_settings *settings)
{
    int res;

    if ((res = nxp_flexcan_freeze(ctx)) < 0)
        return res;

    if ((res = nxp_flexcan_set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if ((res = nxp_flexcan_set_tx_mailboxes(ctx, settings->tx_mailbox_count)) < 0)
        return res;

    if (settings->loopback == CAN_LOOPBACK_ON) {
        ctx->base->CTRL1 |= CTRL1_LPB;
        ctx->base->MCR &= ~MCR_SRXDIS;
    }else{
        ctx->base->CTRL1 &= ~CTRL1_LPB;
        ctx->base->MCR |= MCR_SRXDIS;
    }

    return nxp_flexcan_unfreeze(ctx);
}

int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask)
{
    picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT, return -EINVAL);
    picoRTOS_assert(accept_mask < (can_id_t)CAN_EXTID_COUNT, return -EINVAL);
    picoRTOS_assert(ctx->rx_mailbox_index < ctx->rx_mailbox_count, return -ENOMEM);

    struct CAN_NXP_FLEXCAN_MB *mb = &ctx->base->MB[ctx->rx_mailbox_index];

    /* write the ID */
    if (id < (can_id_t)CAN_ID_COUNT) {
        mb->ID = (uint32_t)ID_ID(id);
        mb->CS = (uint32_t)CS_CODE(CS_CODE_RX_EMPTY);

        /* acceptance mask */
        if (accept_mask != 0)
            ctx->base->RXIMR[ctx->rx_mailbox_index] = (uint32_t)ID_ID(accept_mask);

    }else{
        mb->ID = (uint32_t)ID_IDEXT(id);
        mb->CS = (uint32_t)(CS_CODE(CS_CODE_RX_EMPTY) | CS_IDE);

        /* acceptance mask */
        if (accept_mask != 0) {
            ctx->base->RXIMR[ctx->rx_mailbox_index] = (uint32_t)ID_IDEXT(accept_mask);
            ctx->base->RXIMR[ctx->rx_mailbox_index] |= RXMGMASK_IDE;
        }
    }

    ctx->rx_mailbox_index++;
    return 0;
}

static int nxp_flexcan_prepare_tx(const struct can *ctx,
                                  struct CAN_NXP_FLEXCAN_MB *mb,
                                  can_id_t id, size_t n, uint32_t *CS)
{
    picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    uint32_t CS_flags = 0;

    if ((CS_CODE_t)CS_CODE_GET(mb->CS) == CS_CODE_TX_DATA) {
        /* abort if required */
        if (ctx->tx_auto_abort)
            mb->CS = (uint32_t)CS_CODE(CS_CODE_TX_ABORT);

        return -EAGAIN;
    }

    /* write ID */
    if (id >= (can_id_t)CAN_ID_COUNT) {
        mb->ID = (uint32_t)ID_IDEXT(id);
        CS_flags = (uint32_t)(CS_SRR | CS_IDE);
    }else
        mb->ID = (uint32_t)ID_ID(id);

    *CS = (uint32_t)(CS_DLC(n) | CS_flags);
    return 0;
}

static int mailbox_write(struct CAN_NXP_FLEXCAN_MB *mb, const void *buf, size_t n)
{
    const uint8_t *buf8 = (const uint8_t*)buf;

    /* reset buffer */
    mb->DATAL = 0;
    mb->DATAH = 0;

    switch (n) {
    case 8: mb->DATAH |= (uint32_t)buf8[7] << 0;        /*@fallthrough@*/
    case 7: mb->DATAH |= (uint32_t)buf8[6] << 8;        /*@fallthrough@*/
    case 6: mb->DATAH |= (uint32_t)buf8[5] << 16;       /*@fallthrough@*/
    case 5: mb->DATAH |= (uint32_t)buf8[4] << 24;       /*@fallthrough@*/
    case 4: mb->DATAL |= (uint32_t)buf8[3] << 0;        /*@fallthrough@*/
    case 3: mb->DATAL |= (uint32_t)buf8[2] << 8;        /*@fallthrough@*/
    case 2: mb->DATAL |= (uint32_t)buf8[1] << 16;       /*@fallthrough@*/
    case 1: mb->DATAL |= (uint32_t)buf8[0] << 24; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return (int)n;
}

static int nxp_flexcan_write_mailbox(struct can *ctx, size_t index,
                                     can_id_t id, const void *buf, size_t n)
{
    picoRTOS_assert(index < ctx->mailbox_count, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    int res;
    uint32_t CS = 0;
    struct CAN_NXP_FLEXCAN_MB *mb = &ctx->base->MB[index];

    /* clear any flag */
    (void)nxp_flexcan_clear_mb_flag(ctx, index);

    /* prepare mailbox for tx */
    if ((res = (int)nxp_flexcan_prepare_tx(ctx, mb, id, n, &CS)) < 0)
        return res;

    /* write data */
    (void)mailbox_write(mb, buf, n);

    /* write CS to start xfer */
    mb->CS = (uint32_t)CS_CODE(CS_CODE_TX_DATA) | CS;
    return (int)n;
}

int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    /* get next index */
    size_t index = ctx->rx_mailbox_count + ctx->tx_mailbox_index;

    /* rotate tx index */
    ctx->tx_mailbox_index = (ctx->tx_mailbox_index + 1) % ctx->tx_mailbox_count;
    return nxp_flexcan_write_mailbox(ctx, index, id, buf, n);
}

static int mailbox_read(const struct CAN_NXP_FLEXCAN_MB *mb,
                        void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    uint8_t *buf8 = (uint8_t*)buf;

    switch (n) {
    case 8: buf8[7] = (uint8_t)(mb->DATAH >> 0);        /*@fallthrough@*/
    case 7: buf8[6] = (uint8_t)(mb->DATAH >> 8);        /*@fallthrough@*/
    case 6: buf8[5] = (uint8_t)(mb->DATAH >> 16);       /*@fallthrough@*/
    case 5: buf8[4] = (uint8_t)(mb->DATAH >> 24);       /*@fallthrough@*/
    case 4: buf8[3] = (uint8_t)(mb->DATAL >> 0);        /*@fallthrough@*/
    case 3: buf8[2] = (uint8_t)(mb->DATAL >> 8);        /*@fallthrough@*/
    case 2: buf8[1] = (uint8_t)(mb->DATAL >> 16);       /*@fallthrough@*/
    case 1: buf8[0] = (uint8_t)(mb->DATAL >> 24); break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return (int)n;
}

static can_id_t mailbox_read_id(const struct CAN_NXP_FLEXCAN_MB *mb,
                                uint32_t CS)
{
    if ((CS & CS_IDE) != 0)
        return (can_id_t)(mb->ID & ID_IDEXT_M);

    return (can_id_t)((mb->ID >> 18) & ID_ID_M);
}

static int nxp_flexcan_read_mailbox(struct can *ctx, size_t index,
                                    can_id_t *id, void *buf, size_t n)
{
    picoRTOS_assert(index < ctx->rx_mailbox_count, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    const struct CAN_NXP_FLEXCAN_MB *mb = &ctx->base->MB[index];

    /* check mb flag */
    if (!nxp_flexcan_read_mb_flag(ctx, index))
        return -EAGAIN;

    /* CS read & check */
    uint32_t CS = mb->CS;

    if ((CS_CODE_GET(CS) & CS_CODE_RX_BUSY) != 0)
        return -EAGAIN;

    /* ack IFLAG register */
    (void)nxp_flexcan_clear_mb_flag(ctx, index);

    if ((CS_CODE_t)CS_CODE_GET(CS) == CS_CODE_RX_EMPTY)
        /* undocumented error */
        return -EAGAIN;

    if ((CS_CODE_t)CS_CODE_GET(CS) == CS_CODE_RX_FULL ||
        (CS_CODE_t)CS_CODE_GET(CS) == CS_CODE_RX_OVERRUN) {
        *id = mailbox_read_id(mb, CS);
        return mailbox_read(mb, buf, (size_t)CS_DLC_GET(CS));
    }

    /* unhandled cases */
    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n)
{
    /* classic can only */
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    size_t index = ctx->rx_mailbox_index;

    /* FIXME: sub-optimal */
    while (index-- != 0) {
        int res;
        if ((res = nxp_flexcan_read_mailbox(ctx, index, id, buf, n)) != -EAGAIN) {
            /* force mailboxes unlock */
            ctx->timestamp = ctx->base->TIMER;
            return res;
        }
    }

    return -EAGAIN;
}

int can_request_frame(/*@unused@*/ struct can *ctx __attribute__((unused)),
                      /*@unused@*/ can_id_t id __attribute__((unused)))
{
    return -ENOSYS;
}

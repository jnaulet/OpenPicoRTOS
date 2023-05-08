#include "can-pic32mx.h"
#include "picoRTOS.h"

#include <stdint.h>
#include "misc-pic32mx.h"

#define CAN_PIC32MX_RXM_COUNT 4
#define CAN_PIC32MX_FLTCON_COUNT 8

#define OPMOD_NORMAL      0x0
#define OPMOD_DISABLE     0x1
#define OPMOD_LOOPBACK    0x2
#define OPMOD_LISTEN_ONLY 0x3
#define OPMOD_CONFIG      0x4
#define OPMOD_LISTEN_ALL  0x7

struct CAN_PIC32MX {
    struct PIC32MX_CLR_SET_INV CiCON;
    struct PIC32MX_CLR_SET_INV CiCFG;
    struct PIC32MX_CLR_SET_INV CiINT;
    struct PIC32MX_CLR_SET_INV CiVEC;
    struct PIC32MX_CLR_SET_INV CiTREC;
    struct PIC32MX_CLR_SET_INV CiFSTAT;
    struct PIC32MX_CLR_SET_INV CiRXOVF;
    struct PIC32MX_CLR_SET_INV CiTMR;
    struct PIC32MX_CLR_SET_INV CiRXMn[CAN_PIC32MX_RXM_COUNT];
    struct PIC32MX_CLR_SET_INV CiFLTCONn[CAN_PIC32MX_FLTCON_COUNT];
    struct PIC32MX_CLR_SET_INV CiRXFn[CAN_PIC32MX_FIFO_COUNT];
    struct PIC32MX_CLR_SET_INV CiFIFOBA;
    struct {
        struct PIC32MX_CLR_SET_INV CON;
        struct PIC32MX_CLR_SET_INV INT;
        struct PIC32MX_CLR_SET_INV UA;
        struct PIC32MX_CLR_SET_INV CI;
    } CiFIFO[CAN_PIC32MX_FIFO_COUNT];
};

#define CiCON_ABAT     (1 << 27)
#define CiCON_REQOP_M  0x7u
#define CiCON_REQOP(x) (((x) & CiCON_REQOP_M) << 24)
#define CiCON_OPMOD_M  0x7u
#define CiCON_OPMOD(x) (((x) & CiCON_OPMOD_M) << 21)
#define CiCON_CANCAP   (1 << 20)
#define CiCON_ON       (1 << 15)
#define CiCON_SIDLE    (1 << 13)
#define CiCON_CANBUSY  (1 << 11)
#define CiCON_DNCNT_M  0xfu
#define CiCON_DNCNT(x) ((x) & CiCON_DNCNT_M)

#define CiCFG_WAKFIL    (1 << 22)
#define CiCFG_SEG2PH_M  0x7u
#define CiCFG_SEG2PH(x) (((x) & CiCFG_SEG2PH_M) << 16)
#define CiCFG_SEG2PHTS  (1 << 15)
#define CiCFG_SAM       (1 << 14)
#define CiCFG_SEG1PH_M  0x7u
#define CiCFG_SEG1PH(x) (((x) & CiCFG_SEG1PH_M) << 11)
#define CiCFG_PRSEG_M   0x7u
#define CiCFG_PRSEG(x)  (((x) & CiCFG_PRSEG_M) << 8)
#define CiCFG_SJW_M     0x3u
#define CiCFG_SJW(x)    (((x) & CiCFG_SJW_M) << 6)
#define CiCFG_BRP_M     0x3fu
#define CiCFG_BRP(x)    ((x) & CiCFG_BRP_M)

#define CiINT_IVRIE  (1 << 31)
#define CiINT_WAKIE  (1 << 30)
#define CiINT_CERRIE (1 << 29)
#define CiINT_SERRIE (1 << 28)
#define CiINT_RBOVIE (1 << 27)
#define CiINT_MODIE  (1 << 19)
#define CiINT_CTMRIE (1 << 18)
#define CiINT_RBIE   (1 << 17)
#define CiINT_TBIE   (1 << 16)
#define CiINT_IVRIF  (1 << 15)
#define CiINT_WAKIF  (1 << 14)
#define CiINT_CERRIF (1 << 13)
#define CiINT_SERRIF (1 << 12)
#define CiINT_RBOVIF (1 << 11)
#define CiINT_MODIF  (1 << 3)
#define CiINT_CTMRIF (1 << 2)
#define CiINT_RBIF   (1 << 1)
#define CiINT_TBIF   (1 << 0)

#define CiVEC_FILHIT_M  0x1fu
#define CiVEC_FILHIT(x) (((x) & CiVEC_FILHIT_M) << 8)
#define CiVEC_ICODE_M   0x7fu
#define CiVEC_ICODE(x) ((x) & CiVEC_ICODE_M)

#define CiTREC_TXBO       (1 << 21)
#define CiTREC_TXBP       (1 << 20)
#define CiTREC_RXBP       (1 << 19)
#define CiTREC_TXWARN     (1 << 18)
#define CiTREC_RXWARN     (1 << 17)
#define CiTREC_EWARN      (1 << 16)
#define CiTREC_TERRCNT_M  0xffu
#define CiTREC_TERRCNT(x) (((x) & CiTREC_TERRCNT_M) << 8)
#define CiTREC_RERRCNT_M  0xffu
#define CiTREC_RERRCNT(x) ((x) & CiTREC_RERRCNT_M)

#define CiTMR_CANTS_M     0xffffu
#define CiTMR_CANTS(x)    (((x) & CiTMR_CANTS_M) << 16)
#define CiTMR_CANTSPRE_M  0xffffu
#define CiTMR_CANTSPRE(x) ((x) & CiTMR_CANTSPRE_M)

#define CiRXMn_SID_M  0x7ffu
#define CiRXMn_SID(x) (((x) & CiRXMn_SID_M) << 21)
#define CiRXMn_MIDE   (1 << 19)
#define CiRXMn_EID_M  0x3fffful
#define CiRXMn_EID(x) ((x) & CiRXMn_EID_M)

#define CiFLTCONn_FLTEN0   (1 << 7)
#define CiFLTCONn_MSEL0_M  0x3u
#define CiFLTCONn_MSEL0(x) (((x) & CiFLTCONn_MSEL0_M) << 5)
#define CiFLTCONn_FSEL0_M  0x1fu
#define CiFLTCONn_FSEL0(x) ((x) & CiFLTCONn_FSEL0_M)

#define CiRXFn_SID_M  0x7ffu
#define CiRXFn_SID(x) (((x) & CiRXMn_SID_M) << 21)
#define CiRXFn_EXID   (1 << 19)
#define CiRXFn_EID_M  0x3fffful
#define CiRXFn_EID(x) ((x) & CiRXMn_EID_M)

#define CiFIFOCONn_FSIZE_M  0x1fu
#define CiFIFOCONn_FSIZE(x) (((x) & CiFIFOCONn_FSIZE_M) << 16)
#define CiFIFOCONn_FRESET   (1 << 14)
#define CiFIFOCONn_UINC     (1 << 13)
#define CiFIFOCONn_DONLY    (1 << 12)
#define CiFIFOCONn_TXEN     (1 << 7)
#define CiFIFOCONn_TXABAT   (1 << 6)
#define CiFIFOCONn_TXLARB   (1 << 5)
#define CiFIFOCONn_TXERR    (1 << 4)
#define CiFIFOCONn_TXREQ    (1 << 3)
#define CiFIFOCONn_RTREN    (1 << 2)
#define CiFIFOCONn_TXPRI_M  0x3u
#define CiFIFOCONn_TXPRI(x) ((x) & CiFIFOCONn_TXPRI_M)

#define CiFIFOINTn_TXNFULLIE  (1 << 26)
#define CiFIFOINTn_TXHALFIE   (1 << 25)
#define CiFIFOINTn_TXEMPTYIE  (1 << 24)
#define CiFIFOINTn_RXOVFLIE   (1 << 19)
#define CiFIFOINTn_RXFULLIE   (1 << 18)
#define CiFIFOINTn_RXHALFIE   (1 << 17)
#define CiFIFOINTn_RXNEMPTYIE (1 << 16)
#define CiFIFOINTn_TXNFULLIF  (1 << 10)
#define CiFIFOINTn_TXHALFIF   (1 << 9)
#define CiFIFOINTn_TXEMPTYIF  (1 << 8)
#define CiFIFOINTn_RXOVFLIF   (1 << 3)
#define CiFIFOINTn_RXFULLIF   (1 << 2)
#define CiFIFOINTn_RXHALFIF   (1 << 1)
#define CiFIFOINTn_RXNEMPTYIF (1 << 0)

#define CiFIFOCIn_CiFIFOCI_M  0x1fu
#define CiFIFOCIn_CiFIFOCI(x) ((x) & CiFIFOCIn_CiFIFOCI_M)

struct CAN_PIC32MX_MB {
    volatile uint32_t CMSGID;
    volatile uint32_t CMSGEID;
    volatile uint32_t CMSGDATA0;
    volatile uint32_t CMSGDATA1;
};

#define CMSGID_SID_M  0x7ffu
#define CMSGID_SID(x) ((x) & CMSGID_SID_M)

#define CMSGEID_SRR    (1 << 29)
#define CMSGEID_IDE    (1 << 28)
#define CMSGEID_EID_M  0x3ffff
#define CMSGEID_EID(x) (((x) & CMSGEID_EID_M) << 10)
#define CMSGEID_RTR    (1 << 9)
#define CMSGEID_RB1    (1 << 8)
#define CMSGEID_RB0    (1 << 4)
#define CMSGEID_DLC_M  0xf
#define CMSGEID_DLC(x) ((x) & CMSGEID_DLC_M)

#define CAN_PIC32MX_MB_BY_INDEX(ctx, index)                             \
    (struct CAN_PIC32MX_MB*)                                              \
    PA_TO_KVA((ctx)->base->CiFIFO[(index)].UA.REG,                        \
              (ctx)->kva_base)

static int set_tx_mailboxes(/*@partial@*/ struct can *ctx, size_t n)
{
    if (!picoRTOS_assert(n < (size_t)CAN_PIC32MX_FIFO_COUNT)) return -EINVAL;

    ctx->tx_mailbox_count = n;
    ctx->rx_mailbox_count = (size_t)(CAN_PIC32MX_FIFO_COUNT - n);

    while (n-- != 0)
        ctx->base->CiFIFO[ctx->rx_mailbox_count + n].CON.SET =
            (uint32_t)CiFIFOCONn_TXEN;

    return 0;
}

static int init_fifos(/*@partial@*/ struct can *ctx)
{
    /* set fifos */
    size_t n = (size_t)CAN_PIC32MX_FIFO_COUNT;

    ctx->kva_base = (uint32_t)KVA_BASE(ctx->FIFO);
    ctx->base->CiFIFOBA.REG = (uint32_t)KVA_TO_PA(ctx->FIFO);

    /* TODO: make this configurable */
    while (n-- != 0) {
        /* set max FIFO size */
        ctx->base->CiFIFO[n].CON.REG =
            (uint32_t)CiFIFOCONn_FSIZE(CAN_PIC32MX_FIFO_SIZE_MAX - 1);
        /* put CiRXFn to zero */
        ctx->base->CiRXFn[n].REG = 0;
    }

    return set_tx_mailboxes(ctx, (size_t)1);
}

/* Function: can_pic32mx_init
 * Inits a CAN interface
 *
 * Parameters:
 *  ctx - The CAN interface to init
 *  base - The CAN interface base address
 *  clkid - The CAN interface input clock id
 *
 * Returns:
 * Always 0
 */
int can_pic32mx_init(struct can *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct CAN_PIC32MX*)base;
    ctx->clkid = clkid;
    /* mb allocation */
    ctx->tx_mailbox_index = 0;
    /* filters */
    ctx->rxm_count = 0;
    ctx->rxf_count = 0;
    /* default settings */
    ctx->tx_auto_abort = true;
    ctx->rx_overwrite = true;
    /* state machine */
    ctx->opmod = (uint32_t)OPMOD_NORMAL;
    ctx->kva_base = 0;

    /* turn on */
    ctx->base->CiCON.SET = (uint32_t)CiCON_ON;
    return init_fifos(ctx);
}

/* hooks */

#define TQ_MIN 8
#define TQ_MAX 25

#define PRSEG_MIN 1
#define PRSEG_MAX 8
#define SEG1PH_MIN 1
#define SEG1PH_MAX 8
#define SEG2PH_MIN 1
#define SEG2PH_MAX 8

static int set_brp_and_segs(struct can *ctx, size_t brp, size_t tq)
{
#define div_round_closest(a, b) (((a) + ((b) >> 1)) / (b))

    if (!picoRTOS_assert(tq >= (size_t)TQ_MIN)) return -EINVAL;
    if (!picoRTOS_assert(tq <= (size_t)TQ_MAX)) return -EINVAL;

    size_t seg1ph;
    size_t seg2ph;
    size_t prseg = 0;

    /* -1 (account for SYNC_SEG) */
    tq -= 1;

    /* default sample point is at 87.5% */
    seg1ph = div_round_closest(tq * 64, 73);

    /* compute prseg */
    if (seg1ph > (size_t)SEG1PH_MAX) {
        prseg = seg1ph - (size_t)SEG1PH_MAX;
        seg1ph -= prseg;
    }

    /* check max prseg */
    if (prseg > (size_t)PRSEG_MAX)
        return -EINVAL;

    /* compute seg2ph */
    seg2ph = tq - (prseg + seg1ph);

    /* ignore out of bounds values */
    if (prseg < (size_t)PRSEG_MIN || prseg > (size_t)PRSEG_MAX ||
        seg1ph < (size_t)SEG1PH_MIN || seg1ph > (size_t)SEG1PH_MAX ||
        seg2ph < (size_t)SEG2PH_MIN || seg2ph > (size_t)SEG2PH_MAX)
        return -EINVAL;

    ctx->base->CiCFG.REG = (uint32_t)(CiCFG_BRP(brp - 1) |
                                      CiCFG_SJW(0) |
                                      CiCFG_PRSEG(prseg - 1) |
                                      CiCFG_SEG1PH(seg1ph - 1) |
                                      CiCFG_SEG2PHTS |
                                      CiCFG_SEG2PH(seg2ph - 1));

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
    for (brp = (size_t)1; brp <= (size_t)(CiCFG_BRP_M + 1u); brp++) {
        size_t tq_div = (brp * (size_t)bitrate) * (size_t)2;
        size_t tq = (size_t)freq / tq_div;

        /* check for accuracy */
        if (((size_t)freq % tq_div) != 0)
            continue;

        /* ignore out of bounds values */
        if (tq < (size_t)TQ_MIN || tq > (size_t)TQ_MAX)
            continue;

        /* set values */
        if (set_brp_and_segs(ctx, brp, tq) == 0)
            return 0;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EINVAL;
}

static int set_opmod_busywait(struct can *ctx, uint32_t op)
{
    if (!picoRTOS_assert(op <= (uint32_t)CiCON_REQOP_M)) return -EINVAL;

    int loop = CONFIG_DEADLOCK_COUNT;
    uint32_t cicon = ctx->base->CiCON.REG;

    cicon &= ~CiCON_REQOP(CiCON_REQOP_M);
    ctx->base->CiCON.REG = cicon | (uint32_t)CiCON_REQOP(op);

    while (loop-- != 0) {
        uint32_t opmod = (uint32_t)((ctx->base->CiCON.REG >> 21) & CiCON_REQOP_M);
        if (opmod == op)
            break;
    }

    if (!picoRTOS_assert(loop != -1))
        return -EBUSY;

    ctx->base->CiINT.CLR = (uint32_t)CiINT_MODIF;
    return 0;
}

int can_setup(struct can *ctx, struct can_settings *settings)
{
    if (!picoRTOS_assert(settings->tx_mailbox_count <
                         (size_t)CAN_PIC32MX_FIFO_COUNT)) return -EINVAL;
    int res;

    /* set config mode */
    if ((res = set_opmod_busywait(ctx, (uint32_t)OPMOD_CONFIG)) < 0)
        return res;

    if ((res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if ((res = set_tx_mailboxes(ctx, settings->tx_mailbox_count)) < 0)
        return res;

    ctx->tx_auto_abort = settings->tx_auto_abort;
    ctx->rx_overwrite = settings->rx_overwrite;

    /* back to normal (or loopback) */
    if (settings->loopback) {
        ctx->opmod = (uint32_t)OPMOD_LOOPBACK;
        return set_opmod_busywait(ctx, (uint32_t)OPMOD_LOOPBACK);
    }

    ctx->opmod = (uint32_t)OPMOD_NORMAL;
    return set_opmod_busywait(ctx, (uint32_t)OPMOD_NORMAL);
}

static int find_acceptance_mask(struct can *ctx, can_id_t mask)
{
    if (!picoRTOS_assert(mask < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;

    /* look for an existing mask */
    size_t n = ctx->rxm_count;

    while (n-- != 0) {
        /* only 11bit for now */
        uint32_t rxm = (uint32_t)((ctx->base->CiRXMn[n].REG >> 21) & CiRXMn_SID_M);
        if (rxm == (uint32_t)mask)
            return (int)n;
    }

    return -1;
}

static int allocate_acceptance_mask(struct can *ctx, can_id_t mask)
{
    if (!picoRTOS_assert(mask < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(ctx->rxm_count < (size_t)CAN_PIC32MX_RXM_COUNT)) return -ENOMEM;

    int res;

    if ((res = set_opmod_busywait(ctx, (uint32_t)OPMOD_CONFIG)) < 0)
        return res;

    /* can only be accessed in config mode */
    ctx->base->CiRXMn[ctx->rxm_count].REG = (uint32_t)CiRXMn_SID(mask);
    ctx->base->CiRXMn[ctx->rxm_count].SET = (uint32_t)CiRXMn_MIDE;

    if ((res = set_opmod_busywait(ctx, ctx->opmod)) < 0)
        return res;

    return (int)ctx->rxm_count++;
}

static int setup_filter(struct can *ctx, size_t msel, size_t fsel)
{
    if (!picoRTOS_assert(msel < (size_t)CAN_PIC32MX_RXM_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(fsel < (size_t)CAN_PIC32MX_FIFO_COUNT)) return -EINVAL;

    size_t index = fsel / (size_t)4;
    size_t shift = (size_t)(fsel & 0x3) * 8;

    /* configure filter */
    ctx->base->CiFLTCONn[index].REG = (uint32_t)(CiFLTCONn_MSEL0(msel) |
                                                 CiFLTCONn_FSEL0(fsel) << shift);
    /* turn on filter */
    ctx->base->CiFLTCONn[index].SET = (uint32_t)CiFLTCONn_FLTEN0 << shift;

    return 0;
}

int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask)
{
    if (!picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(accept_mask < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(ctx->rxf_count < (size_t)CAN_PIC32MX_FIFO_COUNT)) return -ENOMEM;

    int res;

    /* look for mask */
    if ((res = find_acceptance_mask(ctx, accept_mask)) < 0) {
        /* not found, try to allocate it */
        if ((res = allocate_acceptance_mask(ctx, accept_mask)) < 0)
            return res;
    }

    /* set fifo id while FLTENn = 0 */
    ctx->base->CiRXFn[ctx->rxf_count].REG = (uint32_t)CiRXFn_SID(id);
    if ((res = setup_filter(ctx, (size_t)res, ctx->rxf_count)) < 0)
        return res;

    ctx->rxf_count++;
    return 0;
}

static int transfer_tx_mailbox(struct can *ctx, size_t index,
                               can_id_t id, const void *buf, size_t n)
{
    if (!picoRTOS_assert(index < (size_t)CAN_PIC32MX_FIFO_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT)) return -EINVAL;

    const uint8_t *buf8 = (const uint8_t*)buf;
    struct CAN_PIC32MX_MB *mb = CAN_PIC32MX_MB_BY_INDEX(ctx, index);

    /* FIFO is full */
    if ((ctx->base->CiFIFO[index].INT.REG & CiFIFOINTn_TXNFULLIF) == 0) {
        ctx->base->CiFIFO[index].CON.SET = (uint32_t)CiFIFOCONn_TXREQ;
        /* TODO: auto abort ? Problem: you can only abort all pending tx messages */
        return -EAGAIN;
    }

    /* reset buffer */
    mb->CMSGDATA0 = 0;
    mb->CMSGDATA1 = 0;

    switch (n) {
    case 8: mb->CMSGDATA1 |= (uint32_t)buf8[7] << 0;    /*@fallthrough@*/
    case 7: mb->CMSGDATA1 |= (uint32_t)buf8[6] << 8;    /*@fallthrough@*/
    case 6: mb->CMSGDATA1 |= (uint32_t)buf8[5] << 16;   /*@fallthrough@*/
    case 5: mb->CMSGDATA1 |= (uint32_t)buf8[4] << 24;   /*@fallthrough@*/
    case 4: mb->CMSGDATA0 |= (uint32_t)buf8[3] << 0;    /*@fallthrough@*/
    case 3: mb->CMSGDATA0 |= (uint32_t)buf8[2] << 8;    /*@fallthrough@*/
    case 2: mb->CMSGDATA0 |= (uint32_t)buf8[1] << 16;   /*@fallthrough@*/
    case 1: mb->CMSGDATA0 |= (uint32_t)buf8[0] << 24; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    /* only 11bit can for now */
    mb->CMSGID = (uint32_t)CMSGID_SID(id);
    mb->CMSGEID = (uint32_t)CMSGEID_DLC(n);

    /* start xfer */
    ctx->base->CiFIFO[index].CON.SET = (uint32_t)CiFIFOCONn_UINC;
    ctx->base->CiFIFO[index].CON.SET = (uint32_t)CiFIFOCONn_TXREQ;

    return (int)n;
}

int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n)
{
    if (!picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT)) return -EINVAL;

    /* get next index */
    size_t index = ctx->rx_mailbox_count + ctx->tx_mailbox_index;

    /* compute next index & transfer */
    ctx->tx_mailbox_index = (ctx->tx_mailbox_index + 1) % ctx->tx_mailbox_count;
    return transfer_tx_mailbox(ctx, index, id, buf, n);
}

static int transfer_rx_mailbox(struct can *ctx, size_t index,
                               can_id_t *id, void *buf, size_t n)
{
    if (!picoRTOS_assert(index < (size_t)CAN_PIC32MX_FIFO_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT)) return -EINVAL;

    uint8_t *buf8 = (uint8_t*)buf;
    struct CAN_PIC32MX_MB *mb = CAN_PIC32MX_MB_BY_INDEX(ctx, index);

    /* check if data is available */
    if ((ctx->base->CiFIFO[index].INT.REG & CiFIFOINTn_RXNEMPTYIF) == 0)
        return -EAGAIN;

    switch (n) {
    case 8: buf8[7] = (uint8_t)(mb->CMSGDATA1 >> 0);    /*@fallthrough@*/
    case 7: buf8[6] = (uint8_t)(mb->CMSGDATA1 >> 8);    /*@fallthrough@*/
    case 6: buf8[5] = (uint8_t)(mb->CMSGDATA1 >> 16);   /*@fallthrough@*/
    case 5: buf8[4] = (uint8_t)(mb->CMSGDATA1 >> 24);   /*@fallthrough@*/
    case 4: buf8[3] = (uint8_t)(mb->CMSGDATA0 >> 0);    /*@fallthrough@*/
    case 3: buf8[2] = (uint8_t)(mb->CMSGDATA0 >> 8);    /*@fallthrough@*/
    case 2: buf8[1] = (uint8_t)(mb->CMSGDATA0 >> 16);   /*@fallthrough@*/
    case 1: buf8[0] = (uint8_t)(mb->CMSGDATA0 >> 24); break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    /* get id & dlc */
    *id = (can_id_t)(mb->CMSGID & CMSGID_SID_M);
    n = (size_t)(mb->CMSGEID & CMSGEID_DLC_M);

    /* mark as processed */
    ctx->base->CiFIFO[index].CON.SET = (uint32_t)CiFIFOCONn_UINC;
    return (int)n;
}

int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT)) return -EINVAL;

    size_t i;

    for (i = 0; i < ctx->rxf_count; i++) {
        int res;
        if ((res = transfer_rx_mailbox(ctx, i, id, buf, n)) != -EAGAIN)
            return res;
    }

    return -EAGAIN;
}

int can_request_frame(/*@unused@*/ struct can *ctx __attribute__((unused)),
                      /*@unused@*/ can_id_t id __attribute__((unused)))
{
    return -ENOSYS;
}

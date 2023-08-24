#include "can-ti_dcan.h"
#include "picoRTOS.h"
#include <generated/autoconf.h>

#include <stdint.h>

struct CAN_REGS {
    volatile uint32_t CAN_CTL;
    uint32_t RESERVED0;
    volatile uint32_t CAN_ES;
    uint32_t RESERVED1;
    volatile uint32_t CAN_ERRC;
    uint32_t RESERVED2;
    volatile uint32_t CAN_BTR;
    uint32_t RESERVED3;
    volatile uint32_t CAN_INT;
    uint32_t RESERVED4;
    volatile uint32_t CAN_TEST;
    uint32_t RESERVED5[3];
    volatile uint32_t CAN_PERR;
    uint32_t RESERVED6[17];
    volatile uint32_t CAN_RAM_INIT;
    uint32_t RESERVED7[7];
    volatile uint32_t CAN_GLB_INT_EN;
    uint32_t RESERVED8;
    volatile uint32_t CAN_GLB_INT_FLG;
    uint32_t RESERVED9;
    volatile uint32_t CAN_GLB_INT_CLR;
    uint32_t RESERVED10[19];
    volatile uint32_t CAN_ABOTR;
    uint32_t RESERVED11;
    volatile uint32_t CAN_TXRQ_X;
    uint32_t RESERVED12;
    volatile uint32_t CAN_TXRQ_21;
    uint32_t RESERVED13[7];
    volatile uint32_t CAN_NDAT_X;
    uint32_t RESERVED14;
    volatile uint32_t CAN_NDAT_21;
    uint32_t RESERVED15[7];
    volatile uint32_t CAN_IPEN_X;
    uint32_t RESERVED16;
    volatile uint32_t CAN_IPEN_21;
    uint32_t RESERVED17[7];
    volatile uint32_t CAN_MVAL_X;
    uint32_t RESERVED18;
    volatile uint32_t CAN_MVAL_21;
    uint32_t RESERVED19[9];
    volatile uint32_t CAN_IP_MUX21;
    uint32_t RESERVED20[19];
    volatile uint32_t CAN_IF1CMD;
    uint32_t RESERVED21;
    volatile uint32_t CAN_IF1MSK;
    uint32_t RESERVED22;
    volatile uint32_t CAN_IF1ARB;
    uint32_t RESERVED23;
    volatile uint32_t CAN_IF1MCTL;
    uint32_t RESERVED24;
    volatile uint32_t CAN_IF1DATA;
    uint32_t RESERVED25;
    volatile uint32_t CAN_IF1DATB;
    uint32_t RESERVED26[5];
    volatile uint32_t CAN_IF2CMD;
    uint32_t RESERVED27;
    volatile uint32_t CAN_IF2MSK;
    uint32_t RESERVED28;
    volatile uint32_t CAN_IF2ARB;
    uint32_t RESERVED29;
    volatile uint32_t CAN_IF2MCTL;
    uint32_t RESERVED30;
    volatile uint32_t CAN_IF2DATA;
    uint32_t RESERVED31;
    volatile uint32_t CAN_IF2DATB;
    uint32_t RESERVED32[5];
    volatile uint32_t CAN_IF3OBS;
    uint32_t RESERVED33;
    volatile uint32_t CAN_IF3MSK;
    uint32_t RESERVED34;
    volatile uint32_t CAN_IF3ARB;
    uint32_t RESERVED35;
    volatile uint32_t CAN_IF3MCTL;
    uint32_t RESERVED36;
    volatile uint32_t CAN_IF3DATA;
    uint32_t RESERVED37;
    volatile uint32_t CAN_IF3DATB;
    uint32_t RESERVED38[5];
    volatile uint32_t CAN_IF3UPD;
};

#define CAN_CTL_SWR  (1ul << 15)
#define CAN_CTL_TEST (1ul << 7)
#define CAN_CTL_CCE  (1ul << 6)
#define CAN_CTL_INIT (1ul << 0)

#define CAN_BTR_BRPE_M   0xful
#define CAN_BTR_BRPE(x)  (((x) & CAN_BTR_BRPE_M) << 16)
#define CAN_BTR_TSEG2_M  0x7ul
#define CAN_BTR_TSEG2(x) (((x) & CAN_BTR_TSEG2_M) << 12)
#define CAN_BTR_TSEG1_M  0xful
#define CAN_BTR_TSEG1(x) (((x) & CAN_BTR_TSEG1_M) << 8)
#define CAN_BTR_SJW_M    0x3ul
#define CAN_BTR_SJW(x)   (((x) & CAN_BTR_SJW_M) << 6)
#define CAN_BTR_BRP_M    0x3ful
#define CAN_BTR_BRP(x)   ((x) & CAN_BTR_BRP_M)

#define CAN_TEST_LBACK (1ul << 4)

#define CAN_RAM_INIT_CAN_RAM_INIT  (1ul << 4)
#define CAN_RAM_INIT_RAM_INIT_DONE (1ul << 5)

#define CAN_IF1CMD_DIR     (1ul << 23)
#define CAN_IF1CMD_ARB     (1ul << 21)
#define CAN_IF1CMD_CONTROL (1ul << 20)
#define CAN_IF1CMD_DATA_A  (1ul << 17)
#define CAN_IF1CMD_DATA_B  (1ul << 16)
#define CAN_IF1CMD_BUSY    (1ul << 15)

#define CAN_IF1ARB_MSGVAL (1ul << 31)
#define CAN_IF1ARB_DIR    (1ul << 29)

#define CAN_IF1MCTL_TXRQST (1ul << 8)
#define CAN_IF1MCTL_EOB    (1ul << 7)

#define CAN_IF2CMD_CONTROL (1ul << 20)
#define CAN_IF2CMD_TXRQST  (1ul << 18)
#define CAN_IF2CMD_DATA_A  (1ul << 17)
#define CAN_IF2CMD_DATA_B  (1ul << 16)
#define CAN_IF2CMD_BUSY    (1ul << 15)

#define CAN_IF2ARB_MSGVAL (1ul << 31)
#define CAN_IF2ARB_XTD    (1ul << 30)
#define CAN_IF2ARB_DIR    (1ul << 29)
#define CAN_IF2ARB_ID_M   0x1ffffffful
#define CAN_IF2ARB_ID(x)  ((x) & CAN_IF2ARB_ID_M)

#define CAN_IF2MCTL_NEWDAT (1ul << 15)
#define CAN_IF2MCTL_DLC_M  0xful
#define CAN_IF2MCTL_DLC(x) ((x) & CAN_IF2MCTL_DLC_M)

static int ifcmd_busywait(volatile const uint32_t *ifcmd, uint32_t mask)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((*ifcmd & mask) == 0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int clear_ram_busywait(struct can *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    ctx->base->CAN_RAM_INIT = (uint32_t)CAN_RAM_INIT_CAN_RAM_INIT | 0xa;

    while (deadlock-- != 0)
        if ((ctx->base->CAN_RAM_INIT & CAN_RAM_INIT_RAM_INIT_DONE) != 0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int set_tx_mailboxes(/*@partial@*/ struct can *ctx, size_t n)
{
    picoRTOS_assert(n < (size_t)CAN_TI_DCAN_MB_COUNT, return -EINVAL);

    ctx->tx_mailbox_count = n;
    ctx->rx_mailbox_count = (size_t)(CAN_TI_DCAN_MB_COUNT - n);

    return 0;
}

/* Function: can_ti_dcan_init
 * Init a CAN interface
 *
 * Parameters:
 *  ctx - The CAN interface to init
 *  base - The CAN interface base address
 *  clkid - The CAN interface inptu clock id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int can_ti_dcan_init(struct can * ctx, long base, clock_id_t clkid)
{
    int res;

    ctx->base = (struct CAN_REGS*)base;
    ctx->clkid = clkid;
    /* mb allocation */
    ctx->rx_mailbox_index = 0;
    ctx->tx_mailbox_index = 0;
    ctx->rx_mailbox_count = 0;
    ctx->tx_mailbox_count = 0;
    /* default settings */
    ctx->tx_auto_abort = true;
    ctx->rx_overwrite = true;

    ctx->base->CAN_CTL = (uint32_t)CAN_CTL_INIT;    /* init */
    ctx->base->CAN_CTL = (uint32_t)CAN_CTL_SWR;     /* reset */

    /* wait until non-busy */
    if ((res = ifcmd_busywait(&ctx->base->CAN_IF1CMD, (uint32_t)CAN_IF1CMD_BUSY)) < 0 ||
        (res = ifcmd_busywait(&ctx->base->CAN_IF2CMD, (uint32_t)CAN_IF2CMD_BUSY)) < 0)
        return res;

    /* clear ram */
    if ((res = clear_ram_busywait(ctx)) < 0)
        return res;

    /* enable can */
    ctx->base->CAN_CTL &= ~CAN_CTL_INIT;
    return set_tx_mailboxes(ctx, (size_t)2);
}

#define TQ_MIN 8
#define TQ_MAX 25

#define TSEG1_MIN    2
#define TSEG1_MAX    16
#define TSEG2_MIN    1
#define TSEG2_MAX    8
#define PSEG_DEFAULT 3

static int set_brp_and_tseg(struct can *ctx, size_t brp, size_t tq)
{
#define div_round_closest(a, b) (((a) + ((b) >> 1)) / (b))

    picoRTOS_assert(tq >= (size_t)TQ_MIN, return -EINVAL);
    picoRTOS_assert(tq <= (size_t)TQ_MAX, return -EINVAL);

    size_t tseg1;
    size_t tseg2;

    /* -1 (account for SYNC_SEG) */
    tq -= 1;

    /* sample point is at 87.5% : 64 / 73 = 0.876 */
    tseg1 = div_round_closest(tq * 64, 73) - (size_t)PSEG_DEFAULT;
    tseg2 = tq - tseg1;

    /* ignore out of bounds values */
    if (tseg1 < (size_t)TSEG1_MIN || tseg1 > (size_t)TSEG1_MAX ||
        tseg2 < (size_t)TSEG2_MIN || tseg2 > (size_t)TSEG2_MAX)
        return -EINVAL;

    /* controller must be placed in init mode and config change */
    ctx->base->CAN_CTL |= (CAN_CTL_INIT | CAN_CTL_CCE);

    ctx->base->CAN_BTR = (uint32_t)CAN_BTR_BRPE((brp - 1) >> 5);
    ctx->base->CAN_BTR |= (uint32_t)CAN_BTR_TSEG2(tseg2 - 1);
    ctx->base->CAN_BTR |= (uint32_t)CAN_BTR_TSEG1(tseg1 - 1);
    ctx->base->CAN_BTR |= (uint32_t)CAN_BTR_SJW(0x3);
    ctx->base->CAN_BTR |= (uint32_t)CAN_BTR_BRP(brp - 1);

    /* restore ctl */
    ctx->base->CAN_CTL &= ~(CAN_CTL_INIT | CAN_CTL_CCE);
    return 0;
}

static int set_bitrate(struct can *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

#define BRPE_MAX 1024

    size_t brp;
    clock_freq_t freq;

    if ((freq = clock_get_freq(ctx->clkid)) < 0)
        return (int)freq;

    picoRTOS_assert(((unsigned long)freq % bitrate) == 0,
                    return -EIO; )

    /* parameters computation (iterative) */
    for (brp = (size_t)1; brp <= (size_t)BRPE_MAX; brp++) {
        size_t tq_div = (brp * (size_t)bitrate);
        size_t tq = (size_t)freq / tq_div;

        /* check for accuracy */
        if (((size_t)freq % tq_div) != 0)
            continue;

        /* ignore out of bounds values */
        if (tq < (size_t)TQ_MIN || tq > (size_t)TQ_MAX)
            continue;

        /* set values */
        if (set_brp_and_tseg(ctx, brp, tq) == 0)
            return 0;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int can_setup(struct can *ctx, struct can_settings *settings)
{
    int res;

    /* bitrate */
    if ((res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* loopback */
    if (settings->loopback) {
        ctx->base->CAN_CTL |= CAN_CTL_TEST;
        ctx->base->CAN_TEST = (uint32_t)CAN_TEST_LBACK;
    }

    return set_tx_mailboxes(ctx, settings->tx_mailbox_count);
}

int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask)
{
    picoRTOS_assert(id < (can_id_t)CAN_ID_COUNT, return -EINVAL);
    picoRTOS_assert(accept_mask < (can_id_t)CAN_ID_COUNT, return -EINVAL);
    picoRTOS_assert(ctx->rx_mailbox_index < ctx->rx_mailbox_count, return -ENOMEM);

    uint32_t if2cmd = (uint32_t)(CAN_IF1CMD_DIR | CAN_IF1CMD_ARB | CAN_IF1CMD_CONTROL);

    /* only classic can */
    ctx->base->CAN_IF2ARB = ((uint32_t)id << 18);
    ctx->base->CAN_IF2ARB |= (uint32_t)CAN_IF1ARB_MSGVAL;
    ctx->base->CAN_IF2MSK = ((uint32_t)accept_mask << 18);

    ctx->base->CAN_IF2MCTL = 0;
    ctx->base->CAN_IF2CMD = if2cmd | (++ctx->rx_mailbox_index);

    return ifcmd_busywait(&ctx->base->CAN_IF2CMD, (uint32_t)CAN_IF2CMD_BUSY);
}

static int copy_data_to_mb(struct can *ctx, const uint_least8_t *buf, size_t n, uint32_t *mask)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    /* message packing */
    uint32_t DATA_A = 0;
    uint32_t DATA_B = 0;

    switch (n) {
    case 8: DATA_B = ((uint32_t)buf[7] << 24);      /*@fallthrough@*/
    case 7: DATA_B |= (uint32_t)buf[6] << 16;       /*@fallthrough@*/
    case 6: DATA_B |= (uint32_t)buf[5] << 8;        /*@fallthrough@*/
    case 5:
        DATA_B |= (uint32_t)buf[4];
        *mask |= (uint32_t)CAN_IF1CMD_DATA_B;
        ctx->base->CAN_IF1DATB = DATA_B;            /*@fallthrough@*/

    case 4: DATA_A = ((uint32_t)buf[3] << 24);      /*@fallthrough@*/
    case 3: DATA_A |= (uint32_t)buf[2] << 16;       /*@fallthrough@*/
    case 2: DATA_A |= (uint32_t)buf[1] << 8;        /*@fallthrough@*/
    case 1:
        DATA_A |= (uint32_t)buf[0];
        *mask |= (uint32_t)CAN_IF1CMD_DATA_A;
        ctx->base->CAN_IF1DATA = DATA_A;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return (int)n;
}

static int copy_mb_to_data(struct can *ctx, size_t index, uint_least8_t *buf, size_t n)
{
    picoRTOS_assert(index < ctx->rx_mailbox_count, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    int res;
    uint32_t DATA_A;
    uint32_t DATA_B;
    uint32_t if2cmd = (uint32_t)CAN_IF2CMD_DATA_A;

    if (n > (size_t)3)
        if2cmd |= CAN_IF2CMD_DATA_B;

    /* read mailbox data a/b */
    ctx->base->CAN_IF2CMD = if2cmd | (index + 1);
    if ((res = ifcmd_busywait(&ctx->base->CAN_IF2CMD, (uint32_t)CAN_IF2CMD_BUSY)) < 0)
        return (int)res;

    /* message unpacking */
    DATA_A = ctx->base->CAN_IF2DATA;
    DATA_B = ctx->base->CAN_IF2DATB;

    switch (n) {
    case 8: buf[7] = (uint_least8_t)(DATA_B >> 24);             /*@fallthrough@*/
    case 7: buf[6] = (uint_least8_t)(DATA_B >> 16);             /*@fallthrough@*/
    case 6: buf[5] = (uint_least8_t)(DATA_B >> 8);              /*@fallthrough@*/
    case 5: buf[4] = (uint_least8_t)(DATA_B & 0xff);            /*@fallthrough@*/
    case 4: buf[3] = (uint_least8_t)(DATA_A >> 24);             /*@fallthrough@*/
    case 3: buf[2] = (uint_least8_t)(DATA_A >> 16);             /*@fallthrough@*/
    case 2: buf[1] = (uint_least8_t)(DATA_A >> 8);              /*@fallthrough@*/
    case 1: buf[0] = (uint_least8_t)(DATA_A & 0xff); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return (int)n;
}

static int transfer_tx_mailbox(struct can *ctx, size_t index,
                               can_id_t id, const uint_least8_t *buf, size_t n)
{
    picoRTOS_assert(index < (size_t)CAN_TI_DCAN_MB_COUNT, return -EINVAL);
    picoRTOS_assert(id < (can_id_t)CAN_ID_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    int res;
    uint32_t if1cmd = 0;

    /* read txrqst */
    ctx->base->CAN_IF1CMD = (uint32_t)CAN_IF1CMD_CONTROL | (index + 1);
    if ((res = ifcmd_busywait(&ctx->base->CAN_IF1CMD, (uint32_t)CAN_IF1CMD_BUSY)) < 0)
        return res;

    /* check */
    if ((ctx->base->CAN_IF1MCTL & CAN_IF1MCTL_TXRQST) != 0)
        return -EAGAIN;

    /* copy/pack data to mb */
    (void)copy_data_to_mb(ctx, buf, n, &if1cmd);

    /* TODO: add support for EXTID */
    ctx->base->CAN_IF1ARB = ((uint32_t)id << 18);
    ctx->base->CAN_IF1ARB |= (uint32_t)CAN_IF1ARB_MSGVAL;
    ctx->base->CAN_IF1ARB |= CAN_IF1ARB_DIR; /* tx */

    ctx->base->CAN_IF1MCTL = (uint32_t)n;
    ctx->base->CAN_IF1MCTL |= (CAN_IF1MCTL_EOB | CAN_IF1MCTL_TXRQST);

    if1cmd |= (uint32_t)(CAN_IF1CMD_DIR | CAN_IF1CMD_ARB | CAN_IF1CMD_CONTROL);
    if1cmd |= (uint32_t)(index + 1);

    /* execute tx command */
    ctx->base->CAN_IF1CMD = if1cmd;
    if ((res = ifcmd_busywait(&ctx->base->CAN_IF1CMD, (uint32_t)CAN_IF1CMD_BUSY)) < 0)
        return res;

    return (int)n;
}

static int transfer_rx_mailbox(struct can *ctx, size_t index,
                               can_id_t *id, uint_least8_t *buf, size_t n)
{
    picoRTOS_assert(index < (size_t)CAN_TI_DCAN_MB_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    int ret = -EAGAIN;

    /* run command */
    ctx->base->CAN_IF2CMD = (uint32_t)CAN_IF2CMD_CONTROL | (index + 1);
    if (ifcmd_busywait(&ctx->base->CAN_IF2CMD, (uint32_t)CAN_IF2CMD_BUSY) < 0)
        return -EBUSY;

    /* check if mb data is ready */
    if ((ctx->base->CAN_IF2MCTL & CAN_IF2MCTL_NEWDAT) != 0) {
        size_t dlc = (size_t)(ctx->base->CAN_IF2MCTL & CAN_IF2MCTL_DLC_M);
        (void)copy_mb_to_data(ctx, index, buf, dlc);
        *id = (can_id_t)((ctx->base->CAN_IF2ARB & CAN_IF2ARB_ID_M) >> 18);
        /* clear flag */
        ctx->base->CAN_IF2CMD = (uint32_t)CAN_IF2CMD_TXRQST | (index + 1);
        ret = (int)dlc;
    }

    return ret;
}

int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n)
{
    picoRTOS_assert(id < (can_id_t)CAN_ID_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    /* get next index */
    size_t index = ctx->rx_mailbox_count + ctx->tx_mailbox_index;
    uint32_t mask = (uint32_t)(1 << index);

    /* check if busy */
    if ((ctx->base->CAN_TXRQ_21 & mask) != 0)
        /* TODO: auto abort ? */
        return -EAGAIN;

    /* compute next index & transfer */
    ctx->tx_mailbox_index = (ctx->tx_mailbox_index + 1) % ctx->tx_mailbox_count;
    return transfer_tx_mailbox(ctx, index, id, (const uint_least8_t*)buf, n);
}

int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    size_t index = ctx->rx_mailbox_index;
    uint32_t mask = (uint32_t)((1 << ctx->rx_mailbox_count) - 1);
    uint32_t ndat = ctx->base->CAN_NDAT_21 & mask;

    if (ndat == 0)
        return -EAGAIN;

    while (index-- != 0)
        if ((ndat & (1 << index)) != 0)
            return transfer_rx_mailbox(ctx, index, id, (uint_least8_t*)buf, n);

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int can_request_frame(/*@unused@*/ struct can *ctx __attribute__((unused)),
                      /*@unused@*/ can_id_t id __attribute__((unused)))
{
    return -ENOSYS;
}

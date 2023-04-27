#include "can-same5x.h"
#include "picoRTOS.h"

#include <stdint.h>

struct CAN_SAME5X {
    volatile uint32_t CREL;
    volatile uint32_t ENDN;
    volatile uint32_t MRCFG;
    volatile uint32_t DBTP;
    volatile uint32_t TEST;
    volatile uint32_t RWD;
    volatile uint32_t CCCR;
    volatile uint32_t NBTP;
    volatile uint32_t TSCC;
    volatile uint32_t TSCV;
    volatile uint32_t TOCV;
    uint32_t RESERVED0[4];
    volatile uint32_t ECR;
    volatile uint32_t PSR;
    volatile uint32_t TDCR;
    uint32_t RESERVED1;
    volatile uint32_t IR;
    volatile uint32_t IE;
    volatile uint32_t ILS;
    volatile uint32_t ILE;
    uint32_t RESERVED2[4];
    volatile uint32_t GFC;
    volatile uint32_t SIDFC;
    volatile uint32_t XIDFC;
    uint32_t RESERVED3;
    volatile uint32_t XIDAM;
    volatile uint32_t HPMS;
    volatile uint32_t NDAT1;
    volatile uint32_t NDAT2;
    volatile uint32_t RXF0C;
    volatile uint32_t RXF0S;
    volatile uint32_t RXF0A;
    volatile uint32_t RXBC;
    volatile uint32_t RXF1C;
    volatile uint32_t RXF1S;
    volatile uint32_t RXF1A;
    volatile uint32_t RXESC;
    volatile uint32_t TXBC;
    volatile uint32_t TXFQS;
    volatile uint32_t TXESC;
    volatile uint32_t TXBRP;
    volatile uint32_t TXBAR;
    volatile uint32_t TXBRC;
    volatile uint32_t TXBTO;
    volatile uint32_t TXBCF;
    volatile uint32_t TXBTIE;
    volatile uint32_t TXBCIE;
    uint32_t RESERVED4[2];
    volatile uint32_t TXEFC;
    volatile uint32_t TXEFS;
    volatile uint32_t TXEFA;
};

#define MRCFG_QOS_M  0x3u
#define MRCFG_QOS(x) (((x) & MRCFG_QOS_M)

#define DBTP_TDC       (1 << 23)
#define DBTP_DBRP_M    0x1fu
#define DBTP_DBRP(x)   (((x) & DBTP_DBRP_M) << 16)
#define DBTP_DTSEG1_M  0x1fu
#define DBTP_DTSEG1(x) (((x) & DBTP_DTSEG1_M) << 8)
#define DBTP_DTSEG2_M  0xfu
#define DBTP_DTSEG2(x) (((x) & DBTP_DTSEG2_M) << 4)
#define DBTP_DSJW_M    0xfu
#define DBTP_DSJW(x)   ((x) & DBTP_DSJW_M)

#define TEST_RX    (1 << 7)
#define TEST_TX_M  0x3u
#define TEST_TX(x) (((x) & TEST_TX_M) << 5)
#define TEST_LBCK  (1 << 4)

#define CCCR_TXP  (1 << 14)
#define CCCR_EFBI (1 << 13)
#define CCCR_PXHD (1 << 12)
#define CCCR_BRSE (1 << 9)
#define CCCR_FDOE (1 << 8)
#define CCCR_TEST (1 << 7)
#define CCCR_DAR  (1 << 6)
#define CCCR_MON  (1 << 5)
#define CCCR_CSR  (1 << 4)
#define CCCR_CSA  (1 << 3)
#define CCCR_ASM  (1 << 2)
#define CCCR_CCE  (1 << 1)
#define CCCR_INIT (1 << 0)

#define NBTP_NSJW_M    0x7fu
#define NBTP_NSJW(x)   (((x) & NBTP_NSJW_M) << 25)
#define NBTP_NBRP_M    0x1ffu
#define NBTP_NBRP(x)   (((x) & NBTP_NBRP_M) << 16)
#define NBTP_NTSEG1_M  0xffu
#define NBTP_NTSEG1(x) (((x) & NBTP_NTSEG1_M) << 8)
#define NBTP_NTSEG2_M  0x7fu
#define NBTP_NTSEG2(x) ((x) & NBTP_NTSEG2_M)

#define TSCC_TCP_M  0xfu
#define TSCC_TCP(x) (((x) & TSCC_TCP_M) << 16)
#define TSCC_TSS_M  0x3u
#define TSCC_TSS(x) ((x) & TSCC_TSS_M)

#define TOCC_TOP_M  0xffffu
#define TOCC_TOP(x) (((x) & TOCC_TOP_M) << 16)
#define TOCC_TOS_M  0x3u
#define TOCC_TOS(x) (((x) & TOCC_TOS_M) << 1)
#define TOCC_ETOC   (1 << 0)

#define PSR_TDCV_M 0x7fu
#define PSR_TDCV(x) (((x) & PSR_TDCV_M) << 16)
#define PSR_PXE (1 << 14)
#define PSR_RFDF (1 << 13)
#define PSR_RBRS (1 << 12)
#define PSR_RESI (1 << 11)
#define PSR_DLEC_M 0x7u
#define PSR_DLEC(x) (((x) & PSR_DLEC_M) << 8)
#define PSR_BO (1 << 7)
#define PSR_EW (1 << 6)
#define PSR_EP (1 << 5)
#define PSR_ACT_M 0x3u
#define PSR_ACT(x) (((x) & PSR_ACT_M) << 3)
#define PSR_LEC_M 0x7u
#define PSR_LEC(x) ((x) & PSR_LEC_M)

#define TDCR_TDCO_M  0x7fu
#define TDCR_TDCO(x) (((x) & TDCR_TDCO_M) << 8)
#define TDCR_TDCF_M  0x7fu
#define TDCR_TDCF(x) ((x) & TDCR_TDCF_M)

#define GFC_ANFS_M  0x3u
#define GFC_ANFS(x) (((x) & GFC_ANFS_M) << 4)
#define GFC_ANFE_M  0x3u
#define GFC_ANFE(x) (((x) & GFC_ANFE_M) << 2)
#define GFC_RRFS    (1 << 1)
#define GFC_RRFE    (1 << 0)

#define SIDFC_LSS_M    0xffu
#define SIDFC_LSS(x)   (((x) & SIDFC_LSS_M) << 16)
#define SIDFC_FLSSA_M  0xffffu
#define SIDFC_FLSSA(x) ((x) & SIDFC_FLSSA_M)

#define XIDFC_LSE_M    0x7fu
#define XIDFC_LSE(x)   (((x) & XIDFC_LSE_M) << 16)
#define XIDFC_FLESA_M  0xffffu
#define XIDFC_FLESA(x) ((x) & XIDFC_FLESA_M)

#define XIDAM_EIDM_M  0x3fffffffu
#define XIDAM_EIDM(x) ((x) & XIDAM_EIDM_M)

#define RXFxC_FxOM    (1u << 31)
#define RXFxC_FxWM_M  0x7fu
#define RXFxC_FxWM(x) (((x) & RXFxC_FxWM_M) << 24)
#define RXFxC_FxS_M   0x7fu
#define RXFxC_FxS(x)  (((x) & RXFxC_FxS_M) << 16)
#define RXFxC_FxSA_M  0xffffu
#define RXFxC_FxSA(x) ((x) & RXFxC_FxSA_M)

#define RXFxS_RFxL    (1 << 25)
#define RXFxS_FxF     (1 << 24)
#define RXFxS_FxPI_M  0x3fu
#define RXFxS_FxPI(x) (((x) & RXFxS_FxPI_M) << 16)
#define RXFxS_FxGI_M  0x3fu
#define RXFxS_FxGI    (((x) & RXFxS_FxGI_M) << 8)
#define RXFxS_FxFL_M  0x7fu
#define RXFxS_FxFL(x) ((x) & RXFxS_FxFL_M)

#define RXFxA_FxAI_M  0x3fu
#define RXFxA_FxAI(x) ((x) & RXFxA_FxAI_M)

#define RXBC_RBSA_M  0xffffu
#define RXBC_RBSA(x) ((x) & RXBC_RBSA_M)

#define RXESC_RBDS_M  0x7u
#define RXESC_RBDS(x) (((x) & RXESC_RBDS_M) << 8)
#define RXESC_F1DS_M  0x7u
#define RXESC_F1DS(x) (((x) & RXESC_F1DS_M) << 4)
#define RXESC_F0DS_M  0x7u
#define RXESC_F0DS(x) ((x) &  RXESC_F0DS_M)

#define TXBC_TQFM    (1 << 30)
#define TXBC_TFQS_M  0x3fu
#define TXBC_TFQS(x) (((x) & TXBC_TFQS_M) << 24)
#define TXBC_NDTB_M  0x3fu
#define TXBC_NDTB(x) (((x) & TXBC_NDTB_M) << 16)
#define TXBC_TBSA_M  0xffffu
#define TXBC_TBSA(x) ((x) & TXBC_TBSA_M)

#define TXFQS_TFQF     (1 << 21)
#define TXFQS_TFQPI_M  0x1fu
#define TXFQS_TFQPI(x) (((x) & TXFQS_TFQPI_M) << 16)
#define TXFQS_TFGI_M   0x1fu
#define TXFQS_TFGI(x)  (((x) & TXFQS_TFGI_M) << 8)
#define TXFQS_TFFL_M   0x7fu
#define TXFQS_TFFL(x)  (((x) & TXFQS_TFFL_M)

#define TXESC_TBDS_M  0x7u
#define TXESC_TBDS(x) ((x) & TXESC_TBDS_M)

#define TXEFC_EFWM_M  0x3fu
#define TXEFC_EFWM(x) (((x) & TXEFC_EFWM_M) << 24)
#define TXEFC_EFS_M   0x3fu
#define TXEFC_EFS(x)  (((x) & TXEFC_EFS_M) << 16)
#define TXEFC_EFSA_M  0xffffu
#define TXEFC_EFSA(x) ((x) & TXEFC_EFSA_M)

#define TXEFS_TEFL    (1 << 25)
#define TXEFS_EFF     (1 << 24)
#define TXEFS_EFPI_M  0x1fu
#define TXEFS_EFPI(x) (((x) & TXEFS_EFPI_M) << 16)
#define TXEFS_EFGI_M  0x1fu
#define TXEFS_EFGI(x) (((x) & TXEFS_EFGI_M) << 8)
#define TXEFS_EFFL_M  0x3fu
#define TXEFS_EFFL(x) ((x) & TXEFS_EFFL_M)

#define TXEFA_EFAI_M  0x1fu
#define TXEFA_EFAI(x) ((x) & TXEFA_EFAI_M)

/* Standard Message ID Filter Element */

#define SMIFE_SFT_M    0x3u
#define SMIFE_SFT(x)   (((x) & SMIFE_SFT_M) << 30)
#define SMIFE_SFEC_M   0x7u
#define SMIFE_SFEC(x)  (((x) & SMIFE_SFEC_M) << 27)
#define SMIFE_SFID1_M  0x7ffu
#define SMIFE_SFID1(x) (((x) & SMIFE_SFID1_M) << 16)
#define SMIFE_SFID2_M  0x7ffu
#define SMIFE_SFID2(x) ((x) & SMIFE_SFID2_M)

/* Tx Buffer Element */

struct TXBE {
    volatile uint32_t T0;
    volatile uint32_t T1;
    volatile uint32_t T2;
    volatile uint32_t T3;
};

#define TXBE_T0_ESI   (1u << 31)
#define TXBE_T0_XTD   (1 << 30)
#define TXBE_T0_RTR   (1 << 29)
#define TXBE_T0_ID_M  0x1fffffffu
#define TXBE_T0_ID(x) ((x) & TXBE_T0_ID_M)

#define TXBE_T1_MM_M   0xffu
#define TXBE_T1_MM(x)  (((x) & TXBE_T1_MM_M) << 24)
#define TXBE_T1_EFC    (1 << 23)
#define TXBE_T1_FDF    (1 << 21)
#define TXBE_T1_BRS    (1 << 20)
#define TXBE_T1_DLC_M  0xfu
#define TXBE_T1_DLC(x) (((x) & TXBE_T1_DLC_M) << 16)

/* Rx Buffer and FIFO Element */

struct RXBE {
    volatile uint32_t R0;
    volatile uint32_t R1;
    volatile uint32_t R2;
    volatile uint32_t R3;
};

#define RXBE_R0_ESI (1u << 31)
#define RXBE_R0_XTD (1 << 30)
#define RXBE_R0_RTR (1 << 29)
#define RXBE_R0_ID_M  0x1fffffffu
#define RXBE_R0_ID(x) ((x) & TXBE_T0_ID_M)

#define RXBE_R1_ANMF    (1u << 31)
#define RXBE_R1_FIXD_M  0x3fu
#define RXBE_R1_FIDX(x) (((x) & RXBE_R1_FIXD_M) << 24)
#define RXBE_R1_FDF     (1 << 21)
#define RXBE_R1_BRS     (1 << 20)
#define RXBE_R1_DLC_M   0xfu
#define RXBE_R1_DLC(x)  (((x) & RXBE_R1_DLC_M) << 16)
#define RXBE_R1_RXTS_M  0xffffu
#define RXBE_R1_RXTS(x) ((x) & RXBE_R1_RXTS_M)

static int set_cccr(/*@reldef@*/ struct can *ctx, uint32_t value)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    ctx->base->CCCR = value;

    while (deadlock-- != 0)
        if (ctx->base->CCCR == value)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

/* Function: can_same5x_init
 * Inits a CAN interface
 *
 * Parameters:
 *  base - The CAN interface base address
 *  clkid - The associated clock identifier
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int can_same5x_init(struct can *ctx, struct CAN_SAME5X *base, clock_id_t clkid)
{
#define RELATIVE_ADDR(x) ((unsigned int)(x) & 0x1fffffffu)
    /* check for memory issues */
    if (!picoRTOS_assert(RELATIVE_ADDR(&ctx->sidf) <= SIDFC_FLSSA_M)) return -ENOMEM;
    if (!picoRTOS_assert(RELATIVE_ADDR(&ctx->rxf0) <= RXFxC_FxSA_M)) return -ENOMEM;
    if (!picoRTOS_assert(RELATIVE_ADDR(&ctx->txb) <= TXBC_TBSA_M)) return -ENOMEM;

    int res;

    ctx->base = base;
    ctx->clkid = clkid;
    ctx->sidf_count = 0;

    /* unprotect */
    if ((res = set_cccr(ctx, ctx->base->CCCR | (uint32_t)(CCCR_INIT | CCCR_CCE))) < 0)
        return res;

    /* FIXME: limited to fifo mode & 1 rx fifo for now */

    /* configure filter acceptance. FIXME */
    ctx->base->SIDFC = (uint32_t)(SIDFC_LSS(128) | SIDFC_FLSSA((uint32_t)&ctx->sidf));
    /* configure rfifo0. FIXME */
    ctx->base->RXF0C = (uint32_t)(RXFxC_FxS(64) | RXFxC_FxSA((uint32_t)&ctx->rxf0));
    /* configure tx buffer. FIXME */
    ctx->base->TXBC = (uint32_t)(TXBC_TFQS(32) | TXBC_TBSA((uint32_t)&ctx->txb));

    /* turn on (defaults to 500kb/s) */
    if ((res = set_cccr(ctx, ctx->base->CCCR & (uint32_t) ~(CCCR_INIT | CCCR_CCE))) < 0)
        return res;

    return 0;
}

#define TQ_MIN 4
#define TQ_MAX 49

#define DTSEG1_MIN 1
#define DTSEG1_MAX 32
#define DTSEG2_MIN 1
#define DTSEG2_MAX 16

static int set_dbrp_and_seg(struct can *ctx, size_t dbrp, size_t tq)
{
#define div_round_closest(a, b) (((a) + ((b) >> 1)) / (b))

    if (!picoRTOS_assert(tq >= (size_t)TQ_MIN)) return -EINVAL;
    if (!picoRTOS_assert(tq <= (size_t)TQ_MAX)) return -EINVAL;

    size_t dtseg1;
    size_t dtseg2;

    /* -1 (account for SYNC_SEG) */
    tq -= 1;

    /* default sample point is at 87.5% : 64 / 73 = 0.876 */
    dtseg1 = div_round_closest(tq * 64, 73);

    /* compute propag */
    if (dtseg1 > (size_t)DTSEG1_MAX)
        return -EINVAL;

    /* compute phase2 */
    dtseg2 = tq - dtseg1;

    /* ignore out of bounds values */
    if (dtseg1 < (size_t)DTSEG1_MIN || dtseg1 > (size_t)DTSEG1_MAX ||
        dtseg2 < (size_t)DTSEG2_MIN || dtseg2 > (size_t)DTSEG2_MAX)
        return -EINVAL;

    ctx->base->DBTP = (uint32_t)(DBTP_DBRP(dbrp - 1) |
                                 DBTP_DTSEG1(dtseg1 - 1) |
                                 DBTP_DTSEG2(dtseg2 - 1) |
                                 DBTP_DSJW(0));

    return 0;
}

static int set_bitrate(struct can *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    size_t dbrp;
    clock_freq_t freq;

    if ((freq = clock_get_freq(ctx->clkid)) < 0)
        return (int)freq;

    if (!picoRTOS_assert(((unsigned long)freq % bitrate) == 0))
        return -EIO;

    /* start iterative detection of params */
    for (dbrp = (size_t)1; dbrp <= (size_t)DBTP_DBRP_M; dbrp++) {
        size_t tq_div = (dbrp * (size_t)bitrate);
        size_t tq = (size_t)freq / tq_div;

        /* check for accuracy */
        if (((size_t)freq % tq_div) != 0)
            continue;

        /* ignore out of bounds values */
        if (tq < (size_t)TQ_MIN || tq > (size_t)TQ_MAX)
            continue;

        /* set values */
        if (set_dbrp_and_seg(ctx, dbrp, tq) == 0)
            return 0;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EINVAL;
}

int can_setup(struct can *ctx, struct can_settings *settings)
{
    int res;

    /* init mode */
    if ((res = set_cccr(ctx, ctx->base->CCCR | (uint32_t)(CCCR_INIT | CCCR_CCE))) < 0)
        return res;

    if ((res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* ignore auto abort for now */

    /* rx overwrite */
    if (settings->rx_overwrite) ctx->base->RXF0C |= RXFxC_FxOM;
    else ctx->base->RXF0C &= ~RXFxC_FxOM;

    if (settings->loopback) {
        ctx->base->CCCR |= CCCR_TEST;
        ctx->base->TEST |= TEST_LBCK;
    }else
        ctx->base->CCCR &= ~CCCR_TEST;

    /* ignore tx_mailboxes (1 tx fifo) */

    /* end init mode */
    if ((res = set_cccr(ctx, ctx->base->CCCR & ~(uint32_t)(CCCR_INIT | CCCR_CCE))) < 0)
        return res;

    return 0;
}

int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask)
{
    if (!picoRTOS_assert(id < (can_id_t)CAN_ID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(accept_mask < (can_id_t)CAN_ID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(ctx->sidf_count < (size_t)CAN_SAME5X_SIDF_WORD_COUNT)) return -ENOMEM;

    ctx->sidf[ctx->sidf_count++] = (uint32_t)(SMIFE_SFT(0x2) |  /* CLASSIC */
                                              SMIFE_SFEC(0x5) | /* PRIF0M */
                                              SMIFE_SFID1(id) |
                                              SMIFE_SFID2(accept_mask));
    /* out */
    return 0;
}

int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n)
{
    if (!picoRTOS_assert(id < (can_id_t)CAN_ID_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    const uint8_t *buf8 = (const uint8_t*)buf;
    struct TXBE *txbe = (struct TXBE*)ctx->txb;
    size_t index = (size_t)((ctx->base->TXFQS >> 16) & TXFQS_TFQPI_M);

    txbe[index].T0 = (uint32_t)TXBE_T0_ID(id << 18);
    txbe[index].T1 = (uint32_t)TXBE_T1_DLC(n);

    /* reset data */
    txbe[index].T2 = 0;
    txbe[index].T3 = 0;

    switch (n) {
    case 8: txbe[index].T3 |= buf8[7] << 24;    /*@fallthrough@*/
    case 7: txbe[index].T3 |= buf8[6] << 16;    /*@fallthrough@*/
    case 6: txbe[index].T3 |= buf8[5] << 8;     /*@fallthrough@*/
    case 5: txbe[index].T3 |= buf8[4] << 0;     /*@fallthrough@*/
    case 4: txbe[index].T2 |= buf8[3] << 24;    /*@fallthrough@*/
    case 3: txbe[index].T2 |= buf8[2] << 16;    /*@fallthrough@*/
    case 2: txbe[index].T2 |= buf8[1] << 8;     /*@fallthrough@*/
    case 1: txbe[index].T2 |= buf8[0] << 0; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    /* trigger transfer */
    ctx->base->TXBAR |= (1 << index);
    return (int)n;
}

int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n)
{
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    if ((ctx->base->RXF0S & RXFxS_FxFL_M) == 0)
        return -EAGAIN;

    uint8_t *buf8 = (uint8_t*)buf;
    struct RXBE *rxbe = (struct RXBE*)ctx->rxf0;
    size_t index = (size_t)((ctx->base->RXF0S >> 8) & RXFxS_FxGI_M);
    size_t dlc = (size_t)((rxbe[index].R1 >> 16) & RXBE_R1_DLC_M);

    /* take the lowest */
    n = MIN(dlc, n);

    switch (n) {
    case 8: buf8[7] = (uint8_t)(rxbe[index].R3 >> 24);  /*@fallthrough@*/
    case 7: buf8[6] = (uint8_t)(rxbe[index].R3 >> 16);  /*@fallthrough@*/
    case 6: buf8[5] = (uint8_t)(rxbe[index].R3 >> 8);   /*@fallthrough@*/
    case 5: buf8[4] = (uint8_t)(rxbe[index].R3 >> 0);   /*@fallthrough@*/
    case 4: buf8[3] = (uint8_t)(rxbe[index].R2 >> 24);  /*@fallthrough@*/
    case 3: buf8[2] = (uint8_t)(rxbe[index].R2 >> 16);  /*@fallthrough@*/
    case 2: buf8[1] = (uint8_t)(rxbe[index].R2 >> 8);   /*@fallthrough@*/
    case 1: buf8[0] = (uint8_t)(rxbe[index].R2 >> 0); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* FIXME: EXTID */
    *id = (can_id_t)(rxbe[index].R0 & RXBE_R0_ID_M) >> 18;

    /* acknowledge */
    ctx->base->RXF0A = (uint32_t)RXFxA_FxAI(index);
    return (int)n;
}

int can_request_frame(/*@unused@*/ struct can *ctx __attribute__ ((unused)),
                      /*@unused@*/ can_id_t id __attribute__((unused)))
{
    return -ENOSYS;
}

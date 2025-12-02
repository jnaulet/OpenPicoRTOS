#include "can-stm32h7xx_fdcan.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct CAN_STM32H7XX_FDCAN {
    volatile uint32_t CREL;
    volatile uint32_t ENDN;
    uint32_t RESERVED0;
    volatile uint32_t DBTP;
    volatile uint32_t TEST;
    volatile uint32_t RWD;
    volatile uint32_t CCCR;
    volatile uint32_t NBTP;
    volatile uint32_t TSCC;
    volatile uint32_t TSCV;
    volatile uint32_t TOCC;
    volatile uint32_t TOCV;
    uint32_t RESERVED1[4];
    volatile uint32_t ECR;
    volatile uint32_t PSR;
    volatile uint32_t TDCR;
    uint32_t RESERVED2;
    volatile uint32_t IR;
    volatile uint32_t IE;
    volatile uint32_t ILS;
    volatile uint32_t ILE;
    uint32_t RESERVED3[8];
    volatile uint32_t GFC;
    volatile uint32_t SIDFC;
    volatile uint32_t XIDFC;
    uint32_t RESERVED4;
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
    volatile uint32_t TXBCR;
    volatile uint32_t TXBTO;
    volatile uint32_t TXBCF;
    volatile uint32_t TXBTIE;
    volatile uint32_t TXBCIE;
    uint32_t RESERVED5[2];
    volatile uint32_t TXEFC;
    volatile uint32_t TXEFS;
    volatile uint32_t TXEFA;
};

#define CREL_REL_M      0xfu
#define CREL_REL(x)     (((x) & CREL_REL_M) << 28)
#define CREL_STEP_M     0xfu
#define CREL_STEP(x)    (((x) & CREL_STEP_M) << 24)
#define CREL_SUBSTEP_M  0xfu
#define CREL_SUBSTEP(x) (((x) & CREL_SUBSTEP_M) << 20)
#define CREL_YEAR_M     0xfu
#define CREL_YEAR(x)    (((x) & CREL_YEAR_M) << 16)
#define CREL_MON_M      0xffu
#define CREL_MON(x)     (((x) & CREL_MON_M) << 8)
#define CREL_DAY_M      0xffu
#define CREL_DAY(x)     ((x) & CREL_DAY_M)

#define C99_ENDN_ETV_M  0xffffffffu
#define C99_ENDN_ETV(x) ((x) & C99_ENDN_ETV_M)

#define DBTP_TDC       (1 << 23)
#define DBTP_DBRP_M    0x1fu
#define DBTP_DBRP(x)   (((x) & DBTP_DBRP_M) << 16)
#define DBTP_DTSEG_M   0x1fu
#define DBTP_DTSEG(x)  (((x) & DBTP_DTSEG_M) << 8)
#define DBTP_DTSEG2_M  0xfu
#define DBTP_DTSEG2(x) (((x) & DBTP_DTSEG2_M) << 4)
#define DBTP_DSJW_M    0xfu
#define DBTP_DSJW(x)   ((x) & DBTP_DSJW_M)

#define TEST_RX    (1 << 7)
#define TEST_TX_M  0x3u
#define TEST_TX(x) (((x) & TEST_TX_M) << 5)
#define TEST_LBCK  (1 << 4)

#define RWD_WDV_M  0xffu
#define RWD_WDV(x) (((x) & RWD_WDV_M) << 8)
#define RWD_WDC_M  0xffu
#define RWD_WDC(x) ((x) & RWD_WDC_M)

#define CCCR_NISO (1 << 15)
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

#define TSCV_TSC_M  0xffffu
#define TSCV_TSC(x) ((x) & TSCV_TSC_M)

#define TOCC_TOP_M  0xffffu
#define TOCC_TOP(x) (((x) & TOCC_TOP_M) << 16)
#define TOCC_TOS_M  0x3u
#define TOCC_TOS(x) (((x) & TOCC_TOS_M) << 1)
#define TOCC_ETOC   (1 << 0)

#define TOCV_TOC_M  0xffffu
#define TOCV_TOC(x) ((x) & TOCV_TOC_M)

#define C99_ECR_CEL_M 0xffu
#define C99_ECR_CEL(x) (((x) & C99_ECR_CEL_M) << 16)
#define C99_ECR_RP     (1 << 15)
#define C99_ECR_REC_M  0x7fu
#define C99_ECR_REC(x) (((x) & C99_ECR_REC_M) << 8)
#define C99_ECR_TEC_M  0xffu
#define C99_ECR_TEC(x) ((x) & C99_ECR_TEC_M)

#define PSR_PXE     (1 << 14)
#define PSR_REDL    (1 << 13)
#define PSR_RBRS    (1 << 12)
#define PSR_RESI    (1 << 11)
#define PSR_DLEC_M  0x7u
#define PSR_DLEC(x) (((x) & PSR_DLEC_M) << 8)
#define PSR_BO      (1 << 7)
#define PSR_EW      (1 << 6)
#define PSR_EP      (1 << 5)
#define PSR_ACT_M   0x3u
#define PSR_ACT(x)  (((x) & PSR_ACT_M) << 3)
#define PSR_LEC_M   0x7u
#define PSR_LEC(x)  ((x) & PSR_LEC_M)

#define TDCR_TDCO_M 0x7fu
#define TDCR_TDCO(x) (((x) & TDCR_TDCO_M) << 8)
#define TDCR_TDCF_M  0x7fu
#define TDCR_TDCF(x) ((x) & TDCR_TDCF_M)

#define IR_ARA  (1 << 29)
#define IR_PED  (1 << 28)
#define IR_PEA  (1 << 27)
#define IR_WDI  (1 << 26)
#define IR_BO   (1 << 25)
#define IR_EW   (1 << 24)
#define IR_EP   (1 << 23)
#define IR_ELO  (1 << 22)
#define IR_DRX  (1 << 19)
#define IR_TOO  (1 << 18)
#define IR_MRAF (1 << 17)
#define IR_TSW  (1 << 16)
#define IR_TEFL (1 << 15)
#define IR_TEFF (1 << 14)
#define IR_TEFW (1 << 13)
#define IR_TEFN (1 << 12)
#define IR_TFE  (1 << 11)
#define IR_TCF  (1 << 10)
#define IR_TC   (1 << 9)
#define IR_HPM  (1 << 8)
#define IR_RF1L (1 << 7)
#define IR_RF1F (1 << 6)
#define IR_RF1W (1 << 5)
#define IR_RF1N (1 << 4)
#define IR_RF0L (1 << 3)
#define IR_RF0F (1 << 2)
#define IR_RF0W (1 << 1)
#define IR_RF0N (1 << 0)

#define IE_ARAE  (1 << 29)
#define IE_PEDE  (1 << 28)
#define IE_PEAE  (1 << 27)
#define IE_WDIE  (1 << 26)
#define IE_BOE   (1 << 25)
#define IE_EWE   (1 << 24)
#define IE_EPE   (1 << 23)
#define IE_ELOE  (1 << 22)
#define IE_DRXE  (1 << 19)
#define IE_TOOE  (1 << 18)
#define IE_MRAFE (1 << 17)
#define IE_TSWE  (1 << 16)
#define IE_TEFLE (1 << 15)
#define IE_TEFFE (1 << 14)
#define IE_TEFWE (1 << 13)
#define IE_TEFNE (1 << 12)
#define IE_TFEE  (1 << 11)
#define IE_TCFE  (1 << 10)
#define IE_TCE   (1 << 9)
#define IE_HPME  (1 << 8)
#define IE_RF1LE (1 << 7)
#define IE_RF1FE (1 << 6)
#define IE_RF1WE (1 << 5)
#define IE_RF1NE (1 << 4)
#define IE_RF0LE (1 << 3)
#define IE_RF0FE (1 << 2)
#define IE_RF0WE (1 << 1)
#define IE_RF0NE (1 << 0)

#define ILS_ARAL  (1 << 29)
#define ILS_PEDL  (1 << 28)
#define ILS_PEAL  (1 << 27)
#define ILS_WDIL  (1 << 26)
#define ILS_BOL   (1 << 25)
#define ILS_EWL   (1 << 24)
#define ILS_EPL   (1 << 23)
#define ILS_ELOL  (1 << 22)
#define ILS_DRXL  (1 << 19)
#define ILS_TOOL  (1 << 18)
#define ILS_MRAFL (1 << 17)
#define ILS_TSWL  (1 << 16)
#define ILS_TEFLL (1 << 15)
#define ILS_TEFFL (1 << 14)
#define ILS_TEFWL (1 << 13)
#define ILS_TEFNL (1 << 12)
#define ILS_TFEL  (1 << 11)
#define ILS_TCFL  (1 << 10)
#define ILS_TCL   (1 << 9)
#define ILS_HPML  (1 << 8)
#define ILS_RF1LL (1 << 7)
#define ILS_RF1FL (1 << 6)
#define ILS_RF1WL (1 << 5)
#define ILS_RF1NL (1 << 4)
#define ILS_RF0LL (1 << 3)
#define ILS_RF0FL (1 << 2)
#define ILS_RF0WL (1 << 1)
#define ILS_RF0NL (1 << 0)

#define ILE_EINT1 (1 << 1)
#define ILE_EINT0 (1 << 0)

#define GFC_ANFS_M  0x3u
#define GFC_ANFS(x) (((x) & GFC_ANFS_M) << 4)
#define GFC_ANFE_M  0x3u
#define GFC_ANFE(x) (((x) & GFC_ANFE_M) << 2)
#define GFC_EINT1   (1 << 1)
#define GFC_EINT0   (1 << 0)

#define SIDFC_LSS_M    0xffu
#define SIDFC_LSS(x)   (((x) & SIDFC_LSS_M) << 16)
#define SIDFC_FLSSA_M  0x3fffu
#define SIDFC_FLSSA(x) (((x) & SIDFC_FLSSA_M) << 2)

#define XIDFC_LSE_M    0xffu
#define XIDFC_LSE(x)   (((x) & XIDFC_LSE_M) << 16)
#define XIDFC_FLESA_M  0x3fffu
#define XIDFC_FLESA(x) (((x) & XIDFC_FLESA_M) << 2)

#define XIDAM_EIDM_M  0x1fffffffu
#define XIDAM_EIDM(x) ((x) & XIDAM_EIDM_M)

#define HPMS_FLST    (1 << 15)
#define HPMS_FIDX_M  0x7fu
#define HPMS_FIDX(x) (((x) & HPMS_FIDX_M) << 8)
#define HPMS_MISI_M  0x3u
#define HPMS_MISI(x) (((x) & HPMS_MISI_M) << 6)
#define HPMS_MIDX_M  0x3fu
#define HPMS_MIDX(x) ((x) & HPMS_MIDX_M)

#define RXFnC_FnOM    (1u << 31)
#define RXFnC_FnWM_M  0x7fu
#define RXFnC_FnWM(x) (((x) & RXFnC_FnWM_M) << 24)
#define RXFnC_FnS_M   0x7fu
#define RXFnC_FnS(x)  (((x) & RXFnC_FnS_M) << 16)
#define RXFnC_FnSA_M  0x3fffu
#define RXFnC_FnSA(x) (((x) & RXFnC_FnSA_M) << 2)

#define RXFnS_RFn     (1 << 25)
#define RXFnS_FnF     (1 << 24)
#define RXFnS_FnPI_M  0x3fu
#define RXFnS_FnPI(x) (((x) & RXFnS_FnPI_M) << 16)
#define RXFnS_FnGI_M  0x3fu
#define RXFnS_FnGI(x) (((x) & RXFnS_FnGI_M) << 8)
#define RXFnS_FnFL_M  0x3fu
#define RXFnS_FnFL(x) ((x) & RXFnS_FnFL_M)

#define RXFnA_FnAI_M  0x3fu
#define RXFnA_FnAI(x) ((x) & RXFnA_FnAI_M)

#define RXBC_RBSA_M  0x3fffu
#define RXBC_RBSA(x) (((x) & RXBC_RBSA_M) << 2)

#define RXF1S_DMS_M  0x3u
#define RXF1S_DMS(x) (((x) & RXF1S_DMS_M) << 30)

#define RXESC_RDBS_M  0x7u
#define RXESC_RDBS(x) (((x) & RXESC_RDBS_M) << 10)
#define RXESC_FD1S_M  0x7u
#define RXESC_FD1S(x) (((x) & RXESC_FD1S_M) << 5)
#define RXESC_FD0S_M  0x7u
#define RXESC_FD0S(x) ((x) & RXESC_FD0S_M)

#define TXBC_TFQM    (1 << 30)
#define TXBC_TFQS_M  0x3fu
#define TXBC_TFQS(x) (((x) & TXBC_TFQS_M) << 24)
#define TXBC_NDTB_M  0x3fu
#define TXBC_NDTB(x) (((x) & TXBC_NDTB_M) << 16)
#define TXBC_TBSA_M  0x3fffu
#define TXBC_TBSA(x) (((x) & TXBC_TBSA_M) << 2)

#define TXFQS_TFQF     (1 << 21)
#define TXFQS_TFQPI_M  0x1fu
#define TXFQS_TFQPI(x) (((x) & TXFQS_TFQPI_M) << 16)
#define TXFQS_TFGI_M   0x1fu
#define TXFQS_TFGI(x)  (((x) & TXFQS_TFGI_M) << 8)
#define TXFQS_TFFL_M   0x3fu
#define TXFQS_TFFL(x)  ((x) & TXFQS_TFFL_M)

#define TXESC_TBDS_M  0x7u
#define TXESC_TBDS(x) ((x) & TXESC_TBDS_M)

#define TXEFC_EFWM_M  0x3fu
#define TXEFC_EFWM(x) (((x) & TXEFC_EFWM_M) << 24)
#define TXEFC_EFS_M   0x3fu
#define TXEFC_EFS(x)  (((x) & TXEFC_EFS_M) << 16)
#define TXEFC_EFSA_M  0x3fffu
#define TXEFC_EFSA(x) (((x) & TXEFC_EFSA_M) << 2)

#define TXEFS_TEF     (1 << 25)
#define TXEFS_EFF     (1 << 24)
#define TXEFS_EFPI_M  0x1fu
#define TXEFS_EFPI(x) (((x) & TXEFS_EFPI_M) << 16)
#define TXEFS_EFGI_M  0x1fu
#define TXEFS_EFGI(x) (((x) & TXEFS_EFGI_M) << 8)
#define TXEFS_EFFL_M  0x3fu
#define TXEFS_EFFL(x) ((x) & TXEFS_EFFL_M)

#define TXEFA_EFA_M  0x1fu
#define TXEFA_EFA(x) ((x) & TXEFA_EFA_M)

/*
 * Message RAM
 */
// static uint32_t *CAN_MSG_RAM = (uint32_t)ADDR_CAN_MSG_RAM;

#define MESSAGE_RAM_WORDS_COUNT 2560

/*
 * buffer elements
 */
struct rx_buf {
    volatile uint32_t R0;
    volatile uint32_t R1;
    volatile uint32_t R2;
    volatile uint32_t R3;
} __attribute__((packed));

#define R0_ESI   (1u << 31)
#define R0_XTD   (1 << 30)
#define R0_RTR   (1 << 29)
#define R0_ID_M  0x1fffffffu
#define R0_ID(x) ((x) & R0_ID_M)

#define R1_ANMF    (1u << 31)
#define R1_FIDX_M  0x7fu
#define R1_FIDX(x) (((x) & R1_FIDX_M) << 24)
#define R1_FDF     (1 << 22)
#define R1_BRS     (1 << 21)
#define R1_DLC_M   0xfu
#define R1_DLC(x)  (((x) & R1_DLC_M) << 16)
#define R1_RXTS_M  0xffffu
#define R1_RXTS(x) ((x) & R1_RXTS_M)

struct tx_buf {
    volatile uint32_t T0;
    volatile uint32_t T1;
    volatile uint32_t T2;
    volatile uint32_t T3;
} __attribute__((packed));

#define T0_ESI   (1u << 31)
#define T0_XTD   (1 << 30)
#define T0_RTR   (1 << 29)
#define T0_ID_M  0x1fffffffu
#define T0_ID(x) ((x) & T0_ID_M)

#define T1_MM_M   0xffu
#define T1_MM(x)  (((x) & T1_MM_M) << 24)
#define T1_EFC    (1 << 23)
#define T1_FDF    (1 << 21)
#define T1_BPS    (1 << 20)
#define T1_DLC_M  0xfu
#define T1_DLC(x) (((x) & T1_DLC_M) << 16)

struct tx_event {
    volatile uint32_t E0;
    volatile uint32_t E1;
} __attribute__((packed));

#define C99_E0_ESI   (1u << 31)
#define C99_E0_XTD   (1 << 30)
#define C99_E0_RTR   (1 << 29)
#define C99_E0_ID_M  0x1fffffffu
#define C99_E0_ID(x) ((x) & C99_E0_ID_M)

#define C99_E1_MM_M    0xffu
#define C99_E1_MM(x)   (((x) & C99_E1_MM_M) << 24)
#define C99_E1_ET_M    0x3u
#define C99_E1_ET(x)   (((x) & C99_E1_ET_M) << 22)
#define C99_E1_EDL     (1 << 21)
#define C99_E1_BRS     (1 << 20)
#define C99_E1_DLC_M   0xfu
#define C99_E1_DLC(x)  (((x) & C99_E1_DLC_M) << 16)
#define C99_E1_TXTS_M  0xffffu
#define C99_E1_TXTS(x) ((x) & C99_E1_TXTS_M)

struct std_filter {
    volatile uint32_t S0;
};

#define S0_SFT_M    0x3u
#define S0_SFT(x)   (((x) & S0_SFT_M) << 30)
#define S0_SFEC_M   0x7u
#define S0_SFEC(x)  (((x) & S0_SFEC_M) << 27)
#define S0_SFID_M   0x7ffu
#define S0_SFID(x)  (((x) & S0_SFID_M) << 16)
#define S0_SFID2_M  0x7ffu
#define S0_SFID2(x) ((x) & S0_SFID2_M)

struct ext_filter {
    volatile uint32_t F0;
    volatile uint32_t F1;
};

#define F0_EFEC_M 0x7u
#define F0_EFEC(x) (((x) & F0_EFEC_M) << 29)
#define F0_EFID1_M 0x1fffffffu
#define F0_EFID1(x) ((x) & F0_EFID1_M)

#define F1_EFTI_M 0x3u
#define F1_EFTI(x) (((x) & F1_EFTI_M) << 30)
#define F1_EFID2_M 0x1fffffffu
#define F1_EFID2(x) ((x) & F1_EFID2_M)

#define RX_FIFOn_SIZE_COUNT     64
#define TX_BUFFERS_SIZE_COUNT   32
#define STD_FILTERS_SIZE_COUNT  128
#define XTD_FILTERS_SIZE_COUNT  64

static int set_init_cce(/*@reldef@*/ const struct can *ctx, bool value)
{
    uint32_t cccr = ctx->base->CCCR;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    if (value) cccr |= (CCCR_INIT | CCCR_CCE);
    else cccr &= ~(CCCR_INIT | CCCR_CCE);

    do{
        ctx->base->CCCR = cccr;
        arch_delay_us(1ul);
    } while (ctx->base->CCCR != cccr && deadlock-- != 0);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static void init_fdcan_filters(struct can *ctx)
{
    struct std_filter *sf = (struct std_filter *)ctx->filter_11bit;
    struct ext_filter *ef = (struct ext_filter *)ctx->filter_29bit;

    /* std */
    for (int n = STD_FILTERS_SIZE_COUNT; n-- != 0;)
        sf[n].S0 = (uint32_t)S0_SFT(0x3);
    /* extended */
    for (int n = XTD_FILTERS_SIZE_COUNT; n-- != 0;)
        ef[n].F0 = (uint32_t)F0_EFEC(0x0);

    /* don't accept non-matching messages */
    ctx->base->GFC |= (GFC_ANFS(0x3) | GFC_ANFE(0x3));
}

int can_stm32h7xx_fdcan_init(struct can *ctx, int base, clock_id_t clkid,
                             uint32_t start_address, size_t n_words)
{
    picoRTOS_assert((start_address & 0x3u) == 0, return -EINVAL);
    picoRTOS_assert(n_words > 0, return -EINVAL);
    picoRTOS_assert(n_words <= (size_t)MESSAGE_RAM_WORDS_COUNT, return -EINVAL);

    int res;

    ctx->base = (struct CAN_STM32H7XX_FDCAN*)base;
    ctx->clkid = clkid;
    ctx->n_filter_11bit = 0;
    ctx->n_filter_29bit = 0;
    ctx->n_words = n_words;
    ctx->bus_off_recovery = 0;

    /* software initialization */
    if ((res = set_init_cce(ctx, true)) < 0)
        return res;

    /* ensure std CAN-only (8 bytes, no FD) */
    ctx->base->RXESC = (uint32_t)(RXESC_RDBS(0) | RXESC_FD1S(0) | RXESC_FD0S(0));
    ctx->base->TXESC = (uint32_t)TXESC_TBDS(0);

    /*
     * TODO: organize RAM according to n_words
     */

    /* set rx FIFO0 start address,
     * as we're pure CAN we can max the size, too */
    ctx->rx_fifo0 = (void*)(ADDR_CAN_MSG_RAM + (start_address << 2));
    ctx->base->RXF0C = (uint32_t)(RXFnC_FnS(RX_FIFOn_SIZE_COUNT) |
                                  RXFnC_FnSA(start_address));
    /* tx buffers start address follows */
    start_address += RX_FIFOn_SIZE_COUNT * 4; /* 4 32bit words per msg */
    ctx->tx_buffers = (void*)(ADDR_CAN_MSG_RAM + (start_address << 2));
    ctx->base->TXBC = (uint32_t)(TXBC_TFQS(TX_BUFFERS_SIZE_COUNT) |
                                 TXBC_TBSA(start_address));
    /* normal acceptance masks */
    start_address += TX_BUFFERS_SIZE_COUNT * 4; /* 4 32bit words per msg */
    ctx->filter_11bit = (void*)(ADDR_CAN_MSG_RAM + (start_address << 2));
    ctx->base->SIDFC = (uint32_t)(SIDFC_LSS(STD_FILTERS_SIZE_COUNT) |
                                  SIDFC_FLSSA(start_address));
    /* extended acceptance masks */
    start_address += STD_FILTERS_SIZE_COUNT; /* 1 32bit word per entry */
    ctx->filter_29bit = (void*)(ADDR_CAN_MSG_RAM + (start_address << 2));
    ctx->base->XIDFC = (uint32_t)(SIDFC_LSS(XTD_FILTERS_SIZE_COUNT) |
                                  SIDFC_FLSSA(start_address));
    /* future-use pointers init */
    start_address += STD_FILTERS_SIZE_COUNT * 2; /* 2 32bit word per entry */
    ctx->rx_fifo1 = (void*)(ADDR_CAN_MSG_RAM + (start_address << 2));
    ctx->rx_buffer = (void*)(ADDR_CAN_MSG_RAM + (start_address << 2));
    ctx->tx_event_fifo = (void*)(ADDR_CAN_MSG_RAM + (start_address << 2));
    ctx->trigger_memory = (void*)(ADDR_CAN_MSG_RAM + (start_address << 2));

    /* init memory */
    init_fdcan_filters(ctx);

    /* end init */
    return set_init_cce(ctx, false);
}

#define TSEG1_MIN  1
#define TSEG1_MAX  (DBTP_DTSEG_M + 1)
#define TSEG2_MIN  1
#define TSEG2_MAX  (DBTP_DTSEG2_M + 1)
#define SJW_MIN    1
#define SJW_MAX    (DBTP_DSJW_M + 1)
#define TQ_MIN     1
#define TQ_MAX     (TSEG1_MAX + TSEG2_MAX + 3) /* according to RM */

static int set_data_bit_timing(struct can *ctx, size_t brp, size_t tq)
{
#define div_round_closest(a, b) (((a) + ((b) >> 1)) / (b))

    picoRTOS_assert(brp > 0, return -EINVAL);
    picoRTOS_assert(brp <= (size_t)(DBTP_DBRP_M + 1u), return -EINVAL);
    picoRTOS_assert(tq >= (size_t)TQ_MIN, return -EINVAL);
    picoRTOS_assert(tq <= (size_t)TQ_MAX, return -EINVAL);

    size_t tseg1;
    size_t tseg2;
    size_t sjw;

    /* SYNC_SEG */
    tq -= 1;

    /* default sample point is at 87.5% : 64 / 73 = 0.876 */
    tseg1 = div_round_closest(tq * 64, 73);
    /* compute tseg2 & sjw */
    tseg2 = tq - tseg1;
    sjw = tseg2 - 1;

    /* ignore out of bounds values */
    if (tseg1 < (size_t)TSEG1_MIN || tseg1 > (size_t)TSEG1_MAX ||
        tseg2 < (size_t)TSEG2_MIN || tseg2 > (size_t)TSEG2_MAX ||
        sjw < (size_t)SJW_MIN || sjw > (size_t)SJW_MAX)
        return -EINVAL;

    /* set DBTP */
    ctx->base->DBTP = (uint32_t)(DBTP_DBRP(brp - 1) |
                                 DBTP_DTSEG(tseg1 - 1) |
                                 DBTP_DTSEG2(tseg2 - 1) |
                                 DBTP_DSJW(sjw - 1));

    /* set NBTP to the exact same values */
    ctx->base->NBTP = (uint32_t)(NBTP_NSJW(sjw - 1) |
                                 NBTP_NBRP(brp - 1) |
                                 NBTP_NTSEG1(tseg1 - 1) |
                                 NBTP_NTSEG2(tseg2 - 1));

#undef div_round_closest
    return 0;
}

static int set_bitrate(struct can *ctx, unsigned long bitrate)
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
    for (brp = (size_t)1; brp <= (size_t)(DBTP_DBRP_M + 1u); brp++) {
        size_t tq_div = (brp * (size_t)bitrate);
        size_t tq = (size_t)freq / tq_div;

        /* check for accuracy */
        if (((size_t)freq % tq_div) != 0)
            continue;

        /* ignore out of bounds values */
        if (tq < (size_t)TQ_MIN || tq > (size_t)TQ_MAX)
            continue;

        if (set_data_bit_timing(ctx, brp, tq) == 0)
            return 0;
    }

    picoRTOS_assert_void(false);
    /*@notreached@*/
    return -EINVAL;
}

static int set_tx_fifo(struct can *ctx,
                       size_t tx_mailbox_count,
                       can_tx_auto_abort_t tx_auto_abort)
{
    picoRTOS_assert(tx_mailbox_count > 0, return -EINVAL);
    picoRTOS_assert(tx_mailbox_count <= (size_t)TX_BUFFERS_SIZE_COUNT, return -EINVAL);
    picoRTOS_assert(tx_auto_abort < CAN_TX_AUTO_ABORT_COUNT, return -EINVAL);

    /* set flags */
    if (tx_auto_abort == CAN_TX_AUTO_ABORT_ON) ctx->base->TXBC |= TXBC_TFQM;
    else ctx->base->TXBC &= ~TXBC_TFQM;

    /* set count */
    ctx->base->TXBC &= ~TXBC_TFQS(TXBC_TFQS_M);
    ctx->base->TXBC |= TXBC_TFQS(tx_mailbox_count);

    return 0;
}

static int set_rx_fifo(struct can *ctx, can_rx_overwrite_t rx_overwrite)
{
    picoRTOS_assert(rx_overwrite < CAN_RX_OVERWRITE_COUNT, return -EINVAL);

    if (rx_overwrite == CAN_RX_OVERWRITE_ON) {
        ctx->base->RXF0C |= RXFnC_FnOM;
        ctx->base->RXF1C |= RXFnC_FnOM;
    }else{
        ctx->base->RXF0C &= ~RXFnC_FnOM;
        ctx->base->RXF1C &= ~RXFnC_FnOM;
    }

    return 0;
}

static int set_loopback_mode(struct can *ctx, can_loopback_t loopback)
{
    picoRTOS_assert(loopback < CAN_LOOPBACK_COUNT, return -EINVAL);

    if (loopback == CAN_LOOPBACK_ON) {
        /* this is definitive */
        ctx->base->CCCR |= (CCCR_TEST | CCCR_MON);
        ctx->base->TEST |= TEST_LBCK;
    }

    return 0;
}

int can_setup(struct can *ctx, const struct can_settings *settings)
{
    int res;

    /* software initialization */
    if ((res = set_init_cce(ctx, true)) < 0)
        return res;

    if ((res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if ((res = set_tx_fifo(ctx, settings->tx_mailbox_count,
                           settings->tx_auto_abort)) < 0)
        return res;

    if ((res = set_rx_fifo(ctx, settings->rx_overwrite)) < 0)
        return res;

    if ((res = set_loopback_mode(ctx, settings->loopback)) < 0)
        return res;

    /* end init */
    return set_init_cce(ctx, false);
}

int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask)
{
    picoRTOS_assert(id < (can_id_t)CAN_EXTID_COUNT, return -EINVAL);
    picoRTOS_assert(accept_mask < (can_id_t)CAN_EXTID_COUNT, return -EINVAL);

    if (id < (can_id_t)CAN_ID_COUNT) {
        /* simple */
        picoRTOS_assert(ctx->n_filter_11bit < (size_t)STD_FILTERS_SIZE_COUNT,
                        return -ENOMEM);

        struct std_filter *sf = (struct std_filter *)ctx->filter_11bit;
        sf[ctx->n_filter_11bit++].S0 = (uint32_t)(S0_SFT(0x2) | S0_SFEC(0x1) |
                                                  S0_SFID(id) | S0_SFID2(accept_mask));
    }else{
        /* extended */
        picoRTOS_assert(ctx->n_filter_29bit < (size_t)XTD_FILTERS_SIZE_COUNT,
                        return -ENOMEM);

        struct ext_filter *ef = (struct ext_filter *)ctx->filter_29bit;

        ef[ctx->n_filter_29bit].F0 = (uint32_t)(F0_EFEC(0x1) | F0_EFID1(id));
        ef[ctx->n_filter_29bit++].F1 = (uint32_t)(F1_EFTI(0x2) | F1_EFID2(accept_mask));
    }

    return 0;
}

static int buffer_write(struct tx_buf *txb, const void *buf, size_t n)
{
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    const uint8_t *buf8 = (const uint8_t*)buf;

    /* reset buffer */
    txb->T2 = 0;
    txb->T3 = 0;

    switch (n) {
    case 8: txb->T3 |= (uint32_t)buf8[7] << 24;         /*@fallthrough@*/
    case 7: txb->T3 |= (uint32_t)buf8[6] << 16;         /*@fallthrough@*/
    case 6: txb->T3 |= (uint32_t)buf8[5] << 8;          /*@fallthrough@*/
    case 5: txb->T3 |= (uint32_t)buf8[4] << 0;          /*@fallthrough@*/
    case 4: txb->T2 |= (uint32_t)buf8[3] << 24;         /*@fallthrough@*/
    case 3: txb->T2 |= (uint32_t)buf8[2] << 16;         /*@fallthrough@*/
    case 2: txb->T2 |= (uint32_t)buf8[1] << 8;          /*@fallthrough@*/
    case 1: txb->T2 |= (uint32_t)buf8[0] << 0; break;
    default:
        picoRTOS_assert_void(false);
        /*@notreached@*/
        return -EIO;
    }

    return (int)n;
}

int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n)
{
#define PSR_LEC_BIT0ERROR      5u
#define BUS_OFF_RECOVERY_COUNT 129
    /* picoRTOS_assert(n > 0, return -EINVAL); */
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    int dlc;
    uint32_t txfqs = ctx->base->TXFQS;

    /* bus_off recovery */
    if (ctx->bus_off_recovery != 0 &&
        (ctx->base->PSR & PSR_LEC_M) == PSR_LEC_BIT0ERROR) {
        ctx->bus_off_recovery--;
        return -EBUSY;
    }

    /* bus_off management */
    if ((ctx->base->PSR & PSR_BO) != 0) {
        ctx->base->CCCR &= ~CCCR_INIT; /* hotfix */
        /* recovery takes 129x11 idle bits */
        ctx->bus_off_recovery = BUS_OFF_RECOVERY_COUNT;
        return -EIO; /* -ECOMM; */
    }

    if ((txfqs & TXFQS_TFQF) != 0)
        return -EAGAIN;

    unsigned index = (txfqs & TXFQS_TFQPI(TXFQS_TFQPI_M)) >> 16;
    struct tx_buf *tx_buf = (struct tx_buf*)ctx->tx_buffers;

    /* TODO: use bit31/0 to mark ESI */
    if (id < (can_id_t)CAN_ID_COUNT) tx_buf[index].T0 = (uint32_t)T0_ID(id << 18);
    else tx_buf[index].T0 = (uint32_t)(T0_XTD | T0_ID(id));

    tx_buf[index].T1 = (uint32_t)T1_DLC(n);
    dlc = buffer_write(&tx_buf[index], buf, n);

    /* request xfer */
    ctx->base->TXBAR |= (uint32_t)(1 << index);
    return dlc;
}

static int buffer_read(const struct rx_buf *rxb, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    uint8_t *buf8 = (uint8_t*)buf;

    switch (n) {
    case 8: buf8[7] = (uint8_t)(rxb->R3 >> 24);         /*@fallthrough@*/
    case 7: buf8[6] = (uint8_t)(rxb->R3 >> 16);         /*@fallthrough@*/
    case 6: buf8[5] = (uint8_t)(rxb->R3 >> 8);          /*@fallthrough@*/
    case 5: buf8[4] = (uint8_t)(rxb->R3 >> 0);          /*@fallthrough@*/
    case 4: buf8[3] = (uint8_t)(rxb->R2 >> 24);         /*@fallthrough@*/
    case 3: buf8[2] = (uint8_t)(rxb->R2 >> 16);         /*@fallthrough@*/
    case 2: buf8[1] = (uint8_t)(rxb->R2 >> 8);          /*@fallthrough@*/
    case 1: buf8[0] = (uint8_t)(rxb->R2 >> 0); break;
    default:
        picoRTOS_assert_void(false);
        /*@notreached@*/
        return -EIO;
    }

    return (int)n;
}

int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    picoRTOS_assert(n <= (size_t)CAN_DATA_COUNT, return -EINVAL);

    /* retrieve message from FIFO */
    size_t dlc;
    uint32_t rxf0s = ctx->base->RXF0S;

    if ((rxf0s & RXFnS_FnFL_M) == 0)
        return -EAGAIN;

    struct rx_buf *rx_buf = (struct rx_buf*)ctx->rx_fifo0;
    unsigned index = (rxf0s & RXFnS_FnGI(RXFnS_FnGI_M)) >> 8;

    if ((rx_buf[index].R0 & R0_XTD) == 0)   /* standard */
        *id = (can_id_t)((rx_buf[index].R0 & R0_ID(R0_ID_M)) >> 18);
    else                                    /* extended */
        *id = (can_id_t)(rx_buf[index].R0 & R0_ID(R0_ID_M));

    dlc = (size_t)(rx_buf[index].R1 & R1_DLC(R1_DLC_M)) >> 16;
    picoRTOS_assert_void(dlc <= (size_t)8); /* double-check */

    (void)buffer_read(&rx_buf[index], buf, dlc);
    /* TODO: check for errors */

    /* ack */
    ctx->base->RXF0A = (uint32_t)RXFnA_FnAI(index);
    return (int)dlc;
}

int can_request_frame(/*@unused@*/ struct can *ctx __attribute__((unused)),
                      /*@unused@*/ can_id_t id __attribute__((unused)))
{
    return -ENOSYS;
}

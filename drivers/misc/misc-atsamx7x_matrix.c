#include "misc-atsamx7x_matrix.h"
#include "picoRTOS.h"

struct ATSAMX7X_MATRIX {
    volatile uint32_t MATRIX_MCFGx[13];
    uint32_t RESERVED0[3];
    volatile uint32_t MATRIX_SCFGx[9];
    uint32_t RESERVED1[7];
    struct {
        volatile uint32_t PRAS;
        volatile uint32_t PRBS;
    } MATRIX_PRABSx[9];
    uint32_t RESERVED2[14];
    volatile uint32_t MATRIX_MRCR;
    uint32_t RESERVED3[3];
    volatile uint32_t CCFG_CAN0;
    volatile uint32_t CCFG_SYSIO;
    volatile uint32_t CCFG_PCCR;
    volatile uint32_t CCFG_DYNCFG;
    uint32_t RESERVED4[1];
    volatile uint32_t CCFG_SMCNFCS;
    uint32_t RESERVED5[47];
    volatile uint32_t MATRIX_WPMR;
    volatile uint32_t MATRIX_WPSR;
};

#define MATRIX_MFCGx_ULBT_M  0x7u
#define MATRIX_MFCGx_ULBT(x) ((x) & MATRIX_MFCGx_ULBT_M)

#define MATRIX_SCFGx_FIXED_DEFMSTR_M  0xfu
#define MATRIX_SCFGx_FIXED_DEFMSTR(x) (((x) & MATRIX_SCFGx_FIXED_DEFMSTR_M) << 18)
#define MATRIX_SCFGx_DEFMSTR_TYPE_M   0x3u
#define MATRIX_SCFGx_DEFMSTR_TYPE(x)  (((x) & MATRIX_SCFGx_DEFMSTR_TYPE_M) << 16)
#define MATRIX_SCFGx_SLOT_CYCLE_M     0x1ffu
#define MATRIX_SCFGx_SLOT_CYCLE(x)    (((x) & MATRIX_SCFGx_SLOT_CYCLE_M) << 1)

#define CCFG_CAN0_CAN0DMABA_M  0xffffu
#define CCFG_CAN0_CAN0DMABA(x) (((x) & CCFG_CAN0_CAN0DMABA_M) << 16)

#define CCFG_SYSIO_CAN1DMABA_M  0xffffu
#define CCFG_SYSIO_CAN1DMABA(x) (((x) & CCFG_SYSIO_CAN1DMABA_M) << 16)
#define CCFG_SYSIO_SYSIO12      (1 << 12)
#define CCFG_SYSIO_SYSIO7       (1 << 7)
#define CCFG_SYSIO_SYSIO6       (1 << 6)
#define CCFG_SYSIO_SYSIO5       (1 << 5)
#define CCFG_SYSIO_SYSIO4       (1 << 4)

#define CCFG_PCCR_I2SC1CC (1 << 22)
#define CCFG_PCCR_I2SC0CC (1 << 21)
#define CCFG_PCCR_TC0CC   (1 << 20)

#define CCFG_DYNCKG_EFCCKG  (1 << 2)
#define CCFG_DYNCKG_BRIDCKG (1 << 1)
#define CCFG_DYNCKG_MATCKG  (1 << 0)

#define CCFG_SMCNFCS_SMC_NFCS3 (1 << 3)
#define CCFG_SMCNFCS_SMC_NFCS2 (1 << 2)
#define CCFG_SMCNFCS_SMC_NFCS1 (1 << 1)
#define CCFG_SMCNFCS_SMC_NFCS0 (1 << 0)

#define MATRIX_WPMR_WPKEY_M  0xffffffu
#define MATRIX_WPMR_WPKEY(x) (((x) & MATRIX_WPMR_WPKEY_M) << 8)
#define MATRIX_WPMR_WPEN     (1 << 0)

#define MATRIX_WPSR_WPVSRC_M  0xffffu
#define MATRIX_WPSR_WPVSRC(x) (((x) & MATRIX_WPSR_WPVSRC_M) << 8)
#define MATRIX_WPSR_WPVS      (1 << 0)

int atsamx7x_matrix_init(struct atsamx7x_matrix *ctx, int base)
{
    ctx->base = (struct ATSAMX7X_MATRIX*)base;
    return 0;
}

int atsamx7x_matrix_set_mcfg(struct atsamx7x_matrix *ctx, size_t index,
                             atsamx7x_matrix_ulbt_t ulbt)
{
    picoRTOS_assert(index < (size_t)12, return -EINVAL);
    picoRTOS_assert(ulbt < ATSAMX7X_MATRIX_ULBT_COUNT, return -EINVAL);

    ctx->base->MATRIX_MCFGx[index] = (uint32_t)MATRIX_MFCGx_ULBT(ulbt);
    return 0;
}

int atsamx7x_matrix_set_scfg(struct atsamx7x_matrix *ctx, size_t index,
                             const struct atsamx7x_matrix_scfg *settings)
{
    picoRTOS_assert(index < (size_t)8, return -EINVAL);
    picoRTOS_assert(settings->fixed_defmstr <= (size_t)MATRIX_SCFGx_FIXED_DEFMSTR_M, return -EINVAL);
    picoRTOS_assert(settings->defmstr_type < ATSAMX7X_MATRIX_DEFMSTR_COUNT, return -EINVAL);
    picoRTOS_assert(settings->slot_cycle <= (size_t)MATRIX_SCFGx_SLOT_CYCLE_M, return -EINVAL);

    ctx->base->MATRIX_SCFGx[index] = (uint32_t)(MATRIX_SCFGx_FIXED_DEFMSTR(settings->fixed_defmstr) |
                                                MATRIX_SCFGx_DEFMSTR_TYPE(settings->defmstr_type) |
                                                MATRIX_SCFGx_SLOT_CYCLE(settings->slot_cycle));

    return 0;
}

int atsamx7x_matrix_set_can0dmaba(struct atsamx7x_matrix *ctx, uint16_t addr_msb)
{
    ctx->base->CCFG_CAN0 &= ~CCFG_CAN0_CAN0DMABA(CCFG_CAN0_CAN0DMABA_M);
    ctx->base->CCFG_CAN0 |= CCFG_CAN0_CAN0DMABA(addr_msb);
    return 0;
}

int atsamx7x_matrix_set_can1dmaba(struct atsamx7x_matrix *ctx, uint16_t addr_msb)
{
    ctx->base->CCFG_SYSIO &= ~CCFG_SYSIO_CAN1DMABA(CCFG_SYSIO_CAN1DMABA_M);
    ctx->base->CCFG_SYSIO |= CCFG_SYSIO_CAN1DMABA(addr_msb);
    return 0;
}

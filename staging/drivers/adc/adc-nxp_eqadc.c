#include "adc-nxp_eqadc.h"
#include "picoRTOS.h"
#include "picoRTOS_port.h"

struct ADC_NXP_EQADC {
    volatile uint32_t MCR;
    volatile uint32_t TST;
    uint32_t RESERVED0;
    volatile uint32_t ETDFR;
    volatile uint32_t CFPR[6];
    uint32_t RESERVED1[2];
    volatile uint32_t RFPR[6];
    uint32_t RESERVED2[2];
    volatile uint32_t CFCR0;
    volatile uint32_t CFCR1;
    volatile uint32_t CFCR2;
    uint32_t RESERVED_3;
    volatile uint32_t IDCR0;
    volatile uint32_t IDCR1;
    volatile uint32_t IDCR2;
    uint32_t RESERVED4;
    volatile uint32_t FISR[6];
    uint32_t RESERVED5[2];
    volatile uint32_t CFTCR0;
    volatile uint32_t CFTCR1;
    volatile uint32_t CFTCR2;
    uint32_t RESERVED6;
    volatile uint32_t CFSSR0;
    volatile uint32_t CFSSR1;
    uint32_t RESERVED7;
    volatile uint32_t CFSR;
    uint32_t RESERVED8[4];
    volatile uint32_t STACCCR;
    uint32_t RESERVED9[11];
    volatile uint32_t CF0R[4];
    volatile uint32_t CF0ER[4];
    uint32_t RESERVED10[8];
    volatile uint32_t CF1R[4];
    uint32_t RESERVED11[12];
    volatile uint32_t CF2R[4];
    uint32_t RESERVED12[12];
    volatile uint32_t CF3R[4];
    uint32_t RESERVED13[12];
    volatile uint32_t CF4R[4];
    uint32_t RESERVED14[12];
    volatile uint32_t CF5R[4];
    uint32_t RESERVED15[44];
    volatile uint32_t RF0R[4];
    uint32_t RESERVED16[12];
    volatile uint32_t RF1R[4];
    uint32_t RESERVED17[12];
    volatile uint32_t RF2R[4];
    uint32_t RESERVED18[12];
    volatile uint32_t RF3R[4];
    uint32_t RESERVED19[12];
    volatile uint32_t RF4R[4];
    uint32_t RESERVED20[12];
    volatile uint32_t RF5R[4];
};

#define CFCR0_STRME0   (1 << 27)
#define CFCR0_SSE0     (1 << 26)
#define CFCR0_MODE0_M  0xfu
#define CFCR0_MODE0(x) (((x) & CFCR0_MODE0_M) << 20)

#define IDCR0_CFFE0 (1 << 25)
#define IDCR0_CFFS0 (1 << 24)
#define IDCR0_RFDE0 (1 << 17)
#define IDCR0_RFDS0 (1 << 16)

#define ADC_NXP_EQADC_CFIFO_EOQ   (1u << 31)
#define ADC_NXP_EQADC_CFIFO_PAUSE (1 << 30)
#define ADC_NXP_EQADC_CFIFO_REP   (1 << 29)
#define ADC_NXP_EQADC_CFIFO_EB    (1 << 26) /* 0b0 */

#define ADC_NXP_EQADC_CC_BN                (1 << 25)
#define ADC_NXP_EQADC_CC_CAL               (1 << 24)
#define ADC_NXP_EQADC_CC_MESSAGE_TAG_M     0xfu
#define ADC_NXP_EQADC_CC_MESSAGE_TAG(x)    (((x) & ADC_NXP_EQADC_CC_MESSAGE_TAG_M) << 20)
#define ADC_NXP_EQADC_CC_LST_M             0x3u
#define ADC_NXP_EQADC_CC_LST(x)            (((x) & ADC_NXP_EQADC_CC_LST_M) << 18)
#define ADC_NXP_EQADC_CC_TSR               (1 << 17)
#define ADC_NXP_EQADC_CC_FMT               (1 << 16)
#define ADC_NXP_EQADC_CC_CHANNEL_NUMBER_M  0xffu
#define ADC_NXP_EQADC_CC_CHANNEL_NUMBER(x) (((x) & ADC_NXP_EQADC_CC_CHANNEL_NUMBER_M) << 8)

#define ADC_NXP_EQADC_WCC_BN                 (1 << 25)
#define ADC_NXP_EQADC_WCC_RW                 (1 << 24) /* 0b0 */
#define ADC_NXP_EQADC_WCC_ADC_REG_HI_BYTE_M  0xffu
#define ADC_NXP_EQADC_WCC_ADC_REG_HI_BYTE(x) (((x) & ADC_NXP_EQADC_WCC_ADC_REG_HI_BYTE_M) << 16)
#define ADC_NXP_EQADC_WCC_ADC_REG_LO_BYTE_M  0xffu
#define ADC_NXP_EQADC_WCC_ADC_REG_LO_BYTE(x) (((x) & ADC_NXP_EQADC_WCC_ADC_REG_LO_BYTE_M) << 8)
#define ADC_NXP_EQADC_WCC_ADC_REG_ADDRESS_M  0xffu
#define ADC_NXP_EQADC_WCC_ADC_REG_ADDRESS(x) ((x) & ADC_NXP_EQADC_WCC_ADC_REG_ADDRESS_M)

#define ADC_NXP_EQADC_RCC_BN                 (1 << 25)
#define ADC_NXP_EQADC_RCC_RW                 (1 << 24) /* 0b1 */
#define ADC_NXP_EQADC_RCC_MESSAGE_TAG_M      0xfu
#define ADC_NXP_EQADC_RCC_MESSAGE_TAG(x)     (((x) & ADC_NXP_EQADC_RCC_MESSAGE_TAG_M) << 20)
#define ADC_NXP_EQADC_RCC_ADC_REG_ADDRESS_M  0xffu
#define ADC_NXP_EQADC_RCC_ADC_REG_ADDRESS(x) ((x) & ADC_NXP_EQADC_RCC_ADC_REG_ADDRESS_M)

#define RESULT_EMPTY (1u << 31)

int adc_nxp_eqadc_init(struct adc_nxp_eqadc *ctx, int base)
{
    int i;
    uint32_t cmd;

    ctx->base = (struct ADC_NXP_EQADC*)base;
    ctx->channel_count = 0;
    ctx->fill = NULL;
    ctx->drain = NULL;

    /* mark results as empty */
    for (i = 0; i < ADC_NXP_EQADC_CHANNEL_COUNT; i++)
        ctx->result[i] = (uint32_t)RESULT_EMPTY;

    /* ADC_0 */
    cmd = (uint32_t)ADC_NXP_EQADC_WCC_ADC_REG_ADDRESS(0x1);
    cmd |= ADC_NXP_EQADC_WCC_ADC_REG_LO_BYTE(1 << 5);   /* clock: system */
    cmd |= ADC_NXP_EQADC_WCC_ADC_REG_HI_BYTE(1 << 7);   /* enable */
    ctx->base->CFPR[0] = cmd;
    /* ADC_1 */
    cmd |= ADC_NXP_EQADC_WCC_BN;
    cmd |= ADC_NXP_EQADC_CFIFO_EOQ;
    ctx->base->CFPR[0] = cmd;

    /* dma auto-trigger */
    ctx->base->IDCR0 = (uint32_t)(IDCR0_CFFE0 | IDCR0_CFFS0 |
                                  IDCR0_RFDE0 | IDCR0_RFDS0);

    return 0;
}

static int adc_nxp_eqadc_register_cc(struct adc_nxp_eqadc *ctx, size_t channel)
{
#define BN_FROM_CHANNEL(x) ((((x) & 0x1) != 0) ? ADC_NXP_EQADC_CC_BN : 0)

    picoRTOS_assert(ctx->channel_count < (size_t)ADC_NXP_EQADC_CHANNEL_COUNT,
                    return -ENOMEM);

    uint32_t cmd = (uint32_t)ADC_NXP_EQADC_CFIFO_EOQ;

    cmd |= BN_FROM_CHANNEL(ctx->channel_count);
    cmd |= ADC_NXP_EQADC_CC_MESSAGE_TAG(0);
    cmd |= ADC_NXP_EQADC_CC_CHANNEL_NUMBER(channel);

    /* append entry */
    if (ctx->channel_count > 0)
        ctx->cmd[ctx->channel_count - 1] &= ~ADC_NXP_EQADC_CFIFO_EOQ;

    ctx->cmd[ctx->channel_count] = cmd;
    arch_flush_dcache(ctx->cmd, sizeof(ctx->cmd));

    /* dma fill */
    ctx->fill_xfer.saddr = (intptr_t)ctx->cmd;
    ctx->fill_xfer.size = sizeof(uint32_t);
    ctx->fill_xfer.incr_read = DMA_XFER_INCREMENT_ON;
    ctx->fill_xfer.byte_count = (size_t)(ctx->channel_count + 1) * sizeof(uint32_t);
    ctx->fill_xfer.daddr = (intptr_t)&ctx->base->CFPR[0];
    ctx->fill_xfer.incr_write = DMA_XFER_INCREMENT_OFF;

    /* dma drain */
    ctx->drain_xfer.saddr = (intptr_t)&ctx->base->RFPR[0];
    ctx->drain_xfer.size = sizeof(uint32_t);
    ctx->fill_xfer.incr_read = DMA_XFER_INCREMENT_OFF;
    ctx->drain_xfer.byte_count = (size_t)(ctx->channel_count + 1) * sizeof(uint32_t);
    ctx->drain_xfer.daddr = (intptr_t)ctx->result;
    ctx->fill_xfer.incr_write = DMA_XFER_INCREMENT_ON;

    return (int)ctx->channel_count++;
}

int adc_nxp_eqadc_setup(struct adc_nxp_eqadc *ctx, const struct adc_nxp_eqadc_settings *settings)
{
    picoRTOS_assert(settings->mode0 < ADC_NXP_EQADC_MODE0_COUNT, return -EINVAL);

    ctx->fill = settings->fill;
    ctx->drain = settings->drain;

    switch (settings->mode0) {
    case ADC_NXP_EQADC_MODE0_SINGLE_SCAN:
        ctx->base->CFCR0 = (uint32_t)(CFCR0_STRME0 | CFCR0_MODE0(1));
        break;

    case ADC_NXP_EQADC_MODE0_CONTINUOUS_SCAN:
        ctx->base->CFCR0 = (uint32_t)(CFCR0_STRME0 | CFCR0_MODE0(9));
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    ctx->mode0 = settings->mode0;
    return 0;
}

int adc_nxp_eqadc_adc_init(struct adc *ctx, struct adc_nxp_eqadc *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)ADC_NXP_EQADC_CHANNEL_COUNT, return -EINVAL);
    picoRTOS_assert(parent->fill != NULL, return -EIO);
    picoRTOS_assert(parent->drain != NULL, return -EIO);

    int res;

    ctx->parent = parent;
    ctx->channel = channel;

    if ((res = adc_nxp_eqadc_register_cc(parent, channel)) < 0)
        return res;

    ctx->channel_index = (size_t)res;
    /* calibration */
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

    /* start */
    if ((res = dma_setup(parent->fill, &parent->fill_xfer)) < 0 ||
        (res = dma_setup(parent->drain, &parent->drain_xfer)) < 0)
        return res;

    return 0;
}

static void channel_set_cal(struct adc *ctx, bool cal)
{
    struct adc_nxp_eqadc *parent = ctx->parent;

    if (cal) parent->cmd[ctx->channel_index] |= ADC_NXP_EQADC_CC_CAL;
    else parent->cmd[ctx->channel_index] &= ~ADC_NXP_EQADC_CC_CAL;
}

static int channel_set_lst(struct adc *ctx, adc_nxp_eqadc_lst_t lst)
{
    picoRTOS_assert(lst < ADC_NXP_EQADC_LST_COUNT, return -EINVAL);

    struct adc_nxp_eqadc *parent = ctx->parent;

    parent->cmd[ctx->channel_index] &= ~ADC_NXP_EQADC_CC_LST(ADC_NXP_EQADC_CC_LST_M);
    parent->cmd[ctx->channel_index] |= ADC_NXP_EQADC_CC_LST(lst);

    return 0;
}

int adc_nxp_eqadc_adc_setup(struct adc *ctx, const struct adc_nxp_eqadc_adc_settings *settings)
{
    channel_set_cal(ctx, settings->cal);
    return channel_set_lst(ctx, settings->lst);
}

int adc_setup(struct adc *ctx, const struct adc_settings *settings)
{
    ctx->multiplier = settings->multiplier;
    ctx->divider = settings->divider;
    ctx->offset = settings->offset;

    return 0;
}

int adc_read(struct adc *ctx, int *data)
{
    int value;
    struct adc_nxp_eqadc *parent = ctx->parent;
    uint32_t *result = &parent->result[ctx->channel_index];

    /* get channel corresponding value */
    picoRTOS_invalidate_dcache(result, sizeof(*result));
    value = (int)*result;

    if ((value & RESULT_EMPTY) != 0) {
        /* software trigger (single scan mode) */
        if (parent->mode0 == ADC_NXP_EQADC_MODE0_SINGLE_SCAN)
            parent->base->CFCR0 |= CFCR0_SSE0;

        return -EAGAIN;
    }

    *data = (value * ctx->multiplier) / ctx->divider + ctx->offset;
    *result |= RESULT_EMPTY; /* mark as read */

    return 1;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    return adc_read(ctx, data);
}

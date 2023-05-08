#include "adc-gd32vf103.h"
#include "picoRTOS.h"

#include <stdint.h>

struct ADC_GD32VF103 {
    volatile uint32_t ADC_STAT;
    volatile uint32_t ADC_CTL0;
    volatile uint32_t ADC_CTL1;
    volatile uint32_t ADC_SAMPT0;
    volatile uint32_t ADC_SAMPT1;
    uint32_t RESERVED0[4];
    volatile uint32_t ADC_WDHT;
    volatile uint32_t ADC_WDLT;
    volatile uint32_t ADC_RSQ0;
    volatile uint32_t ADC_RSQ1;
    volatile uint32_t ADC_RSQ2;
    uint32_t RESERVED1[5];
    volatile uint32_t ADC_RDATA;
    uint32_t RESERVED2[12];
    volatile uint32_t ADC_OVSAMPCTL;
};

#define ADC_STAT_STRC (1 << 4)
#define ADC_STAT_EOC  (1 << 1)
#define ADC_STAT_WDE  (1 << 0)

#define ADC_CTL0_RWDEN      (1 << 23)
#define ADC_CTL0_SYNCM_M    0xfu
#define ADC_CTL0_SYNCM(x)   (((x) & ADC_CTL0_SYNCM_M) << 16)
#define ADC_CTL0_DISNUM_M   0x7u
#define ADC_CTL0_DISNUM(x)  (((x) & ADC_CTL0_DISNUM_M) << 13)
#define ADC_CTL0_DISRC      (1 << 11)
#define ADC_CTL0_WDSC       (1 << 9)
#define ADC_CTL0_SM         (1 << 8)
#define ADC_CTL0_WDEIE      (1 << 6)
#define ADC_CTL0_EOCIE      (1 << 5)
#define ADC_CTL0_WDCHSEL_M  0x1fu
#define ADC_CTL0_WDCHSEL(x) ((x) & ADC_CTL0_WDCHSEL_M)

#define ADC_CTL1_TSVREN   (1 << 23)
#define ADC_CTL1_SWRCST   (1 << 22)
#define ADC_CTL1_ETERC    (1 << 20)
#define ADC_CTL1_ETSRC_M  0x7u
#define ADC_CTL1_ETSRC(x) (((x) & ADC_CTL1_ETSRC_M) << 17)
#define ADC_CTL1_DAL      (1 << 11)
#define ADC_CTL1_DMA      (1 << 8)
#define ADC_CTL1_RSTCLB   (1 << 3)
#define ADC_CTL1_CLB      (1 << 2)
#define ADC_CTL1_CTN      (1 << 1)
#define ADC_CTL1_ADCON    (1 << 0)

#define ADC_RSQ0_RL_M  0xfu
#define ADC_RSQ0_RL(x) (((x) & ADC_RSQ0_RL_M) << 20)

#define ADC_RSQ2_RSQ0_M  0x1fu
#define ADC_RSQ2_RSQ0(x) ((x) & ADC_RSQ2_RSQ0_M)

int adc_gd32vf103_init(struct adc_gd32vf103 *ctx, int base)
{
    int delay = CONFIG_DEADLOCK_COUNT; /* FIXME */
    int deadlock = CONFIG_DEADLOCK_COUNT;

    ctx->base = (struct ADC_GD32VF103*)base;

    /* turn on */
    ctx->base->ADC_CTL1 |= ADC_CTL1_ADCON;
    /* delay 14 CK_ADC (FIXME) */
    while (delay-- != 0) {
    }

    /* calibration procedure */
    ctx->base->ADC_CTL1 |= (ADC_CTL1_RSTCLB | ADC_CTL1_CLB);

    while (deadlock-- != 0)
        if ((ctx->base->ADC_CTL1 & ADC_CTL1_CLB) == 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    /* modes */
    ctx->mode = ADC_GD32VF103_MODE_SINGLE;
    ctx->rsq_count = 0;
    ctx->dma = NULL;

    return 0;
}

static int setup_dma(struct adc_gd32vf103 *ctx, /*@null@*/ struct dma *dma)
{
    if (!picoRTOS_assert(dma != NULL)) return -EINVAL;

    ctx->base->ADC_CTL1 |= ADC_CTL1_DMA;

    ctx->xfer.saddr = (intptr_t)&ctx->base->ADC_RDATA;
    ctx->xfer.daddr = (intptr_t)ctx->result;
    ctx->xfer.incr_read = false;
    ctx->xfer.incr_write = true;
    ctx->xfer.size = sizeof(uint32_t);

    ctx->dma = dma;
    return 0;
}

int adc_gd32vf103_setup(struct adc_gd32vf103 *ctx, struct adc_gd32vf103_settings *settings)
{
    if (!picoRTOS_assert(settings->mode < ADC_GD32VF103_MODE_COUNT)) return -EINVAL;

    int res = 0;

    switch (settings->mode) {
    case ADC_GD32VF103_MODE_SINGLE:
        ctx->base->ADC_CTL0 &= ~ADC_CTL0_SM;
        ctx->base->ADC_CTL1 &= ~ADC_CTL1_CTN;
        break;

    case ADC_GD32VF103_MODE_CONTINUOUS:
        ctx->base->ADC_CTL0 &= ~ADC_CTL0_SM;
        ctx->base->ADC_CTL1 |= ADC_CTL1_CTN;
        break;

    case ADC_GD32VF103_MODE_SCAN_SINGLE:
        ctx->base->ADC_CTL0 |= ADC_CTL0_SM;
        ctx->base->ADC_CTL1 &= ~ADC_CTL1_CTN;
        res = setup_dma(ctx, settings->dma);
        break;

    case ADC_GD32VF103_MODE_SCAN_CONTINUOUS:
        ctx->base->ADC_CTL0 |= ADC_CTL0_SM;
        ctx->base->ADC_CTL1 |= ADC_CTL1_CTN;
        res = setup_dma(ctx, settings->dma);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    if (res < 0)
        return res;

    if (settings->tvsen)
        ctx->base->ADC_CTL1 |= ADC_CTL1_TSVREN;

    ctx->mode = settings->mode;
    return 0;
}

int adc_gd32vf103_start(struct adc_gd32vf103 *ctx)
{
    return dma_setup(ctx->dma, &ctx->xfer);
}

static int set_rsq(struct adc_gd32vf103 *ctx, size_t channel, size_t rsq)
{
    if (!picoRTOS_assert(channel < (size_t)ADC_GD32VF103_CHANNEL_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(rsq < (size_t)ADC_GD32VF103_RSQ_COUNT)) return -EINVAL;

    size_t shift;

    if (rsq < (size_t)6) {
        shift = rsq * (size_t)5;
        ctx->base->ADC_RSQ2 &= ~(ADC_RSQ2_RSQ0(ADC_RSQ2_RSQ0_M) << shift);
        ctx->base->ADC_RSQ2 |= ADC_RSQ2_RSQ0(channel) << shift;
        return 0;
    }

    if (rsq < (size_t)12) {
        shift = (rsq - (size_t)6) * (size_t)5;
        ctx->base->ADC_RSQ1 &= ~(ADC_RSQ2_RSQ0(ADC_RSQ2_RSQ0_M) << shift);
        ctx->base->ADC_RSQ1 |= ADC_RSQ2_RSQ0(channel) << shift;
        return 0;
    }

    /* rsq0 */
    shift = (rsq - (size_t)12) * (size_t)5;
    ctx->base->ADC_RSQ0 &= ~(ADC_RSQ2_RSQ0(ADC_RSQ2_RSQ0_M) << shift);
    ctx->base->ADC_RSQ0 |= ADC_RSQ2_RSQ0(channel) << shift;

    return 0;
}

static int register_channel(struct adc_gd32vf103 *ctx, size_t channel)
{
    if (!picoRTOS_assert(channel < (size_t)ADC_GD32VF103_CHANNEL_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(ctx->rsq_count < (size_t)ADC_GD32VF103_RSQ_COUNT)) return -ENOMEM;

    int res;

    /* set RSQx[4:0] */
    if ((res = set_rsq(ctx, channel, ctx->rsq_count)) < 0)
        return res;

    /* set RL[3:0] */
    ctx->base->ADC_RSQ0 &= ~ADC_RSQ0_RL(ADC_RSQ0_RL_M);
    ctx->base->ADC_RSQ0 |= ADC_RSQ0_RL(ctx->rsq_count + 1);
    /* dma */
    ctx->xfer.byte_count = sizeof(uint32_t) * (ctx->rsq_count + 1);

    return (int)ctx->rsq_count++;
}

/* channels */

int adc_gd32vf103_adc_init(struct adc *ctx, struct adc_gd32vf103 *parent, size_t channel)
{
    if (!picoRTOS_assert(channel < (size_t)ADC_GD32VF103_CHANNEL_COUNT)) return -EINVAL;

    ctx->parent = parent;
    ctx->channel = channel;
    /* calibration */
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;
    /* scan mode */
    ctx->rsq_index = 0;

    if (ctx->parent->mode == ADC_GD32VF103_MODE_SCAN_SINGLE ||
        ctx->parent->mode == ADC_GD32VF103_MODE_SCAN_CONTINUOUS) {
        /* register channel */
        int res;
        if ((res = register_channel(ctx->parent, ctx->channel)) < 0)
            return res;

        ctx->rsq_index = (size_t)res;
    }

    return 0;
}

int adc_setup(struct adc *ctx, struct adc_settings *settings)
{
    ctx->multiplier = settings->multiplier;
    ctx->divider = settings->divider;
    ctx->offset = settings->offset;

    return 0;
}

static int adc_read_single(struct adc *ctx, int *data)
{
    int rdata;
    struct adc_gd32vf103 *parent = ctx->parent;

    if ((parent->base->ADC_STAT & ADC_STAT_EOC) == 0) {
        /* TODO: check if there's an external trigger */
        /* start conversion if needed */
        if ((parent->base->ADC_STAT & ADC_STAT_STRC) == 0) {
            /* set RSQ0[4:0] */
            parent->base->ADC_RSQ2 &= ~ADC_RSQ2_RSQ0(ADC_RSQ2_RSQ0_M);
            parent->base->ADC_RSQ2 |= ADC_RSQ2_RSQ0(ctx->channel);
            /* sw trigger */
            parent->base->ADC_CTL1 |= ADC_CTL1_SWRCST;
        }

        return -EAGAIN;
    }

    rdata = 0xfff & parent->base->ADC_RDATA;
    *data = ((rdata * ctx->multiplier) / ctx->divider) + ctx->offset;

    return 1;
}

static int adc_read_scan(struct adc *ctx, int *data)
{
    int rdata;
    struct adc_gd32vf103 *parent = ctx->parent;

    rdata = 0xfff & parent->result[ctx->rsq_index];
    *data = ((rdata * ctx->multiplier) / ctx->divider) + ctx->offset;

    return 1;
}

int adc_read(struct adc *ctx, int *data)
{
    if (ctx->parent->mode == ADC_GD32VF103_MODE_SINGLE ||
        ctx->parent->mode == ADC_GD32VF103_MODE_CONTINUOUS)
        /* single & continuous */
        return adc_read_single(ctx, data);

    if (ctx->parent->mode == ADC_GD32VF103_MODE_SCAN_SINGLE ||
        ctx->parent->mode == ADC_GD32VF103_MODE_SCAN_CONTINUOUS)
        return adc_read_scan(ctx, data);

    /* unsupported mode */
    picoRTOS_break();
    /*@notreached@*/ return -EFAULT;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* FIXME */
    return adc_read(ctx, data);
}

#include "adc-sam3x.h"
#include "picoRTOS.h"

#include <stdint.h>

struct ADC_SAM3X {
    volatile uint32_t ADC_CR;
    volatile uint32_t ADC_MR;
    volatile uint32_t ADC_SEQR1;
    volatile uint32_t ADC_SEQR2;
    volatile uint32_t ADC_CHER;
    volatile uint32_t ADC_CHDR;
    volatile uint32_t ADC_CHSR;
    uint32_t RESERVED0;
    volatile uint32_t ADC_LCDR;
    volatile uint32_t ADC_IER;
    volatile uint32_t ADC_IDR;
    volatile uint32_t ADC_IMR;
    volatile uint32_t ADC_ISR;
    uint32_t RESERVED1[2];
    volatile uint32_t ADC_OVER;
    volatile uint32_t ADC_EMR;
    volatile uint32_t ADC_CWR;
    volatile uint32_t ADC_CGR;
    volatile uint32_t ADC_COR;
    volatile uint32_t ADC_CDR[ADC_SAM3X_CHANNEL_COUNT];
    uint32_t RESERVED2;
    volatile uint32_t ADC_ACR;
    uint32_t RESERVED3[19];
    volatile uint32_t ADC_WPMR;
    volatile uint32_t ADC_WPSR;
};

#define ADC_CR_START (1 << 1)
#define ADC_CR_SWRST (1 << 0)

#define ADC_MR_USEQ        (1 << 31)
#define ADC_MR_TRANSFER_M  0x3u
#define ADC_MR_TRANSFER(x) (((x) & ADC_MR_TRANSFER_M) << 28)
#define ADC_MR_TRACKTIM_M  0xfu
#define ADC_MR_TRACKTIM(x) (((x) & ADC_MR_TRACKTIM_M) << 24)
#define ADC_MR_ANACH       (1 << 23)
#define ADC_MR_SETTLING_M  0x3u
#define ADC_MR_SETTLING(x) (((x) & ADC_MR_SETTLING_M) << 20)
#define ADC_MR_STARTUP_M   0xfu
#define ADC_MR_STARTUP(x)  (((x) & ADC_MR_STARTUP_M) << 16)
#define ADC_MR_PRESCAL_M   0xffu
#define ADC_MR_PRESCAL(x)  (((x) & ADC_MR_PRESCAL_M) << 8)
#define ADC_MR_FREERUN     (1 << 7)
#define ADC_MR_FWUP        (1 << 6)
#define ADC_MR_SLEEP       (1 << 5)
#define ADC_MR_LOWRES      (1 << 4)
#define ADC_MR_TRGSEL_M    0x7u
#define ADC_MR_TRGSEL(x)   (((x) & ADC_MR_TRGSEL_M) << 1)
#define ADC_MR_TRGEN       (1 << 0)

/* Function: adc_sam3x_init
 * Initializes an ADC block
 *
 * Parameters:
 *  ctx - The ADC block to init
 *  base - The ADC block base address
 *
 * Returns:
 * Always 0
 */
int adc_sam3x_init(struct adc_sam3x *ctx, int base)
{
    ctx->base = (struct ADC_SAM3X*)base;

    /* enable & freerun */
    ctx->base->ADC_CR |= ADC_CR_START;
    ctx->base->ADC_MR |= ADC_MR_FREERUN;

    return 0;
}

/* Function: adc_sam3x_adc_init
 * Initializes an ADC channel
 *
 * Parameters:
 *  ctx - The Adc channel to init
 *  parent - The parent ADC block
 *  channel - The selected channel
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int adc_sam3x_adc_init(struct adc *ctx, struct adc_sam3x *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)ADC_SAM3X_CHANNEL_COUNT,
                    return -EINVAL);

    ctx->parent = parent;
    ctx->channel = channel;
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

    /* enable channel */
    parent->base->ADC_CHER |= (1 << channel);

    return 0;
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
    struct adc_sam3x *parent = ctx->parent;

    uint32_t cdr = parent->base->ADC_CDR[ctx->channel];

    /* apply calibration */
    *data = ((int)cdr * ctx->multiplier) / ctx->divider + ctx->offset;
    return 1;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    return adc_read(ctx, data);
}

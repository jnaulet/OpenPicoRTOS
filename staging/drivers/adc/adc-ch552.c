#include "adc-ch552.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_ADC_CTRL) ADC_CTRL;
__sfr __at(ADDR_ADC_CFG) ADC_CFG;
__sfr __at(ADDR_ADC_DATA) ADC_DATA;
#else
static unsigned char ADC_CTRL;
static unsigned char ADC_CFG;
static unsigned char ADC_DATA;
#endif

#define ADC_CTRL_CMPO        (1 << 7)
#define ADC_CTRL_CMP_IF      (1 << 6)
#define ADC_CTRL_ADC_IF      (1 << 5)
#define ADC_CTRL_ADC_START   (1 << 4)
#define ADC_CTRL_CMP_CHAN    (1 << 3)
#define ADC_CTRL_ADC_CHAN_M  0x3u
#define ADC_CTRL_ADC_CHAN(x) ((x) & ADC_CTRL_ADC_CHAN_M)

#define ADC_CFG_ADC_EN  (1 << 3)
#define ADC_CFG_CMP_EN  (1 << 2)
#define ADC_CFG_ADC_CLK (1 << 0)

int adc_ch522_init(struct adc_ch552 *ctx)
{
    /*@i@*/ (void)ctx;
    ADC_CFG = (unsigned char)ADC_CFG_ADC_EN;
    return 0;
}

int adc_ch552_init(struct adc *ctx, size_t channel)
{
    picoRTOS_assert(channel < (size_t)ADC_CH552_CHANNEL_COUNT, return -EINVAL);

    ctx->channel = (unsigned char)channel;

    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

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
    if ((ADC_CTRL & ADC_CTRL_ADC_IF) != (unsigned char)0) {
        /* this is not the conversion you are looking for */
        if ((ADC_CTRL & ADC_CTRL_ADC_CHAN_M) != ctx->channel)
            return -EAGAIN;
        /* result & clear */
        unsigned adc_data = (unsigned)ADC_DATA;
        ADC_CTRL &= ~ADC_CTRL_ADC_IF;
        /* apply calibration */
        *data = (((int)adc_data * ctx->multiplier) /
                 ctx->divider) + ctx->offset;
        /* return */
        return 1;
    }else{
        ADC_CTRL &= ~ADC_CTRL_ADC_CHAN(ADC_CTRL_ADC_CHAN_M);
        ADC_CTRL |= ADC_CTRL_ADC_CHAN(ctx->channel);
        ADC_CTRL |= ADC_CTRL_ADC_START;
    }

    return -EAGAIN;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    return adc_read(ctx, data);
}

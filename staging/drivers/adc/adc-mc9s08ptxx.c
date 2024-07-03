#include "adc-mc9s08ptxx.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct ADC_MC9S08PTXX {
    volatile uint8_t SC1;
    volatile uint8_t SC2;
    volatile uint8_t SC3;
    volatile uint8_t SC4;
    volatile uint8_t RH;
    volatile uint8_t RL;
    volatile uint8_t CVH;
    volatile uint8_t CVL;
};

#define APCTL1 ((volatile uint8_t*)(ADDR_ADCH))
#define APCTL2 ((volatile uint8_t*)(ADDR_ADCH + 1))

#define SC1_COCO    (1 << 7)
#define SC1_AIEN    (1 << 6)
#define SC1_ADCO    (1 << 5)
#define SC1_ADCH_M  0x1fu
#define SC1_ADCH(x) ((x) & SC1_ADCH_M)

#define SC2_ADACT  (1 << 7)
#define SC2_ADTRG  (1 << 6)
#define SC2_ACFE   (1 << 5)
#define SC2_ACFGT  (1 << 4)
#define SC2_FEMPTY (1 << 3)
#define SC2_FFULL  (1 << 2)

#define SC3_ADLPC     (1 << 7)
#define SC3_ADIV_M    0x3u
#define SC3_ADIV(x)   (((x) & SC3_ADIV_M) << 5)
#define SC3_ADLSMP    (1 << 4)
#define SC3_MODE_M    0X3u
#define SC3_MODE(x)   (((x) & SC3_MODE_M) << 2)
#define SC3_ADICLK_M  0x3u
#define SC3_ADICLK(x) ((x) & SC3_ADICLK_M)

#define SC4_ASCANE   (1 << 6)
#define SC4_ACFSEL   (1 << 5)
#define SC4_AFDEP_M  0x7u
#define SC4_AFDEP(x) ((x) & SC4_AFDEP_M)

int adc_mc9s08ptxx_init(struct adc_mc9s08ptxx *ctx, int base)
{
    ctx->base = (struct ADC_MC9S08PTXX*)base;
    return 0;
}

int adc_mc9s08ptxx_setup(struct adc_mc9s08ptxx *ctx, struct adc_mc9s08ptxx_settings *settings)
{
    picoRTOS_assert(settings->mode < ADC_MC9S08PTXX_MODE_COUNT, return -EINVAL);

    ctx->base->SC3 &= ~SC3_MODE(SC3_MODE_M);
    ctx->base->SC3 |= SC3_MODE(settings->mode);

    return 0;
}

/* channels */

int adc_mc9s08ptxx_adc_init(struct adc *ctx, struct adc_mc9s08ptxx *parent, size_t channel)
{
#define ADC_MC9S08PTXX_ADC8  8
#define ADC_MC9S08PTXX_ADC15 15

    picoRTOS_assert(channel < (size_t)ADC_MC9S08PTXX_CHANNEL_COUNT, return -EINVAL);

    ctx->parent = parent;
    ctx->channel = channel;

    /* turn I/O control off */
    if (channel < (size_t)ADC_MC9S08PTXX_ADC8) {
        *APCTL1 |= (1 << channel);
        return 0;
    }

    if (channel <= (size_t)ADC_MC9S08PTXX_ADC15) {
        *APCTL2 |= (1 << (channel - (size_t)8));
        return 0;
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

int adc_read(struct adc *ctx, int *data)
{
    struct adc_mc9s08ptxx *parent = ctx->parent;

    if ((parent->base->SC2 & SC2_ADACT) != 0)
        return -EAGAIN;

    if ((parent->base->SC1 & SC1_COCO) != 0) {
        /* read data */
        uint16_t result = (((uint16_t)parent->base->RH << 8) |
                           (uint16_t)parent->base->RL);
        /* apply calibration */
        *data = ((int)result * ctx->multiplier) / ctx->divider + ctx->offset;
        return 1;
    }

    /* no conversion pending & no completion, start */
    parent->base->SC1 = (uint8_t)SC1_ADCH(ctx->channel);
    return -EAGAIN;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    return adc_read(ctx, data);
}

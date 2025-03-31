#include "adc-n76e003_sar.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_ADCCON0) ADCCON0;
__sfr __at(ADDR_ADCCON1) ADCCON1;
__sfr __at(ADDR_ADCCON2) ADCCON2;
__sfr __at(ADDR_AINDIDS) AINDIDS;
__sfr __at(ADDR_ADCDLY)  ADCDLY;
__sfr __at(ADDR_ADCRH)   ADCRH;
__sfr __at(ADDR_ADCRL)   ADCRL;
__sfr __at(ADDR_ADCMPH)  ADCMPH;
__sfr __at(ADDR_ADCMPL)  ADCMPL;
#else
static unsigned char ADCCON0;
static unsigned char ADCCON1;
/*@unused@*/ static unsigned char ADCCON2;
static unsigned char AINDIDS;
/*@unused@*/ static unsigned char ADCDLY;
static unsigned char ADCRH;
static unsigned char ADCRL;
/*@unused@*/ static unsigned char ADCMPH;
/*@unused@*/ static unsigned char ADCMPL;
#endif

#define ADCCON0_ADCF      (1 << 7)
#define ADCCON0_ADCS      (1 << 6)
#define ADCCON0_ETGSEL_M  0x3u
#define ADCCON0_ETGSEL(x) (((x) & ADCCON0_ETGSEL_M) << 4)
#define ADCCON0_ADCHS_M   0xfu
#define ADCCON0_ADCHS(x)  ((x) & ADCCON0_ADCHS_M)

#define ADCCON1_STADCPX   (1 << 6)
#define ADCCON1_ETGTYP_M  0x3u
#define ADCCON1_ETGTYP(x) (((x) & ADCCON1_ETGTYP_M) << 2)
#define ADCCON1_ADCEX     (1 << 1)
#define ADCCON1_ADCEN     (1 << 0)

#define ADCCON2_ADFBEN   (1 << 7)
#define ADCCON2_ADCMPOP  (1 << 6)
#define ADCCON2_ADMPEN   (1 << 5)
#define ADCCON2_ADCMPO   (1 << 4)
#define ADCCON2_ADCDLY_8 (1 << 0)

#define AINDIDS_P11DIDS (1 << 7)
#define AINDIDS_P03DIDS (1 << 6)
#define AINDIDS_P04DIDS (1 << 5)
#define AINDIDS_P05DIDS (1 << 4)
#define AINDIDS_P06DIDS (1 << 3)
#define AINDIDS_P07DIDS (1 << 2)
#define AINDIDS_P30DIDS (1 << 1)
#define AINDIDS_P17DIDS (1 << 0)

int adc_n76e003_init(struct adc_n76e003 *ctx)
{
    ADCCON1 |= ADCCON1_ADCEN;       /* enable SAR */
    AINDIDS = (unsigned char)0xff;  /* disable all inputs */

    ctx->dummy = 0;
    return 0;
}

int adc_n76e003_adc_init(struct adc *ctx, size_t channel)
{
    picoRTOS_assert(channel < (size_t)ADC_N76E003_SAR_CHANNEL_COUNT, return -EINVAL);

    ctx->channel = (unsigned char)channel;
    AINDIDS &= ~(1u << ctx->channel);

    /* basics */
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
    if ((ADCCON0 & ADCCON0_ADCF) != (unsigned char)0) {
        /* this is not the conversion you are looking for */
        if ((ADCCON0 & ADCCON0_ADCHS_M) != ctx->channel)
            return -EAGAIN;
        /* result & clear */
        unsigned adcr = (unsigned)ADCRH << 4 | (unsigned)ADCRL;
        ADCCON0 &= ~ADCCON0_ADCF;
        /* apply calibration */
        *data = (((int)adcr * ctx->multiplier) /
                 ctx->divider) + ctx->offset;
        /* return */
        return 1;

    }else{
        ADCCON0 &= ~ADCCON0_ADCHS(ADCCON0_ADCHS_M);
        ADCCON0 |= ADCCON0_ADCHS(ctx->channel);
        ADCCON0 |= ADCCON0_ADCS;
    }

    return -EAGAIN;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    return adc_read(ctx, data);
}

#include "adc-pic32mx.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

/* TODO:
 * put DEVICE_ limits on some of these registers for pic32mz
 * and maybe separate ADCDATAx from the struct
 */

#define ADC_PIC32MX_ADC_COUNT           8 /* in theory */
#define ADC_PIC32MX_SHARED_SAR          7
#define ADC_PIC32MX_TRGSRCABLE_COUNT    (DEVICE_ADC_DEDICATED_SAR_COUNT + \
                                         DEVICE_ADC_SHARED_CL2_SAR_COUNT)
#define ADC_PIC32MX_CHANNEL_COUNT       (DEVICE_ADC_DEDICATED_SAR_COUNT + \
                                         DEVICE_ADC_SHARED_CL2_SAR_COUNT + \
                                         DEVICE_ADC_SHARED_CL3_SAR_COUNT)

struct ADC_PIC32MX {
    volatile uint32_t ADCCON1;
    volatile uint32_t ADCCON2;
    volatile uint32_t ADCCON3;
    volatile uint32_t ADCTRGMODE;
    volatile uint32_t ADCIMCONx[4];
    volatile uint32_t ADCGIRQEN[2];
    volatile uint32_t ADCCSS[2];
    volatile uint32_t ADCDSTAT[2];
    struct {
        volatile uint32_t ADCCMPEN;
        volatile uint32_t ADCCMP;
    } ADCCMPx[ADC_PIC32MX_ADC_COUNT - 2];                       /* 1-6 */
    volatile uint32_t ADCFLTRx[ADC_PIC32MX_ADC_COUNT - 2];      /* 1-6 */
    volatile uint32_t ADCTRG[8];                                /* only 3 on pic32mz */
    volatile uint32_t ADCCMPCON1;
    volatile uint32_t ADCCMPCONx[ADC_PIC32MX_ADC_COUNT - 3];    /* 2-6 */
    volatile uint32_t ADCFSTAT;
    volatile uint32_t ADCFIFO;
    volatile uint32_t ADCBASE;
    volatile uint32_t ADCDMASTAT;                           /* n/a on pic32mz */
    volatile uint32_t ADCCNTB;                              /* n/a on pic32mz */
    volatile uint32_t ADCDMAB;                              /* n/a on pic32mz */
    volatile uint32_t ADCTRGSNS;
    volatile uint32_t ADCxTIME[ADC_PIC32MX_ADC_COUNT - 1];  /* 0-6, only 5 on pic32mz */
    volatile uint32_t ADCEIEN[2];
    volatile uint32_t ADCEISTAT[2];
    volatile uint32_t ADCANCON;
    uint32_t RESERVED0[31];
    volatile uint32_t ADCxCFG[ADC_PIC32MX_ADC_COUNT];
    uint32_t RESERVED1[8];
    volatile uint32_t ADCSYSCFG[2];
    uint32_t RESERVED2[14];
    volatile uint32_t ADCDATAx[64]; /* only 45 on pic32mz */
};

#define ADCCON1_TRBEN      (1u << 31)
#define ADCCON1_TRBERR     (1 << 30)
#define ADCCON1_TRBMST_M   0x7u
#define ADCCON1_TRBMST(x)  (((x) & ADCCON1_TRBMST_M) << 27)
#define ADCCON1_TRBSLV_M   0x7u
#define ADCCON1_TRBSLV(x)  (((x) & ADCCON1_TRBMST_M) << 24)
#define ADCCON1_FRACT      (1 << 23)
#define ADCCON1_SELRES_M   0x3u
#define ADCCON1_SELRES(x)  (((x) & ADCCON1_SELRES_M) << 21)
#define ADCCON1_STRGSRC_M  0x1fu
#define ADCCON1_STRGSRC(x) (((x) & ADCCON1_STRGSRC_M) << 16)
#define ADCCON1_ON         (1 << 15)
#define ADCCON1_SIDL       (1 << 13)
#define ADCCON1_AICPMPEN   (1 << 12)
#define ADCCON1_CVDEN      (1 << 11)
#define ADCCON1_FSSCLKEN   (1 << 10)
#define ADCCON1_FSPBCLKEN  (1 << 9)
#define ADCCON1_IRQVS_M    0x7u
#define ADCCON1_IRQVS(x)   (((x) & ADCCON1_IRQVS_M) << 4)
#define ADCCON1_STRGLVL    (1 << 3)

#define ADCCON2_BGVRRDY   (1u << 31)
#define ADCCON2_REFFLT    (1 << 30)
#define ADCCON2_EOSRDY    (1 << 29)
#define ADCCON2_CVDCPL_M  0x7u
#define ADCCON2_CVDCPL(x) (((x) & ADCCON2_CVDCPL_M) << 26)
#define ADCCON2_SAMC_M    0x7ffu
#define ADCCON2_SAMC(x)   (((x) & ADCCON2_SAMC_M) << 16)
#define ADCCON2_BGVRIEN   (1 << 15)
#define ADCCON2_REFFLTIEN (1 << 14)
#define ADCCON2_EOSIEN    (1 << 13)
#define ADCCON2_ADCEIOVR  (1 << 12)
#define ADCCON2_ADCEIS_M  0x7u
#define ADCCON2_ADCEIS(x) (((x) & ADCCON2_ADCEIS_M) << 8)
#define ADCCON2_ADCDIV_M  0x7fu
#define ADCCON2_ADCDIV(x) ((x) & ADCCON2_ADCDIV_M)

#define ADCCON3_ADCSEL_M     0x3u
#define ADCCON3_ADCSEL(x)    (((x) & ADCCON3_ADCSEL_M) << 30)
#define ADCCON3_CONCLKDIV_M  0x3fu
#define ADCCON3_CONCLKDIV(x) (((x) & ADCCON3_CONCLKDIV_M) << 24)
#define ADCCON3_DIGEN7       (1 << 23)
#define ADCCON3_DIGEN4       (1 << 20)
#define ADCCON3_DIGEN3       (1 << 19)
#define ADCCON3_DIGEN2       (1 << 18)
#define ADCCON3_DIGEN1       (1 << 17)
#define ADCCON3_DIGEN0       (1 << 16)
#define ADCCON3_DIGENx(x)    (0x1u << ((x) + (size_t)16))
#define ADCCON3_VREFSEL_M    0x7u
#define ADCCON3_VREFSEL(x)   (((x) & ADCCON3_VREFSEL_M) << 13)
#define ADCCON3_TRGSUSP      (1 << 12)
#define ADCCON3_UPDIEN       (1 << 11)
#define ADCCON3_UPDRDY       (1 << 10)
#define ADCCON3_SAMP         (1 << 9)
#define ADCCON3_RQCNVRT      (1 << 8)
#define ADCCON3_GLSWTRG      (1 << 7)
#define ADCCON3_GSWTRG       (1 << 6)
#define ADCCON3_ADINSEL_M    0x3fu
#define ADCCON3_ADINSEL(x)   ((x) & ADCCON3_ADINSEL_M)

#define ADCTRGMODE_SH4ALT_M  0x3u
#define ADCTRGMODE_SH4ALT(x) (((x) & ADCTRGMODE_SH4ALT_M) << 24)
#define ADCTRGMODE_SH3ALT_M  0x3u
#define ADCTRGMODE_SH3ALT(x) (((x) & ADCTRGMODE_SH4ALT_M) << 22)
#define ADCTRGMODE_SH2ALT_M  0x3u
#define ADCTRGMODE_SH2ALT(x) (((x) & ADCTRGMODE_SH4ALT_M) << 20)
#define ADCTRGMODE_SH1ALT_M  0x3u
#define ADCTRGMODE_SH1ALT(x) (((x) & ADCTRGMODE_SH4ALT_M) << 18)
#define ADCTRGMODE_SH0ALT_M  0x3u
#define ADCTRGMODE_SH0ALT(x) (((x) & ADCTRGMODE_SH4ALT_M) << 16)
#define ADCTRGMODE_STRGEN4   (1 << 12)
#define ADCTRGMODE_STRGEN3   (1 << 11)
#define ADCTRGMODE_STRGEN2   (1 << 10)
#define ADCTRGMODE_STRGEN1   (1 << 9)
#define ADCTRGMODE_STRGEN0   (1 << 8)
#define ADCTRGMODE_SSAMPEN4  (1 << 4)
#define ADCTRGMODE_SSAMPEN3  (1 << 3)
#define ADCTRGMODE_SSAMPEN2  (1 << 2)
#define ADCTRGMODE_SSAMPEN1  (1 << 1)
#define ADCTRGMODE_SSAMPEN0  (1 << 0)

#define ADCFLTRx_AFEN        (1u << 31)
#define ADCFLTRx_DATA16EN    (1 << 30)
#define ADCFLTRx_DFMODE      (1 << 29)
#define ADCFLTRx_OVRSAM_M    0x7u
#define ADCFLTRx_OVRSAM(x)   (((x) & ADCFLTRx_OVRSAM_M) << 26)
#define ADCFLTRx_AFGIEN      (1 << 25)
#define ADCFLTRx_AFRDY       (1 << 24)
#define ADCFLTRx_CHNLID_M    0x1fu
#define ADCFLTRx_CHNLID(x)   (((x) & ADCFLTRx_CHNLID_M) << 16)
#define ADCFLTRx_FLTRDATA_M  0xffffu
#define ADCFLTRx_FLTRDATA(x) ((x) & ADCFLTRx_FLTRDATA_M)

#define ADCCMPCON1_CVDDATA_M  0xffffu
#define ADCCMPCON1_CVDDATA(x) (((x) & ADCCMPCON1_CVDDATA_M) << 16)
#define ADCCMPCON1_AINID_M    0x3fu
#define ADCCMPCON1_AINID(x)   (((x) & ADCCMPCON1_AINID_M) << 8)
#define ADCCMPCON1_ENCDMP     (1 << 7)
#define ADCCMPCON1_DCMPGIEN   (1 << 6)
#define ADCCMPCON1_DCMPED     (1 << 5)
#define ADCCMPCON1_IEBTWN     (1 << 4)
#define ADCCMPCON1_IEHIHI     (1 << 3)
#define ADCCMPCON1_IEHILO     (1 << 2)
#define ADCCMPCON1_IELOHI     (1 << 1)
#define ADCCMPCON1_IELOLO     (1 << 0)

#define ADCCMPCONx_AINID_M  0x1fu
#define ADCCMPCONx_AINID(x) (((x) & ADCCMPCON1_AINID_M) << 8)
#define ADCCMPCONx_ENCDMP   (1 << 7)
#define ADCCMPCONx_DCMPGIEN (1 << 6)
#define ADCCMPCONx_DCMPED   (1 << 5)
#define ADCCMPCONx_IEBTWN   (1 << 4)
#define ADCCMPCONx_IEHIHI   (1 << 3)
#define ADCCMPCONx_IEHILO   (1 << 2)
#define ADCCMPCONx_IELOHI   (1 << 1)
#define ADCCMPCONx_IELOLO   (1 << 0)

#define ADCTRG_TRGSRC0_M  0x1fu
#define ADCTRG_TRGSRC0(x) ((x) & ADCTRG_TRGSRC0_M)

#define ADCFSTAT_FEN      (1u << 31)
#define ADCFSTAT_ADC4EN   (1 << 28)
#define ADCFSTAT_ADC3EN   (1 << 27)
#define ADCFSTAT_ADC2EN   (1 << 26)
#define ADCFSTAT_ADC1EN   (1 << 25)
#define ADCFSTAT_ADC0EN   (1 << 24)
#define ADCFSTAT_FIEN     (1 << 23)
#define ADCFSTAT_FRDY     (1 << 22)
#define ADCFSTAT_FWROVERR (1 << 21)
#define ADCFSTAT_FCNT_M   0xffu
#define ADCFSTAT_FCNT(x)  (((x) & ADCFSTAT_FCNT_M) << 8)
#define ADCFSTAT_FSIGN    (1 << 7)
#define ADCFSTAT_ADCID_M  0x7u
#define ADCFSTAT_ADCID(x) ((x) & ADCFSTAT_ADCID_M)

#define ADCTRGSNS_LVL(x) (0x1u << (x))

#define ADCxTIME_ADCEIS_M  0x7u
#define ADCxTIME_ADCEIS(x) (((x) & ADCxTIME_ADCEIS_M) << 26)
#define ADCxTIME_SELRES_M  0x3u
#define ADCxTIME_SELRES(x) (((x) & ADCxTIME_SELRES_M) << 24)
#define ADCxTIME_ADCDIV_M  0x3fu
#define ADCxTIME_ADCDIV(x) (((x) & ADCxTIME_ADCDIV_M) << 16)
#define ADCxTIME_SAMC_M    0x3ffu
#define ADCxTIME_SAMC(x)   ((x) & ADCxTIME_SAMC_M)

#define ADCANCON_WKUPCLKCNT_M  0xfu
#define ADCANCON_WKUPCLKCNT(x) (((x) & ADCANCON_WKUPCLKCNT_M) << 24)
#define ADCANCON_WKIENx(x)     (0x1u << ((x) + (size_t)16))
#define ADCANCON_WKRDYx(x)     (0x1u << ((x) + (size_t)8))
#define ADCANCON_ANENx(x)      (0x1u << (x))

static void calibrate_from_flash(struct adc_pic32mx *ctx)
{
    ctx->base->ADCxCFG[0] = *(uint32_t*)(ADDR_DEVADCx + 0x0);
    ctx->base->ADCxCFG[1] = *(uint32_t*)(ADDR_DEVADCx + 0x4);
    ctx->base->ADCxCFG[2] = *(uint32_t*)(ADDR_DEVADCx + 0x8);
    ctx->base->ADCxCFG[3] = *(uint32_t*)(ADDR_DEVADCx + 0xc);
    ctx->base->ADCxCFG[4] = *(uint32_t*)(ADDR_DEVADCx + 0x10);
    ctx->base->ADCxCFG[5] = *(uint32_t*)(ADDR_DEVADCx + 0x14);
    ctx->base->ADCxCFG[6] = *(uint32_t*)(ADDR_DEVADCx + 0x18);
    ctx->base->ADCxCFG[7] = *(uint32_t*)(ADDR_DEVADCx + 0x1c);
}

static int vref_ready_busywait(struct adc_pic32mx *ctx)
{
    int loop = CONFIG_DEADLOCK_COUNT;

    while (loop-- != 0)
        if ((ctx->base->ADCCON2 & ADCCON2_BGVRRDY) != 0 &&
            (ctx->base->ADCCON2 & ADCCON2_REFFLT) == 0)
            break;

    picoRTOS_assert(loop != -1, return -EBUSY);
    return 0;
}

/* Function: adc_pic32mx_init
 * Init an ADC block
 *
 * Parameters:
 *  ctx - The ADC block to init
 *  base - The ADC block base address
 *
 * Returns:
 * Awlays 0
 */
int adc_pic32mx_init(struct adc_pic32mx *ctx, int base)
{
    ctx->base = (struct ADC_PIC32MX*)base;
    ctx->strgsrc = ADC_PIC32MX_STRGSRC_NONE;

    /* init ADC calibration settings */
    calibrate_from_flash(ctx);

    /* reset */
    ctx->base->ADCCON1 = 0;
    ctx->base->ADCCON2 = 0;

    return 0;
}

/* Function: adc_pic32mx_setup
 * Configures an ADC block
 *
 * Parameters:
 *  ctx - The ADC block to init
 *  settings - The ADC block parameters
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int adc_pic32mx_setup(struct adc_pic32mx *ctx, struct adc_pic32mx_settings *settings)
{
    picoRTOS_assert(settings->wkupclkcnt <= (unsigned long)ADCANCON_WKUPCLKCNT_M, return -EINVAL);
    picoRTOS_assert(settings->adcsel < ADC_PIC32MX_ADCSEL_COUNT, return -EINVAL);
    picoRTOS_assert(settings->conclkdiv != 0, return -EINVAL);
    picoRTOS_assert(settings->conclkdiv <= (unsigned long)ADCCON3_CONCLKDIV_M + 1ul, return -EINVAL);
    picoRTOS_assert(settings->vrefsel < ADC_PIC32MX_VREFSEL_COUNT, return -EINVAL);
    picoRTOS_assert(settings->strgsrc < ADC_PIC32MX_STRGSRC_COUNT, return -EINVAL);
    picoRTOS_assert(settings->strgsrc != ADC_PIC32MX_STRGSRC_RESERVED, return -EINVAL);

    /* turn off adc */
    ctx->base->ADCCON1 &= ~ADCCON1_ON;

    /* warm up time register */
    ctx->base->ADCANCON = (uint32_t)ADCANCON_WKUPCLKCNT(settings->wkupclkcnt);

    /* clock setting */
    ctx->base->ADCCON3 = (uint32_t)ADCCON3_ADCSEL(settings->adcsel);
    ctx->base->ADCCON3 |= ADCCON3_CONCLKDIV(settings->conclkdiv - 1ul);
    ctx->base->ADCCON3 |= ADCCON3_VREFSEL(settings->vrefsel);

    /* scan trigger */
    ctx->base->ADCCON1 &= ~ADCCON1_STRGSRC(ADCCON1_STRGSRC_M);
    ctx->base->ADCCON1 |= ADCCON1_STRGSRC(settings->strgsrc);
    ctx->strgsrc = settings->strgsrc;

    /* turn on ADC */
    ctx->base->ADCCON1 |= ADCCON1_ON;
    return vref_ready_busywait(ctx);
}

/* channels */

static int wakeup_ready_busywait(struct adc *ctx)
{
    int loop = CONFIG_DEADLOCK_COUNT;
    struct adc_pic32mx *parent = ctx->parent;

    while (loop-- != 0)
        if ((parent->base->ADCANCON & ADCANCON_WKRDYx(ctx->sar)) != 0)
            break;

    picoRTOS_assert(loop != -1, return -EBUSY);
    return 0;
}

static int sar_from_channel(size_t channel)
{
    picoRTOS_assert(channel < (size_t)ADC_PIC32MX_CHANNEL_COUNT, return -EINVAL);

    if (channel < (size_t)DEVICE_ADC_DEDICATED_SAR_COUNT)
        return (int)channel;

    return ADC_PIC32MX_SHARED_SAR;
}

/* Function: adc_pic32mx_adc_init
 * Inits an ADC channel
 *
 * Parameters:
 *  ctx - The ADC channel to init
 *  parent - The parent block object
 *  channel - The channel number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int adc_pic32mx_adc_init(struct adc *ctx, struct adc_pic32mx *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)ADC_PIC32MX_CHANNEL_COUNT, return -EINVAL);

    ctx->parent = parent;
    ctx->channel = channel;
    ctx->sar = (size_t)sar_from_channel(channel);
    ctx->trgsrc = ADC_PIC32MX_ADC_TRGSRC_NONE;

    /* claibration */
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

    /* don't enable adc module yet */
    return 0;
}

static int set_input_mode(struct adc *ctx, adc_pic32mx_adc_im_t im)
{
    picoRTOS_assert(im < ADC_PIC32MX_ADC_IM_COUNT, return -EINVAL);

    struct adc_pic32mx *parent = ctx->parent;

    size_t index = ctx->channel >> 4;
    size_t sign_bit = (size_t)((ctx->channel & 0xfu) * 2ul);
    size_t diff_bit = sign_bit + (size_t)1;

    switch (im) {
    case ADC_PIC32MX_ADC_IM_UNSIGNED_DATA_SINGLE_ENDED:
        parent->base->ADCIMCONx[index] &= ~sign_bit;
        parent->base->ADCIMCONx[index] &= ~diff_bit;
        break;

    case ADC_PIC32MX_ADC_IM_SIGNED_DATA_SINGLE_ENDED:
        parent->base->ADCIMCONx[index] |= sign_bit;
        parent->base->ADCIMCONx[index] &= ~diff_bit;
        break;

    case ADC_PIC32MX_ADC_IM_UNSIGNED_DATA_DIFFERENTIAL:
        parent->base->ADCIMCONx[index] &= ~sign_bit;
        parent->base->ADCIMCONx[index] |= diff_bit;
        break;

    case ADC_PIC32MX_ADC_IM_SIGNED_DATA_DIFFERENTIAL:
        parent->base->ADCIMCONx[index] |= sign_bit;
        parent->base->ADCIMCONx[index] |= diff_bit;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

static int add_channel_to_css(struct adc *ctx)
{
    struct adc_pic32mx *parent = ctx->parent;

    size_t index = (ctx->channel >> 5);
    size_t shift = (size_t)(ctx->channel & 0x1fu);

    parent->base->ADCCSS[index] |= (1 << shift);
    return 0;
}

static int set_trgsrc_and_lvl(struct adc *ctx,
                              adc_pic32mx_adc_trgsrc_t trgsrc,
                              adc_pic32mx_adc_lvl_t lvl)
{
    picoRTOS_assert(trgsrc < ADC_PIC32MX_ADC_TRGSRC_COUNT, return -EINVAL);
    picoRTOS_assert(lvl < ADC_PIC32MX_ADC_LVL_COUNT, return -EINVAL);
    picoRTOS_assert(ctx->channel < (size_t)ADC_PIC32MX_TRGSRCABLE_COUNT, return -EIO);

    struct adc_pic32mx *parent = ctx->parent;
    size_t shift = (size_t)((ctx->channel & 0x3) * 8);
    size_t index = ctx->channel >> 2;

    parent->base->ADCTRG[index] = (uint32_t)(ADCTRG_TRGSRC0(trgsrc) << shift);

    /* level */
    if (lvl == ADC_PIC32MX_ADC_LVL_HIGH)
        parent->base->ADCTRGSNS |= ADCTRGSNS_LVL(ctx->channel);
    else
        parent->base->ADCTRGSNS &= ~ADCTRGSNS_LVL(ctx->channel);

    /* scan */
    if (trgsrc == ADC_PIC32MX_ADC_TRGSRC_STRIG)
        (void)add_channel_to_css(ctx);

    /* remember this */
    ctx->trgsrc = trgsrc;
    return 0;
}

static int adc_pic32mx_shared_setup(struct adc *ctx,
                                    struct adc_pic32mx_adc_settings *settings)
{
    picoRTOS_assert(settings->selres < ADC_PIC32MX_ADC_SELRES_COUNT, return -EINVAL);
    picoRTOS_assert(settings->samc >= 2ul, return -EINVAL);
    picoRTOS_assert(settings->samc <= (unsigned long)ADCxTIME_SAMC_M + 2ul, return -EINVAL);
    picoRTOS_assert(settings->lvl < ADC_PIC32MX_ADC_LVL_COUNT, return -EINVAL);

    struct adc_pic32mx *parent = ctx->parent;

    /* reslution */
    parent->base->ADCCON1 &= ~ADCCON1_SELRES(ADCCON1_SELRES_M);
    parent->base->ADCCON1 |= ADCCON1_SELRES(settings->selres);
    /* sample time */
    parent->base->ADCCON2 &= ~ADCCON2_SAMC(ADCCON2_SAMC_M);
    parent->base->ADCCON2 |= ADCCON2_SAMC(settings->samc - 2ul);
    /* divider */
    parent->base->ADCCON2 &= ~ADCCON2_ADCDIV(ADCCON2_ADCDIV_M);
    parent->base->ADCCON2 |= ADCCON2_ADCDIV(settings->adcdiv);

    /* lvl */
    if (settings->lvl == ADC_PIC32MX_ADC_LVL_HIGH)
        parent->base->ADCCON1 |= ADCCON1_STRGLVL;
    else
        parent->base->ADCCON1 &= ~ADCCON1_STRGLVL;

    /* class 3 channel */
    if (ctx->channel >= (size_t)ADC_PIC32MX_TRGSRCABLE_COUNT) {
        (void)add_channel_to_css(ctx);
        ctx->trgsrc = (adc_pic32mx_adc_trgsrc_t)parent->strgsrc;
    }

    return 0;
}

/* Function: adc_pic32mx_adc_setup
 * Configures an ADC channel
 *
 * Parameters:
 *  ctx - The ADC channel to configure
 *  settings - The channel parameters
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int adc_pic32mx_adc_setup(struct adc *ctx, struct adc_pic32mx_adc_settings *settings)
{
    picoRTOS_assert(settings->adcdiv >= 2ul, return -EINVAL);
    picoRTOS_assert(settings->adcdiv <= (unsigned long)ADCxTIME_ADCDIV_M + 1ul, return -EINVAL);
    picoRTOS_assert(settings->selres < ADC_PIC32MX_ADC_SELRES_COUNT, return -EINVAL);
    picoRTOS_assert(settings->samc >= 2ul, return -EINVAL);
    picoRTOS_assert(settings->samc <= (unsigned long)ADCxTIME_SAMC_M + 2ul, return -EINVAL);

    int res;
    struct adc_pic32mx *parent = ctx->parent;

    if ((res = set_input_mode(ctx, settings->im)) < 0)
        return res;

    /* class 1 & 2 channels */
    if (ctx->channel < (size_t)ADC_PIC32MX_TRGSRCABLE_COUNT &&
        (res = set_trgsrc_and_lvl(ctx, settings->trgsrc, settings->lvl)) < 0)
        return res;

    if (ctx->sar == (size_t)ADC_PIC32MX_SHARED_SAR) {
        /* setup for shared ADC */
        if ((res = adc_pic32mx_shared_setup(ctx, settings)) < 0)
            return res;

    }else{
        /* unshared ADCs */
        parent->base->ADCxTIME[ctx->sar] = (uint32_t)ADCxTIME_ADCDIV(settings->adcdiv - 1ul);
        parent->base->ADCxTIME[ctx->sar] |= ADCxTIME_SELRES(settings->selres);
        parent->base->ADCxTIME[ctx->sar] |= ADCxTIME_SAMC(settings->samc - 2ul);
    }

    /* enable ADC */
    parent->base->ADCANCON |= ADCANCON_ANENx(ctx->sar);
    if ((res = wakeup_ready_busywait(ctx)) < 0)
        return res;

    parent->base->ADCCON3 |= ADCCON3_DIGENx(ctx->sar);
    return 0;
}

/* hooks */

int adc_setup(struct adc *ctx, struct adc_settings *settings)
{
    ctx->multiplier = settings->multiplier;
    ctx->divider = settings->divider;
    ctx->offset = settings->offset;

    return 0;
}

static bool adc_data_is_ready(struct adc *ctx)
{
    struct adc_pic32mx *parent = ctx->parent;
    size_t shift = (size_t)(ctx->channel & 0x1f);
    size_t index = ctx->channel >> 5;

    /* reset scan */
    /*@i@*/ (void)parent->base->ADCCON2;

    return (parent->base->ADCDSTAT[index] & (1 << shift)) != 0;
}

int adc_read(struct adc *ctx, int *data)
{
    uint32_t adcdatax;
    struct adc_pic32mx *parent = ctx->parent;

    if (!adc_data_is_ready(ctx)) {
        /* software triggers */
        if (ctx->trgsrc == ADC_PIC32MX_ADC_TRGSRC_GSWTRG)
            parent->base->ADCCON3 |= ADCCON3_GSWTRG;

        if (ctx->trgsrc == ADC_PIC32MX_ADC_TRGSRC_GLSWTRG)
            parent->base->ADCCON3 |= ADCCON3_GLSWTRG;

        return -EAGAIN;
    }

    /* get data */
    adcdatax = parent->base->ADCDATAx[ctx->channel];
    /* apply calibration */
    *data = ((int)adcdatax * ctx->multiplier) / ctx->divider + ctx->offset;

    return 1;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    return adc_read(ctx, data);
}

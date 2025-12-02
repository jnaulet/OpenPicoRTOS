#include "adc-stm32h7xx_sar.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <stdint.h>

struct ADC_STM32H7XX_SAR {
    volatile uint32_t ISR;
    volatile uint32_t IER;
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CFGR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t PCSEL;
    volatile uint32_t LTR1;
    volatile uint32_t HTR1;
    uint32_t RESERVED0[2];
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t SQR4;
    volatile uint32_t DR;
    uint32_t RESERVED1[2];
    volatile uint32_t JSQR;
    uint32_t RESERVED2[4];
    volatile uint32_t OFRy[4];
    uint32_t RESERVED3[4];
    volatile uint32_t JDRy[4];
    uint32_t RESERVED4[4];
    volatile uint32_t AWD2CR;
    volatile uint32_t AWD3CR;
    uint32_t RESERVED5[2];
    volatile uint32_t LTR2;
    volatile uint32_t HTR2;
    volatile uint32_t LTR3;
    volatile uint32_t HTR3;
    volatile uint32_t DIFSEL;
    volatile uint32_t CALFACT;
    volatile uint32_t CALFACT2;
};

#define ISR_LDORDY (1 << 12)
#define ISR_JQOVF  (1 << 10)
#define ISR_AWD3   (1 << 9)
#define ISR_AWD2   (1 << 8)
#define ISR_AWD1   (1 << 7)
#define ISR_JEOS   (1 << 6)
#define ISR_JEOC   (1 << 5)
#define ISR_OVR    (1 << 4)
#define ISR_EOS    (1 << 3)
#define ISR_EOC    (1 << 2)
#define ISR_EOSMP  (1 << 1)
#define ISR_ADRDY  (1 << 0)

#define IER_LDORDYIE (1 << 12)
#define IER_JQOVFIE  (1 << 10)
#define IER_AWD3IE   (1 << 9)
#define IER_AWD2IE   (1 << 8)
#define IER_AWD1IE   (1 << 7)
#define IER_JEOSIE   (1 << 6)
#define IER_JEOCIE   (1 << 5)
#define IER_OVRIE    (1 << 4)
#define IER_EOSIE    (1 << 3)
#define IER_EOCIE    (1 << 2)
#define IER_EOSMPIE  (1 << 1)
#define IER_ADRDYIE  (1 << 0)

#define CR_ADCAL       (1u << 31)
#define CR_ADCALDIF    (1 << 30)
#define CR_DEEPPWD     (1 << 29)
#define CR_AVDREGEN    (1 << 28)
#define CR_LINCALRDYW6 (1 << 27)
#define CR_LINCALRDYW5 (1 << 26)
#define CR_LINCALRDYW4 (1 << 25)
#define CR_LINCALRDYW3 (1 << 24)
#define CR_LINCALRDYW2 (1 << 23)
#define CR_LINCALRDYW1 (1 << 22)
#define CR_ADCALLIN    (1 << 16)
#define CR_BOOST_M     0x3u
#define CR_BOOST(x)    (((x) & CR_BOOST_M) << 8)
#define CR_JADSTP      (1 << 5)
#define CR_ADSTP       (1 << 4)
#define CR_JADSTART    (1 << 3)
#define CR_ADSTART     (1 << 2)
#define CR_ADDIS       (1 << 1)
#define CR_ADEN        (1 << 0)

#define CFGR_JQDIS      (1u << 31)
#define CFGR_AWD1CH_M   0x1fu
#define CFGR_AWD1CH(x)  (((x) & CFGR_AWD1CH_M) << 26)
#define CFGR_JAUTO      (1 << 25)
#define CFGR_JAWD1EN    (1 << 24)
#define CFGR_AWD1EN     (1 << 23)
#define CFGR_AWD1SGL    (1 << 22)
#define CFGR_JQM        (1 << 21)
#define CFGR_JDISCEN    (1 << 20)
#define CFGR_DISCNUM_M  0x7u
#define CFGR_DISCNUM(x) (((x) & CFGR_DISCNUM_M) << 17)
#define CFGR_DISCEN     (1 << 16)
#define CFGR_AUTDLY     (1 << 14)
#define CFGR_CONT       (1 << 13)
#define CFGR_OVRMOD     (1 << 12)
#define CFGR_EXTEN_M    0x3u
#define CFGR_EXTEN(x)   (((x) & CFGR_EXTEN_M) << 10)
#define CFGR_EXTSEL_M   0x1fu
#define CFGR_EXTSEL(x)  (((x) & CFGR_EXTSEL_M) << 5)
#define CFGR_RES_M      0x3u
#define CFGR_RES(x)     (((x) & CFGR_RES_M) << 2)
#define CFGR_DMNGT_M    0x3u
#define CFGR_DMNGT(x)   ((x) & CFGR_DMNGT_M)

#define CFGR2_LSHIFT_M  0xfu
#define CFGR2_LSHIFT(x) (((x) & CFGR2_LSHIFT_M) << 28)
#define CFGR2_OSVR_M    0x3ffu
#define CFGR2_OSVR(x)   (((x) & CFGR2_OSVR_M) << 16)
#define CFGR2_RSHIFT4   (1 << 14)
#define CFGR2_RSHIFT3   (1 << 13)
#define CFGR2_RSHIFT2   (1 << 12)
#define CFGR2_RSHIFT1   (1 << 11)
#define CFGR2_ROSVM     (1 << 10)
#define CFGR2_TROVS     (1 << 9)
#define CFGR2_OVSS_M    0xfu
#define CFGR2_OVSS(x)   (((x) & CFGR2_OVSS_M) << 5)
#define CFGR2_JOVSE     (1 << 1)
#define CFGR2_ROVSE     (1 << 0)

#define SMPR1_SMP0_M  0x7u
#define SMPR1_SMP0(x) ((x) & SMPR1_SMP0_M)

#define SMPR2_SMP10_M  0x7u
#define SMPR2_SMP10(x) ((x) & SMPR2_SMP10_M)

#define LTR1_LTR1_M  0x3ffffffu
#define LTR1_LTR1(x) ((x) & LTR1_LTR1_M)

#define HTR1_HTR1_M  0x3ffffffu
#define HTR1_HTR1(x) ((x) & HTR1_HTR1_M)

#define SQR1_SQ4_M  0x1fu
#define SQR1_SQ4(x) (((x) & SQR1_SQ4_M) << 24)
#define SQR1_SQ3_M  0x1fu
#define SQR1_SQ3(x) (((x) & SQR1_SQ3_M) << 18)
#define SQR1_SQ2_M  0x1fu
#define SQR1_SQ2(x) (((x) & SQR1_SQ2_M) << 12)
#define SQR1_SQ1_M  0x1fu
#define SQR1_SQ1(x) (((x) & SQR1_SQ1_M) << 6)
#define SQR1_L_M    0xfu
#define SQR1_L(x)   ((x) & SQR1_L_M)

#define SQR2_SQ9_M  0x1fu
#define SQR2_SQ9(x) (((x) & SQR2_SQ9_M) << 24)
#define SQR2_SQ8_M  0x1fu
#define SQR2_SQ8(x) (((x) & SQR2_SQ8_M) << 18)
#define SQR2_SQ7_M  0x1fu
#define SQR2_SQ7(x) (((x) & SQR2_SQ7_M) << 12)
#define SQR2_SQ6_M  0x1fu
#define SQR2_SQ6(x) (((x) & SQR2_SQ6_M) << 6)
#define SQR2_SQ5_M  0x1fu
#define SQR2_SQ5(x) ((x) & SQR2_SQ5_M)

#define SQR3_SQ14_M  0x1fu
#define SQR3_SQ14(x) (((x) & SQR3_SQ14_M) << 24)
#define SQR3_SQ13_M  0x1fu
#define SQR3_SQ13(x) (((x) & SQR3_SQ13_M) << 18)
#define SQR3_SQ12_M  0x1fu
#define SQR3_SQ12(x) (((x) & SQR3_SQ12_M) << 12)
#define SQR3_SQ11_M  0x1fu
#define SQR3_SQ11(x) (((x) & SQR3_SQ11_M) << 6)
#define SQR3_SQ10_M  0x1fu
#define SQR3_SQ10(x) ((x) & SQR3_SQ10_M)

#define SQR4_SQ16_M  0x1fu
#define SQR4_SQ16(x) (((x) & SQR4_SQ15_M) << 6)
#define SQR4_SQ15_M  0x1fu
#define SQR4_SQ15(x) ((x) & SQR4_SQ15_M)

#define JSQR_JSQ1_M     0x1fu
#define JSQR_JSQ1(x)    (((x) & JSQR_JSQ1_M) << 9)
#define JSQR_JEXTEN_M   0x3u
#define JSQR_JEXTEN(x)  (((x) & JSQR_JEXTEN_M) << 7)
#define JSQR_JEXTSEL_M  0x1fu
#define JSQR_JEXTSEL(x) (((x) & JSQR_JEXTSEL_M) << 2)
#define JSQR_JL_M       0x3u
#define JSQR_JL(x)      ((x) & JSQR_JL_M)

#define OFRy_SSATE        (1u << 31)
#define OFRy_OFFSET_CH_M  0x1fu
#define OFRy_OFFSET_CH(x) (((x) & OFRy_OFFSET_CH_M) << 26)
#define OFRy_OFFSET_M     0x3ffffffu
#define OFRy_OFFSET(x)    ((x) & OFRy_OFFSET_M)

#define AWD2CR_AWD2CH_M  0xfffffu
#define AWD2CR_AWD2CH(x) ((x) & AWD2CR_AWD2CH_M)

#define AWD3CR_AWD3CH_M  0xfffffu
#define AWD3CR_AWD3CH(x) ((x) & AWD3CR_AWD3CH_M)

#define LTR2_LTR2_M  0x3ffffffu
#define LTR2_LTR2(x) ((x) & LTR2_LTR2_M)

#define HTR2_HTR2_M  0x3ffffffu
#define HTR2_HTR2(x) ((x) & HTR2_HTR2_M)

#define LTR3_LTR3_M  0x3ffffffu
#define LTR3_LTR3(x) ((x) & LTR3_LTR3_M)

#define HTR3_HTR3_M  0x3ffffffu
#define HTR3_HTR3(x) ((x) & HTR3_HTR3_M)

#define DIFSEL_DIFSEL_M  0xfffffu
#define DIFSEL_DIFSEL(x) ((x) & DIFSEL_DIFSEL_M)

#define CALFACT_CALFACT_D_M 0x7ffu
#define CALFACT_CALFACT_D(x) (((x) & CALFACT_CALFACT_D_M) << 16)
#define CALFACT_CALFACT_S_M 0x7ffu
#define CALFACT_CALFACT_S(x) ((x) & CALFACT_CALFACT_S_M)

#define CALFACT2_LINCALFACT_M  0x3fffffffu
#define CALFACT2_LINCALFACT(x) ((x) & CALFACT2_LINCALFACT_M)

struct ADC_STM32H7XX_SAR_COMMON {
    volatile uint32_t CSR;
    uint32_t RESERVED0;
    volatile uint32_t CCR;
    volatile uint32_t CDR;
    volatile uint32_t CDR2;
};

#define CCR_VBATEN    (1 << 24)
#define CCR_TSEN      (1 << 23)
#define CCR_VREFEN    (1 << 22)
#define CCR_PRESC_M   0xfu
#define CCR_PRESC(x)  (((x) & CCR_PRESC_M) << 18)
#define CCR_CKMODE_M  0x3u
#define CCR_CKMODE(x) (((x) & CCR_CKMODE_M) << 16)
#define CCR_DAMDF_M   0x3u
#define CCR_DAMDF(x)  (((x) & CCR_DAMDF_M) << 14)
#define CCR_DELAY_M   0xfu
#define CCR_DELAY(x)  (((x) & CCR_DELAY_M) << 8)
#define CCR_DUAL_M    0x1fu
#define CCR_DUAL(x)   ((x) & CCR_DUAL_M)

int adc_stm32h7xx_sar_init(struct adc_stm32h7xx_sar *ctx, int base,
                           clock_id_t clkid, adc_stm32h7xx_sar_t type)
{
    picoRTOS_assert(type < ADC_STM32H7XX_SAR_COUNT, return -EINVAL);

    int deadlock = CONFIG_DEADLOCK_COUNT;
    clock_freq_t freq = clock_get_freq(clkid);

    picoRTOS_assert(freq > 0, return -EINVAL);
    picoRTOS_assert(freq <= (clock_freq_t)50000000, return -EINVAL);

    ctx->clkid = clkid;
    ctx->base = (struct ADC_STM32H7XX_SAR*)
                ((type == ADC_STM32H7XX_SAR_MASTER) ?
                 base + 0x0 :
                 base + 0x100);
    ctx->common = (struct ADC_STM32H7XX_SAR_COMMON*)
                  ((type == ADC_STM32H7XX_SAR_MASTER) ?
                   base + 0x300 :
                   base + 0x200);

    /* basics */
    ctx->channel_count = 0;

    ctx->base->CR &= ~CR_BOOST(CR_BOOST_M);
    if (freq > (clock_freq_t)25000000) ctx->base->CR |= CR_BOOST(3);
    else if (freq > (clock_freq_t)12500000) ctx->base->CR |= CR_BOOST(2);
    else if (freq > (clock_freq_t)6250000) ctx->base->CR |= CR_BOOST(1);
    else ctx->base->CR |= CR_BOOST(0);

    /* leave deep-power-down mode & enable voltage regulator */
    ctx->base->CR &= ~CR_DEEPPWD;
    ctx->base->CR |= CR_AVDREGEN;
    while ((ctx->base->ISR & ISR_LDORDY) == 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    /* run calibration */
    ctx->base->CR |= CR_ADCAL;
    while ((ctx->base->CR & CR_ADCAL) != 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int adc_disable(struct adc_stm32h7xx_sar *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* don't run if it's not necessary */
    if ((ctx->base->CR & CR_ADEN) == 0)
        return 0;

    /* check no conversion is happening */
    if ((ctx->base->CR & (CR_ADSTART | CR_JADSTART)) != 0)
        return -EAGAIN;

    /* disable */
    ctx->base->CR |= CR_ADDIS;
    while ((ctx->base->CR & CR_ADEN) != 0 &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int adc_dma_setup(struct adc_stm32h7xx_sar *ctx, struct dma *drain)
{
    /* null check */
    picoRTOS_assert(drain != NULL, return -EIO);

    ctx->drain = drain;
    /* prepare xfer */
    ctx->drain_xfer.saddr = (intptr_t)&ctx->base->DR;
    ctx->drain_xfer.daddr = (intptr_t)ctx->DR;
    ctx->drain_xfer.incr_read = DMA_XFER_INCREMENT_OFF;
    ctx->drain_xfer.incr_write = DMA_XFER_INCREMENT_ON;
    ctx->drain_xfer.size = sizeof(*ctx->DR);
    ctx->drain_xfer.byte_count = 0; /* TBD */

    /* only one-shot mode yet */
    ctx->base->CFGR &= ~CFGR_DMNGT(CFGR_DMNGT_M);
    ctx->base->CFGR |= CFGR_DMNGT(0x1);

    return 0;
}

static int adc_enable(struct adc_stm32h7xx_sar *ctx)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* enable */
    ctx->base->ISR = (uint32_t)ISR_ADRDY;
    ctx->base->CR |= CR_ADEN;
    while ((ctx->base->ISR & ISR_ADRDY) == 0 &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

int adc_stm32h7xx_sar_setup(struct adc_stm32h7xx_sar *ctx,
                            const struct adc_stm32h7xx_sar_settings *settings)
{
    picoRTOS_assert(settings->presc < ADC_STM32H7XX_SAR_PRESC_COUNT, return -EINVAL);
    picoRTOS_assert(settings->ckmode < ADC_STM32H7XX_SAR_CKMODE_COUNT, return -EINVAL);
    picoRTOS_assert(settings->res < ADC_STM32H7XX_SAR_RES_COUNT, return -EINVAL);
    picoRTOS_assert(settings->osvr >= ADC_STM32H7XX_SAR_OSVR_MIN, return -EINVAL);
    picoRTOS_assert(settings->osvr <= ADC_STM32H7XX_SAR_OSVR_MAX, return -EINVAL);

    int res;

    if ((res = adc_disable(ctx)) < 0)
        return res;

    /* common */
    ctx->common->CCR = (uint32_t)((settings->tsen ? CCR_TSEN : 0) |
                                  CCR_PRESC(settings->presc) |
                                  CCR_CKMODE(settings->ckmode));
    /* local */
    if (settings->cont == ADC_STM32H7XX_SAR_CONT_CONTINUOUS) ctx->base->CFGR |= CFGR_CONT;
    else ctx->base->CFGR &= ~CFGR_CONT;

    /* res */
    ctx->base->CFGR &= ~CFGR_RES(CFGR_RES_M);
    ctx->base->CFGR |= CFGR_RES(settings->res);

    /* osvr */
    ctx->base->CFGR2 &= ~CFGR2_OSVR(CFGR2_OSVR_M);
    ctx->base->CFGR2 |= CFGR2_OSVR(settings->osvr - 1);

    /* dma */
    if ((res = adc_dma_setup(ctx, settings->drain)) < 0)
        return res;

    return adc_enable(ctx);
}

int adc_stm32h7xx_sar_adc_init(struct adc *ctx,
                               struct adc_stm32h7xx_sar *parent,
                               size_t channel)
{
    picoRTOS_assert(channel < (size_t)ADC_STM32H7XX_SAR_ADC_CHANNEL_COUNT, return -EINVAL);

    ctx->parent = parent;
    ctx->channel = channel;
    ctx->index = parent->channel_count;

    /* basics */
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

    switch (parent->channel_count) {
    case 15: parent->base->SQR4 |= SQR4_SQ16(channel); break;
    case 14: parent->base->SQR4 |= SQR4_SQ15(channel); break;
    case 13: parent->base->SQR3 |= SQR3_SQ14(channel); break;
    case 12: parent->base->SQR3 |= SQR3_SQ13(channel); break;
    case 11: parent->base->SQR3 |= SQR3_SQ12(channel); break;
    case 10: parent->base->SQR3 |= SQR3_SQ11(channel); break;
    case 9: parent->base->SQR3 |= SQR3_SQ10(channel); break;
    case 8: parent->base->SQR2 |= SQR2_SQ9(channel); break;
    case 7: parent->base->SQR2 |= SQR2_SQ8(channel); break;
    case 6: parent->base->SQR2 |= SQR2_SQ7(channel); break;
    case 5: parent->base->SQR2 |= SQR2_SQ6(channel); break;
    case 4: parent->base->SQR2 |= SQR2_SQ5(channel); break;
    case 3: parent->base->SQR1 |= SQR1_SQ4(channel); break;
    case 2: parent->base->SQR1 |= SQR1_SQ3(channel); break;
    case 1: parent->base->SQR1 |= SQR1_SQ2(channel); break;
    case 0: parent->base->SQR1 |= SQR1_SQ1(channel); break;
    default:
        picoRTOS_assert_void(false);
        /*@notreached@*/ return -EIO;
    }

    parent->base->SQR1 &= ~SQR1_L(SQR1_L_M);
    parent->base->SQR1 |= SQR1_L(parent->channel_count++);
    parent->base->PCSEL |= (1u << ctx->channel);

    /* dma */
    parent->drain_xfer.byte_count = parent->channel_count * sizeof(*parent->DR);
    return 0;
}

int adc_stm32h7xx_sar_adc_setup(struct adc *ctx,
                                const struct adc_stm32h7xx_sar_adc_settings *settings)
{
    picoRTOS_assert(settings->smp < ADC_STM32H7XX_SAR_ADC_SMP_COUNT, return -EINVAL);

    struct adc_stm32h7xx_sar *parent = ctx->parent;

    if (ctx->channel < (size_t)9) {
        size_t shift = ctx->channel * (size_t)3;
        parent->base->SMPR1 &= ~(SMPR1_SMP0(SMPR1_SMP0_M) << shift);
        parent->base->SMPR1 |= (SMPR1_SMP0(settings->smp) << shift);
    }else{
        size_t shift = (ctx->channel - (size_t)9) * (size_t)3;
        parent->base->SMPR2 &= ~(SMPR2_SMP10(SMPR2_SMP10_M) << shift);
        parent->base->SMPR2 |= (SMPR2_SMP10(settings->smp) << shift);
    }

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
    struct adc_stm32h7xx_sar *parent = ctx->parent;
    uint32_t mask = (uint32_t)(1u << ctx->index);

    /* data is fresh and available */
    if ((parent->DR_mask & mask) != 0) {
        parent->DR_mask &= ~mask;
        goto end;
    }

    /* check if busy */
    if ((parent->base->CR & CR_ADSTART) != 0 ||
        dma_xfer_done(parent->drain) != 0)
        return -EAGAIN;

    /* conversion done */
    if ((parent->base->ISR & ISR_EOS) == 0) {
        /* start conversions */
        (void)dma_setup(parent->drain, &parent->drain_xfer);
        parent->base->CR |= CR_ADSTART;
        /* invalidate cache */
        arch_invalidate_dcache(parent->DR, sizeof(parent->DR));
        return -EAGAIN;
    }

    /* flags */
    parent->base->ISR = (uint32_t)ISR_EOS;
    parent->DR_mask = ~mask; /* every channel but us */

end:
    /* apply calibration */
    *data = (((int)parent->DR[ctx->index] * ctx->multiplier) /
             ctx->divider + ctx->offset);

    return 1;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    return adc_read(ctx, data);
}

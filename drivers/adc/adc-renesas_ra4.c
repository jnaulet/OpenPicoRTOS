#include "adc-renesas_ra4.h"
#include "picoRTOS.h"

#include <stdint.h>

#define AADC_RENESAS_RA4_ADSSTRn_COUNT 15

struct ADC_RENESAS_RA4 {
    volatile uint16_t ADCSR;                                    /* c000h */
    uint16_t RESERVED0;
    volatile uint16_t ADANSA0;                                  /* c004h */
    volatile uint16_t ADANSA1;                                  /* c006h */
    volatile uint16_t ADADS0;                                   /* c008h */
    volatile uint16_t ADADS1;                                   /* c00ah */
    volatile uint8_t ADADC;                                     /* c00ch */
    uint8_t RESERVED1;
    volatile uint16_t ADCER;                                    /* c00eh */
    volatile uint16_t ADSTRGR;                                  /* c010h */
    volatile uint16_t ADEXICR;                                  /* c012h */
    volatile uint16_t ADANSB0;                                  /* c014h */
    volatile uint16_t ADANSB1;                                  /* c016h */
    volatile uint16_t ADDBLDR;                                  /* c018h */
    volatile uint16_t ADTSDR;                                   /* c01ah */
    volatile uint16_t ADOCDR;                                   /* c01ch */
    volatile uint16_t ADRD;                                     /* c01eh */
    volatile uint16_t ADDRn[ADC_RENESAS_RA4_CHANNEL_COUNT];     /* c020h */
    uint16_t RESERVED2[9];
    volatile uint8_t ADDISCR;                                   /* c07ah */
    uint8_t RESERVED3[5];
    volatile uint16_t ADGSPCR;                                  /* c080h */
    uint16_t RESERVED4;
    volatile uint16_t ADDBLDRA;                                 /* c084h */
    volatile uint16_t ADDBLDRB;                                 /* c086h */
    uint16_t RESERVED5;
    volatile uint8_t ADHVREFCNT;                                /* c08ah */
    uint8_t RESERVED6;
    volatile uint8_t ADWINMON;                                  /* c08ch */
    uint8_t RESERVED7[3];
    volatile uint16_t ADCMPCR;                                  /* c090h */
    volatile uint8_t ADCMPANSER;                                /* c092h */
    volatile uint8_t ADCMPLER;                                  /* c093h */
    volatile uint16_t ADCMPANSR0;                               /* c094h */
    volatile uint16_t ADCMPANSR1;                               /* c096h */
    volatile uint16_t ADCMPLR0;                                 /* c098h */
    volatile uint16_t ADCMPLR1;                                 /* c09ah */
    volatile uint16_t ADCPMDR0;                                 /* c09ch */
    volatile uint16_t ADCPMDR1;                                 /* c09eh */
    volatile uint16_t ADCPMSR0;                                 /* c0a0h */
    volatile uint16_t ADCPMSR1;                                 /* c0a2h */
    volatile uint8_t ADCMPSER;                                  /* c0a4h */
    uint8_t RESERVED8;
    volatile uint8_t ADCMPBNSR;                                 /* c0a6h */
    uint8_t RESERVED9;
    volatile uint16_t ADCWINLLB;                                /* c0a8h */
    volatile uint16_t ADCWINULB;                                /* c0aah */
    volatile uint8_t ADCMPBSR;                                  /* c0ach */
    uint8_t RESERVED10[48];
    volatile uint8_t ADSSTRL;                                   /* c0ddh */
    volatile uint8_t ADSSTRT;                                   /* c0deh */
    volatile uint8_t ADSSTRO;                                   /* c0dfh */
    volatile uint8_t ADSSTRn[AADC_RENESAS_RA4_ADSSTRn_COUNT];   /* c0e0h */
};

#define ADCSR_ADST      (1 << 15)
#define ADCSR_ADCS_M    0x3u
#define ADCSR_ADCS(x)   (((x) & ADCSR_ADCS_M) << 13)
#define ADCSR_ADHSC     (1 << 10)
#define ADCSR_TRGE      (1 << 9)
#define ADCSR_EXTRG     (1 << 8)
#define ADCSR_DBLE      (1 << 7)
#define ADCSR_GBADIE    (1 << 6)
#define ADCSR_DBLANS_M  0x1fu
#define ADCSR_DBLANS(x) ((x) & ADCSR_DBLANS_M)

#define ADADC_AVEE   (1 << 7)
#define ADADC_ADC_M  0x7u
#define ADADC_ADC(x) ((x) & ADADC_ADC_M)

#define ADCER_ADRFMT     (1 << 15)
#define ADCER_DIAGM      (1 << 11)
#define ADCER_DIAGLD     (1 << 10)
#define ADCER_DIAGVAL_M  0x3u
#define ADCER_DIAGVAL(x) (((x) & ADCER_DIAGVAL_M) << 8)
#define ADCER_ACE        (1 << 5)
#define ADCER_ADPRC_M    0x3u
#define ADCER_ADPRC(x)   ((x) & ADCER_ADPRC_M)

#define ADSTRGR_TRSA_M  0x3fu
#define ADSTRGR_TRSA(x) (((x) & ADSTRGR_TRSA_M) << 8)
#define ADSTRGR_TRSB_M  0x3fu
#define ADSTRGR_TRSB(x) ((x) & ADSTRGR_TRSB_M)

#define ADEXICR_OCSA  (1 << 9)
#define ADEXICR_TSSA  (1 << 8)
#define ADEXICR_OCSAD (1 << 1)
#define ADEXICR_TSSAD (1 << 0)

#define ADDISCR_PCHG      (1 << 4)
#define ADDISCR_ADNDIS_M  0xfu
#define ADDISCR_ADNDIS(x) ((x) & ADDISCR_ADNDIS_M)

#define ADGSPCR_GBRP   (1 << 15)
#define ADGSPCR_GBRSCN (1 << 1)
#define ADGSPCR_PGS    (1 << 0)

#define ADCMPCR_CMPAIE   (1 << 15)
#define ADCMPCR_WCMPE    (1 << 14)
#define ADCMPCR_CMPBIE   (1 << 13)
#define ADCMPCR_CMPAE    (1 << 11)
#define ADCMPCR_CMPBE    (1 << 9)
#define ADCMPCR_CMPAB_M  0x3u
#define ADCMPCR_CMPAB(x) ((x) & ADCMPCR_CMPAB_M)

#define ADCMPANSER_CMPOCA (1 << 1)
#define ADCMPANSER_CMPTSA (1 << 0)

#define ADCMPLER_CMPLOCA (1 << 1)
#define ADCMPLER_CMPLTSA (1 << 0)

#define ADCMPSER_CMPSTOCA (1 << 1)
#define ADCMPSER_CMPSTTSA (1 << 0)

#define ADCMPBNSR_CMPLB     (1 << 7)
#define ADCMPBNSR_CMPCHB_M  0x3fu
#define ADCMPBNSR_CMPCHB(x) ((x) & ADCMPBNSR_CMPCHB_M)

#define ADCMPBSR_CMPSTB (1 << 0)

#define ADWINMON_MONCMPB (1 << 5)
#define ADWINMON_MONCMPA (1 << 4)
#define ADWINMON_MONCOMB (1 << 0)

#define ADHVREFCNT_ADSLP    (1 << 7)
#define ADHVREFCNT_LVSEL    (1 << 4)
#define ADHVREFCNT_HVSEL_M  0x3u
#define ADHVREFCNT_HVSEL(x) ((x) & ADHVREFCNT_HVSEL_M)

/* Function: adc_renesas_ra4_init
 * Initializes an ADC block
 *
 * Parameters:
 *  ctx - The adc_renesas_ra4 instance to initialize
 *  base - The ADC base address
 *
 * Returns:
 * always 0
 */
int adc_renesas_ra4_init(struct adc_renesas_ra4 *ctx, int base)
{
    int n = ADC_RENESAS_RA4_CHANNEL_COUNT;

    ctx->base = (struct ADC_RENESAS_RA4*)base;
    ctx->adcs = ADC_RENESAS_RA4_ADCS_SINGLE;

    while (n-- != 0)
        ctx->status[n] = false;

    return 0;
}

/* Function: adc_renesas_ra4_setup
 * Configures an ADC block
 *
 * Parameters:
 *  ctx - The ADC block instance to configure
 *  settings - The settings to apply
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int adc_renesas_ra4_setup(struct adc_renesas_ra4 *ctx,
                          const struct adc_renesas_ra4_settings *settings)
{
    picoRTOS_assert(settings->adprc < ADC_RENESAS_RA4_ADPRC_COUNT, return -EINVAL);
    picoRTOS_assert(settings->adcs < ADC_RENESAS_RA4_ADCS_COUNT, return -EINVAL);

    /* ADPRC */
    ctx->base->ADCER &= ~ADCER_ADPRC(ADCER_ADPRC_M);
    if (settings->adprc == ADC_RENESAS_RA4_ADPRC_14BIT)
        ctx->base->ADCER |= ADCER_ADPRC(0x3);

    /* ADCS */
    ctx->base->ADCSR &= ~ADCSR_ADCS(ADCSR_ADCS_M);
    ctx->base->ADCSR |= ADCSR_ADCS(settings->adcs);

    /* remember */
    ctx->adcs = settings->adcs;

    return 0;
}

/* Function: adc_renesas_ra4_adc_init
 * Initializes an ADC channel
 *
 * Parameters:
 *  ctx - The ADC channel to init
 *  parent - The parent ADC block
 *  channel - The ADC channel number
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int adc_renesas_ra4_adc_init(struct adc *ctx,
                             struct adc_renesas_ra4 *parent,
                             size_t channel)
{
    picoRTOS_assert(channel < (size_t)ADC_RENESAS_RA4_CHANNEL_COUNT,
                    return -EINVAL);

    ctx->parent = parent;
    ctx->channel = channel;

    /* calibration */
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

    /* enable for scan */
    if (channel <= (size_t)14) parent->base->ADANSA0 |= (1 << channel);
    else parent->base->ADANSA1 |= (1 << (0xfu & channel));

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
    int addrn;

    if ((ctx->parent->base->ADCSR & ADCSR_ADST) != 0)
        return -EAGAIN;

    if (!ctx->parent->status[ctx->channel]) {
        /* already read */
        ctx->parent->status[ctx->channel] = true;
        ctx->parent->base->ADCSR |= ADCSR_ADST;
        return -EAGAIN;
    }

    addrn = (int)ctx->parent->base->ADDRn[ctx->channel];

    ctx->parent->status[ctx->channel] = false;
    *data = addrn * ctx->multiplier / ctx->divider + ctx->offset;

    return 1;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    return adc_read(ctx, data);
}

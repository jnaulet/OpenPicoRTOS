#ifndef ADC_PIC32MX_H
#define ADC_PIC32MX_H

#include "adc.h"

/*
 * This ADC is the Frankenstein's creature,
 * this is a minimalistic implementation
 */

struct ADC_PIC32MX;

/* ADC_PIC32MX_CHANNEL_COUNT is defined as
 * (DEVICE_ADC_DEDICATED_SAR_COUNT +
 *  DEVICE_ADC_SHARED_CL2_SAR_COUNT +
 *  DEVICE_ADC_SHARED_CL3_SAR_COUNT)
 * in src file
 */

typedef enum {
    ADC_PIC32MX_ADCSEL_PBCLK3   = 0,
    ADC_PIC32MX_ADCSEL_SYSCLK   = 1,
    ADC_PIC32MX_ADCSEL_REFCLK3  = 2,
    ADC_PIC32MX_ADCSEL_FRC      = 3,
    ADC_PIC32MX_ADCSEL_COUNT
} adc_pic32mx_adcsel_t;

typedef enum {
    ADC_PIC32MX_VREFSEL_AVDD_AVSS           = 0,
    ADC_PIC32MX_VREFSEL_EXTVREFH_AVSS       = 1,
    ADC_PIC32MX_VREFSEL_AVDD_EXTVREFL       = 2,
    ADC_PIC32MX_VREFSEL_EXTVREFH_EXTVREFL   = 3,
    ADC_PIC32MX_VREFSEL_COUNT
} adc_pic32mx_vrefsel_t;

typedef enum {
    ADC_PIC32MX_STRGSRC_NONE        = 0,
    ADC_PIC32MX_STRGSRC_GSWTRG      = 1,
    ADC_PIC32MX_STRGSRC_GLSWTRG     = 2,
    ADC_PIC32MX_STRGSRC_RESERVED    = 3,
    ADC_PIC32MX_STRGSRC_INT0        = 4,
    ADC_PIC32MX_STRGSRC_TMR1        = 5,
    ADC_PIC32MX_STRGSRC_TMR3        = 6,
    ADC_PIC32MX_STRGSRC_TMR5        = 7,
    ADC_PIC32MX_STRGSRC_OCMP1       = 8,
    ADC_PIC32MX_STRGSRC_OCMP3       = 9,
    ADC_PIC32MX_STRGSRC_OCMP5       = 10,
    ADC_PIC32MX_STRGSRC_COUT1       = 11,
    ADC_PIC32MX_STRGSRC_COUT2       = 12,
    ADC_PIC32MX_STRGSRC_COUNT
} adc_pic32mx_strgsrc_t;

struct adc_pic32mx {
    /*@temp@*/ struct ADC_PIC32MX *base;
    adc_pic32mx_strgsrc_t strgsrc;
};

struct adc_pic32mx_settings {
    unsigned long wkupclkcnt;       /* warm-up clock count */
    adc_pic32mx_adcsel_t adcsel;    /* clock source */
    unsigned long conclkdiv;        /* control clock div 1-64 */
    adc_pic32mx_vrefsel_t vrefsel;  /* voltage reference */
    adc_pic32mx_strgsrc_t strgsrc;  /* scan trigger source */
};

int adc_pic32mx_init(/*@out@*/ struct adc_pic32mx *ctx, int base);
int adc_pic32mx_setup(struct adc_pic32mx *ctx, const struct adc_pic32mx_settings *settings);

typedef enum {
    ADC_PIC32MX_ADC_IM_UNSIGNED_DATA_SINGLE_ENDED   = 0,
    ADC_PIC32MX_ADC_IM_SIGNED_DATA_SINGLE_ENDED     = 1,
    ADC_PIC32MX_ADC_IM_UNSIGNED_DATA_DIFFERENTIAL   = 2,
    ADC_PIC32MX_ADC_IM_SIGNED_DATA_DIFFERENTIAL     = 3,
    ADC_PIC32MX_ADC_IM_COUNT
} adc_pic32mx_adc_im_t;

typedef enum {
    ADC_PIC32MX_ADC_SELRES_6BITS    = 0,
    ADC_PIC32MX_ADC_SELRES_8BITS    = 1,
    ADC_PIC32MX_ADC_SELRES_10BITS   = 2,
    ADC_PIC32MX_ADC_SELRES_12BITS   = 3,
    ADC_PIC32MX_ADC_SELRES_COUNT
} adc_pic32mx_adc_selres_t;

typedef enum {
    ADC_PIC32MX_ADC_TRGSRC_NONE     = 0,
    ADC_PIC32MX_ADC_TRGSRC_GSWTRG   = 1,
    ADC_PIC32MX_ADC_TRGSRC_GLSWTRG  = 2,
    ADC_PIC32MX_ADC_TRGSRC_STRIG    = 3,
    ADC_PIC32MX_ADC_TRGSRC_INT0     = 4,
    ADC_PIC32MX_ADC_TRGSRC_TMR1     = 5,
    ADC_PIC32MX_ADC_TRGSRC_TMR3     = 6,
    ADC_PIC32MX_ADC_TRGSRC_TMR5     = 7,
    ADC_PIC32MX_ADC_TRGSRC_OCMP1    = 8,
    ADC_PIC32MX_ADC_TRGSRC_OCMP3    = 9,
    ADC_PIC32MX_ADC_TRGSRC_OCMP5    = 10,
    ADC_PIC32MX_ADC_TRGSRC_COUT1    = 11,
    ADC_PIC32MX_ADC_TRGSRC_COUT2    = 12,
    ADC_PIC32MX_ADC_TRGSRC_COUNT
} adc_pic32mx_adc_trgsrc_t;

typedef enum {
    ADC_PIC32MX_ADC_LVL_EDGE    = 0,
    ADC_PIC32MX_ADC_LVL_HIGH    = 1,
    ADC_PIC32MX_ADC_LVL_COUNT
} adc_pic32mx_adc_lvl_t;

struct adc {
    /*@temp@*/ struct adc_pic32mx *parent;
    size_t channel;
    size_t sar;
    adc_pic32mx_adc_trgsrc_t trgsrc;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

struct adc_pic32mx_adc_settings {
    adc_pic32mx_adc_im_t im;            /* input mode */
    unsigned long adcdiv;               /* clock divisor (2-254) */
    adc_pic32mx_adc_selres_t selres;    /* resolution select */
    unsigned long samc;                 /* sample time bits (2-1025) */
    adc_pic32mx_adc_trgsrc_t trgsrc;    /* trigger source */
    adc_pic32mx_adc_lvl_t lvl;          /* trigger edge */
};

int adc_pic32mx_adc_init(/*@out@*/ struct adc *ctx, struct adc_pic32mx *parent, size_t channel);
int adc_pic32mx_adc_setup(struct adc *ctx, const struct adc_pic32mx_adc_settings *settings);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 *
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

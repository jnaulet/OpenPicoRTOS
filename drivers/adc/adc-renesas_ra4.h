#ifndef ADC_RENESAS_RA4_H
#define ADC_RENESAS_RA4_H

#include "adc.h"
#include <stdbool.h>

#define ADC_RENESAS_RA4_CHANNEL_COUNT 28

struct ADC_RENESAS_RA4;

typedef enum {
    ADC_RENESAS_RA4_ADPRC_12BIT,
    ADC_RENESAS_RA4_ADPRC_14BIT,
    ADC_RENESAS_RA4_ADPRC_COUNT
} adc_renesas_ra4_adprc_t;

typedef enum {
    ADC_RENESAS_RA4_ADCS_SINGLE     = 0,
    ADC_RENESAS_RA4_ADCS_GROUP      = 1,
    ADC_RENESAS_RA4_ADCS_CONTINUOUS = 2,
    ADC_RENESAS_RA4_ADCS_COUNT
} adc_renesas_ra4_adcs_t;

struct adc_renesas_ra4_settings {
    adc_renesas_ra4_adprc_t adprc;
    adc_renesas_ra4_adcs_t adcs;
};

struct adc_renesas_ra4 {
    /*@temp@*/ struct ADC_RENESAS_RA4 *base;
    bool status[ADC_RENESAS_RA4_CHANNEL_COUNT];
    adc_renesas_ra4_adcs_t adcs;
};

int adc_renesas_ra4_init(/*@out@*/ struct adc_renesas_ra4 *ctx, int base);
int adc_renesas_ra4_setup(struct adc_renesas_ra4 *ctx, const struct adc_renesas_ra4_settings *settings);

struct adc {
    /*@temp@*/ struct adc_renesas_ra4 *parent;
    size_t channel;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_renesas_ra4_adc_init(/*@out@*/ struct adc *ctx,
                             struct adc_renesas_ra4 *parent,
                             size_t channel);

/* Implements:
 * int adc_setup(struct adc *ctx, const struct adc_settings *settings);
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

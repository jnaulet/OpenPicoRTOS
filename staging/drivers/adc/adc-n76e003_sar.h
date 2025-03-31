#ifndef ADC_N76E003_SAR_H
#define ADC_N76E003_SAR_H

#include "adc.h"

#define ADC_N76E003_SAR_CHANNEL_COUNT 8

struct adc_n76e003 {
    int dummy;
};

int adc_n76e003_init(/*@out@*/ struct adc_n76e003 *ctx);

struct adc {
    unsigned char channel;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_n76e003_adc_init(/*@out@*/ struct adc *ctx, size_t channel);

/*
 * Implements:
 * int adc_setup(struct adc *ctx, const struct adc_settings *settings);
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

#ifndef ADC_CH552_H
#define ADC_CH552_H

#include "adc.h"

#define ADC_CH552_CHANNEL_COUNT 4

struct adc_ch552 {
    int dummy;
};

int adc_ch522_init(/*@out@*/ struct adc_ch552 *ctx);

struct adc {
    unsigned char channel;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_ch552_init(/*@out@*/ struct adc *ctx, size_t channel);

/*
 * Implements:
 * int adc_setup(struct adc *ctx, const struct adc_settings *settings);
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

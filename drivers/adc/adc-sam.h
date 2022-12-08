#ifndef ADC_SAM_H
#define ADC_SAM_H

#include "adc.h"
#include <stddef.h>

struct ADC_SAM;

#define ADC_SAM_CHANNEL_COUNT 16

struct adc_sam {
    /*@temp@*/ struct ADC_SAM *base;
};

int adc_sam_init(/*@out@*/ struct adc_sam *ctx, struct ADC_SAM *base);

struct adc {
    /*@temp@*/ struct adc_sam *adc;
    size_t channel;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_sam_adc_init(/*@out@*/ struct adc *ctx, struct adc_sam *adc, size_t channel);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 *
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

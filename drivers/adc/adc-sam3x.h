#ifndef ADC_SAM3X_H
#define ADC_SAM3X_H

#include "adc.h"
#include <stddef.h>

struct ADC_SAM3X;

#define ADC_SAM3X_CHANNEL_COUNT 16

struct adc_sam3x {
    /*@temp@*/ struct ADC_SAM3X *base;
};

int adc_sam3x_init(/*@out@*/ struct adc_sam3x *ctx, struct ADC_SAM3X *base);

struct adc {
    /*@temp@*/ struct adc_sam3x *parent;
    size_t channel;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_sam3x_adc_init(/*@out@*/ struct adc *ctx, struct adc_sam3x *parent, size_t channel);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 *
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

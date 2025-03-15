#ifndef ADC_MC9S08PTXX_H
#define ADC_MC9S08PTXX_H

#include "adc.h"

#define ADC_MC9S08PTXX_CHANNEL_COUNT 32

struct ADC_MC9S08PTXX;

struct adc_mc9s08ptxx {
    /*@temp@*/ struct ADC_MC9S08PTXX *base;
};

typedef enum {
    ADC_MC9S08PTXX_MODE_8BIT    = 0,
    ADC_MC9S08PTXX_MODE_10BIT   = 1,
    ADC_MC9S08PTXX_MODE_12BIT   = 2,
    ADC_MC9S08PTXX_MODE_COUNT
} adc_mc9s08ptxx_mode_t;

struct adc_mc9s08ptxx_settings {
    adc_mc9s08ptxx_mode_t mode;
};

int adc_mc9s08ptxx_init(/*@out@*/ struct adc_mc9s08ptxx *ctx, int base);
int adc_mc9s08ptxx_setup(struct adc_mc9s08ptxx *ctx, const struct adc_mc9s08ptxx_settings *settings);

/* channels */

struct adc {
    /*@temp@*/ struct adc_mc9s08ptxx *parent;
    size_t channel;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_mc9s08ptxx_adc_init(/*@out@*/ struct adc *ctx, struct adc_mc9s08ptxx *parent, size_t channel);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 *
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

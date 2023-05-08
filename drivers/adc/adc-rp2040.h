#ifndef ADC_RP2040_H
#define ADC_RP2040_H

#include "adc.h"

struct ADC_RP2040;

#define ADC_RP2040_CHANNEL_TEMP_SENSOR 4

struct adc_rp2040 {
    /*@temp@*/ struct ADC_RP2040 *base;
};

int adc_rp2040_init(/*@out@*/ struct adc_rp2040 *ctx, int base);

#define ADC_RP2040_CHANNEL_COUNT 5

typedef enum {
    ADC_RP2040_ADC_STATE_IDLE,
    ADC_RP2040_ADC_STATE_ACQ,
    ADC_RP2040_ADC_STATE_COUNT
} adc_rp2040_adc_state_t;

struct adc {
    /*@temp@*/ struct adc_rp2040 *parent;
    size_t channel;
    adc_rp2040_adc_state_t state;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_rp2040_adc_init(/*@out@*/ struct adc *ctx, struct adc_rp2040 *parent, size_t channel);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 * int adc_read(struct adc *ctx, adc_sample_t *data);
 * int adc_read_multiple(struct adc *ctx, adc_sample_t *data, size_t n);
 */

#endif

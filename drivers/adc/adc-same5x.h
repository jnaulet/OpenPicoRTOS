#ifndef ADC_SAME5X_H
#define ADC_SAME5X_H

#include "adc.h"

struct ADC_SAME5X;

typedef enum {
    ADC_SAME5X_CHANNEL_AIN0             = 0,
    ADC_SAME5X_CHANNEL_AIN1             = 1,
    ADC_SAME5X_CHANNEL_AIN2             = 2,
    ADC_SAME5X_CHANNEL_AIN3             = 3,
    ADC_SAME5X_CHANNEL_AIN4             = 4,
    ADC_SAME5X_CHANNEL_AIN5             = 5,
    ADC_SAME5X_CHANNEL_AIN6             = 6,
    ADC_SAME5X_CHANNEL_AIN7             = 7,
    ADC_SAME5X_CHANNEL_AIN8             = 8,
    ADC_SAME5X_CHANNEL_AIN9             = 9,
    ADC_SAME5X_CHANNEL_AIN10            = 10,
    ADC_SAME5X_CHANNEL_AIN11            = 11,
    ADC_SAME5X_CHANNEL_AIN12            = 12,
    ADC_SAME5X_CHANNEL_AIN13            = 13,
    ADC_SAME5X_CHANNEL_AIN14            = 14,
    ADC_SAME5X_CHANNEL_AIN15            = 15,
    ADC_SAME5X_CHANNEL_SCALEDCOREVCC    = 24,
    ADC_SAME5X_CHANNEL_SCALEDVBAT       = 25,
    ADC_SAME5X_CHANNEL_SCALEDIOVCC      = 26,
    ADC_SAME5X_CHANNEL_BANDGAP          = 27,
    ADC_SAME5X_CHANNEL_PTAT             = 28,
    ADC_SAME5X_CHANNEL_CTAT             = 29,
    ADC_SAME5X_CHANNEL_DAC0             = 30,
    ADC_SAME5X_CHANNEL_COUNT
} adc_same5x_channel_t;

typedef enum {
    ADC_SAME5X_STATE_IDLE,
    ADC_SAME5X_STATE_ACQ,
    ADC_SAME5X_STATE_COUNT
} adc_same5x_state_t;

struct adc {
    /*@temp@*/ struct ADC_SAME5X *base;
    adc_same5x_channel_t channel;
    adc_same5x_state_t state;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_same5x_init(/*@out@*/ struct adc *ctx, struct ADC_SAME5X *base, adc_same5x_channel_t channel);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 *
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

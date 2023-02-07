#ifndef ADC_ATTINY1X_H
#define ADC_ATTINY1X_H

#include "adc.h"
#include "clock.h"
#include <stdbool.h>

struct ADC_ATTINY1X;

typedef enum {
    ADC_ATTINY1X_STATE_IDLE,
    ADC_ATTINY1X_STATE_ACQ,
    ADC_ATTINY1X_STATE_COUNT
} adc_attiny1x_state_t;

struct adc_attiny1x {
    /*@temp@*/ struct ADC_ATTINY1X *base;
    clock_id_t clkid;
    adc_attiny1x_state_t state;
};

/* ADC ATTINY1X settings definition
 * (forget about optional stuff for the moment) */

typedef enum {
    ADC_ATTINY1X_RESSEL_10BIT   = 0,
    ADC_ATTINY1X_RESSEL_8BIT    = 1,
    ADC_ATTINY1X_RESSEL_COUNT
} adc_attiny1x_ressel_t;

typedef enum {
    ADC_ATTINY1X_REFSEL_INTERNAL    = 0,
    ADC_ATTINY1X_REFSEL_VDD         = 1,
    ADC_ATTINY1X_REFSEL_VREFA       = 2,
    ADC_ATTINY1X_REFSEL_COUNT
} adc_attiny1x_refsel_t;

#define ADC_ATTINY1X_CLK_FREQ_MIN 50000
#define ADC_ATTINY1X_CLK_FREQ_MAX 1500000

struct adc_attiny1x_settings {
    adc_attiny1x_ressel_t ressel;
    adc_attiny1x_refsel_t refsel;
    /* According to doc, this should be between 50Khz & 1.5Mhz for 10bit conversions */
    unsigned long clk_freq; /* Warning: this will be approximative */
};

int adc_attiny1x_init(/*@out@*/ struct adc_attiny1x *ctx, struct ADC_ATTINY1X *base, clock_id_t clkid);
int adc_attiny1x_setup(struct adc_attiny1x *ctx, struct adc_attiny1x_settings *settings);

/* ADC channels */

typedef enum {
    ADC_ATTINY1X_MUXPOS_AIN0,
    ADC_ATTINY1X_MUXPOS_AIN1,
    ADC_ATTINY1X_MUXPOS_AIN2,
    ADC_ATTINY1X_MUXPOS_AIN3,
    ADC_ATTINY1X_MUXPOS_AIN4,
    ADC_ATTINY1X_MUXPOS_AIN5,
    ADC_ATTINY1X_MUXPOS_AIN6,
    ADC_ATTINY1X_MUXPOS_AIN7,
    ADC_ATTINY1X_MUXPOS_AIN8,
    ADC_ATTINY1X_MUXPOS_AIN9,
    ADC_ATTINY1X_MUXPOS_AIN10,
    ADC_ATTINY1X_MUXPOS_AIN11,
    ADC_ATTINY1X_MUXPOS_DAC0,
    ADC_ATTINY1X_MUXPOS_INTREF,
    ADC_ATTINY1X_MUXPOS_TEMPSENSE,
    ADC_ATTINY1X_MUXPOS_GND,
    ADC_ATTINY1X_MUXPOS_COUNT
} adc_attiny1x_muxpos_t;

struct adc {
    /*@temp@*/ struct adc_attiny1x *adc;
    adc_attiny1x_muxpos_t muxpos;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_attiny1x_adc_init(/*@out@*/ struct adc *ctx, struct adc_attiny1x *adc, adc_attiny1x_muxpos_t muxpos);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 *
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

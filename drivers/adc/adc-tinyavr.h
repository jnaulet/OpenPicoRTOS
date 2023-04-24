#ifndef ADC_TINYAVR_H
#define ADC_TINYAVR_H

#include "adc.h"
#include "clock.h"
#include <stdbool.h>

struct ADC_TINYAVR;

typedef enum {
    ADC_TINYAVR_STATE_IDLE,
    ADC_TINYAVR_STATE_ACQ,
    ADC_TINYAVR_STATE_COUNT
} adc_tinyavr_state_t;

struct adc_tinyavr {
    /*@temp@*/ struct ADC_TINYAVR *base;
    clock_id_t clkid;
    adc_tinyavr_state_t state;
};

/* ADC TINYAVR settings definition
 * (forget about optional stuff for the moment) */

typedef enum {
    ADC_TINYAVR_RESSEL_10BIT    = 0,
    ADC_TINYAVR_RESSEL_8BIT     = 1,
    ADC_TINYAVR_RESSEL_COUNT
} adc_tinyavr_ressel_t;

typedef enum {
    ADC_TINYAVR_REFSEL_INTERNAL = 0,
    ADC_TINYAVR_REFSEL_VDD      = 1,
    ADC_TINYAVR_REFSEL_VREFA    = 2,
    ADC_TINYAVR_REFSEL_COUNT
} adc_tinyavr_refsel_t;

#define ADC_TINYAVR_CLK_FREQ_MIN 50000
#define ADC_TINYAVR_CLK_FREQ_MAX 1500000

struct adc_tinyavr_settings {
    adc_tinyavr_ressel_t ressel;
    adc_tinyavr_refsel_t refsel;
    /* According to doc, this should be between 50Khz & 1.5Mhz for 10bit conversions */
    unsigned long clk_freq; /* Warning: this will be approximative */
};

int adc_tinyavr_init(/*@out@*/ struct adc_tinyavr *ctx, struct ADC_TINYAVR *base, clock_id_t clkid);
int adc_tinyavr_setup(struct adc_tinyavr *ctx, struct adc_tinyavr_settings *settings);

/* ADC channels */

typedef enum {
    ADC_TINYAVR_MUXPOS_AIN0,
    ADC_TINYAVR_MUXPOS_AIN1,
    ADC_TINYAVR_MUXPOS_AIN2,
    ADC_TINYAVR_MUXPOS_AIN3,
    ADC_TINYAVR_MUXPOS_AIN4,
    ADC_TINYAVR_MUXPOS_AIN5,
    ADC_TINYAVR_MUXPOS_AIN6,
    ADC_TINYAVR_MUXPOS_AIN7,
    ADC_TINYAVR_MUXPOS_AIN8,
    ADC_TINYAVR_MUXPOS_AIN9,
    ADC_TINYAVR_MUXPOS_AIN10,
    ADC_TINYAVR_MUXPOS_AIN11,
    ADC_TINYAVR_MUXPOS_DAC0,
    ADC_TINYAVR_MUXPOS_INTREF,
    ADC_TINYAVR_MUXPOS_TEMPSENSE,
    ADC_TINYAVR_MUXPOS_GND,
    ADC_TINYAVR_MUXPOS_COUNT
} adc_tinyavr_muxpos_t;

struct adc {
    /*@temp@*/ struct adc_tinyavr *parent;
    adc_tinyavr_muxpos_t muxpos;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_tinyavr_adc_init(/*@out@*/ struct adc *ctx, struct adc_tinyavr *parent, adc_tinyavr_muxpos_t muxpos);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 *
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

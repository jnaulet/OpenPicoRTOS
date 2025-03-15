#ifndef ADC_TI_TYPE4_H
#define ADC_TI_TYPE4_H

/* ADC driver for TI Ti_Type4 (C2000) */

#include "adc.h"
#include <stdint.h>
#include <stddef.h>

/* limits */
#define ADC_TI_TYPE4_CHANNEL_COUNT 16
#define ADC_TI_TYPE4_SAMPLES_MAX   16 /* oversampling, up to 16 */

typedef enum {
    ADC_TI_TYPE4_RESOLUTION_12BIT   = 0,
    ADC_TI_TYPE4_RESOLUTION_16BIT   = 1,
    ADC_TI_TYPE4_RESOLUTION_COUNT
} adc_ti_type4_resolution_t;

#define ADC_TI_TYPE4_ACQPS_MIN_12BIT 14     /* FIXME: arbitrary */
#define ADC_TI_TYPE4_ACQPS_MIN_16BIT 63     /* FIXME: arbitrary */

typedef enum {
    ADC_TI_TYPE4_SIGMODE_SINGLE         = 0,
    ADC_TI_TYPE4_SIGMODE_DIFFERENTIAL   = 1,
    ADC_TI_TYPE4_SIGMODE_COUNT
} adc_ti_type4_sigmode_t;

typedef enum {
    ADC_TI_TYPE4_PRESCALE_1_0       = 0,
    ADC_TI_TYPE4_PRESCALE_INVALID   = 1,
    ADC_TI_TYPE4_PRESCALE_2_0       = 2,
    ADC_TI_TYPE4_PRESCALE_2_5       = 3,
    ADC_TI_TYPE4_PRESCALE_3_0       = 4,
    ADC_TI_TYPE4_PRESCALE_3_5       = 5,
    ADC_TI_TYPE4_PRESCALE_4_0       = 6,
    ADC_TI_TYPE4_PRESCALE_4_5       = 7,
    ADC_TI_TYPE4_PRESCALE_5_0       = 8,
    ADC_TI_TYPE4_PRESCALE_5_5       = 9,
    ADC_TI_TYPE4_PRESCALE_6_0       = 10,
    ADC_TI_TYPE4_PRESCALE_6_5       = 11,
    ADC_TI_TYPE4_PRESCALE_7_0       = 12,
    ADC_TI_TYPE4_PRESCALE_7_5       = 13,
    ADC_TI_TYPE4_PRESCALE_8_0       = 14,
    ADC_TI_TYPE4_PRESCALE_8_5       = 15,
    ADC_TI_TYPE4_PRESCALE_COUNT,
} adc_ti_type4_prescale_t;

/* calibration */

#define ADCA_CAL_INL_ADDR 0x703b4ul
#define ADCB_CAL_INL_ADDR 0x703b2ul
#define ADCC_CAL_INL_ADDR 0x703b0ul
#define ADCD_CAL_INL_ADDR 0x703aeul

typedef enum {
    ADC_TI_TYPE4_TRIM_OFF_INDEX_ADCA    = 0,
    ADC_TI_TYPE4_TRIM_OFF_INDEX_ADCB    = 1,
    ADC_TI_TYPE4_TRIM_OFF_INDEX_ADCC    = 2,
    ADC_TI_TYPE4_TRIM_OFF_INDEX_ADCD    = 3,
    ADC_TI_TYPE4_TRIM_OFF_INDEX_COUNT
} adc_ti_type4_trim_off_index_t;

struct ADC_REGS;
struct ADC_RESULT_REGS;

struct adc_ti_type4 {
    /*@temp@*/ struct ADC_REGS *base;
    /*@temp@*/ struct ADC_RESULT_REGS *result;
    size_t acqps_min;
    /* counters */
    size_t sample_count;
};

struct adc_ti_type4_settings {
    adc_ti_type4_resolution_t resolution;
    adc_ti_type4_sigmode_t sigmode;
    adc_ti_type4_prescale_t prescale;
    adc_ti_type4_trim_off_index_t trim_off_index;
};

int adc_ti_type4_init(/*@out@*/ struct adc_ti_type4 *ctx,
                      int base, int result_base,
                      unsigned long cal_inl_addr);

int adc_ti_type4_setup(struct adc_ti_type4 *ctx, const struct adc_ti_type4_settings *settings);

/* channels */

typedef enum {
    ADC_TI_TYPE4_INTFLG_1   = 0,
    ADC_TI_TYPE4_INTFLG_2   = 1,
    ADC_TI_TYPE4_INTFLG_3   = 2,
    ADC_TI_TYPE4_INTFLG_4   = 3,
    ADC_TI_TYPE4_INTFLG_COUNT
} adc_ti_type4_intflg_t;

typedef enum {
    ADC_TI_TYPE4_TRIG_SOFT          = 0,
    ADC_TI_TYPE4_TRIG_CPU1_TINT0    = 1,
    ADC_TI_TYPE4_TRIG_CPU1_TINT1    = 2,
    ADC_TI_TYPE4_TRIG_CPU1_TINT2    = 3,
    ADC_TI_TYPE4_TRIG_GPIO          = 4,
    ADC_TI_TYPE4_TRIG_EPWM1_SOCA    = 5,
    ADC_TI_TYPE4_TRIG_EPWM1_SOCB    = 6,
    ADC_TI_TYPE4_TRIG_EPWM2_SOCA    = 7,
    ADC_TI_TYPE4_TRIG_EPWM2_SOCB    = 8,
    ADC_TI_TYPE4_TRIG_EPWM3_SOCA    = 9,
    ADC_TI_TYPE4_TRIG_EPWM3_SOCB    = 10,
    ADC_TI_TYPE4_TRIG_EPWM4_SOCA    = 11,
    ADC_TI_TYPE4_TRIG_EPWM4_SOCB    = 12,
    ADC_TI_TYPE4_TRIG_EPWM5_SOCA    = 13,
    ADC_TI_TYPE4_TRIG_EPWM5_SOCB    = 14,
    ADC_TI_TYPE4_TRIG_EPWM6_SOCA    = 15,
    ADC_TI_TYPE4_TRIG_EPWM6_SOCB    = 16,
    ADC_TI_TYPE4_TRIG_EPWM7_SOCA    = 17,
    ADC_TI_TYPE4_TRIG_EPWM7_SOCB    = 18,
    ADC_TI_TYPE4_TRIG_EPWM8_SOCA    = 19,
    ADC_TI_TYPE4_TRIG_EPWM8_SOCB    = 20,
    ADC_TI_TYPE4_TRIG_EPWM9_SOCA    = 21,
    ADC_TI_TYPE4_TRIG_EPWM9_SOCB    = 22,
    ADC_TI_TYPE4_TRIG_EPWM10_SOCA   = 23,
    ADC_TI_TYPE4_TRIG_EPWM10_SOCB   = 24,
    ADC_TI_TYPE4_TRIG_EPWM11_SOCA   = 25,
    ADC_TI_TYPE4_TRIG_EPWM11_SOCB   = 26,
    ADC_TI_TYPE4_TRIG_EPWM12_SOCA   = 27,
    ADC_TI_TYPE4_TRIG_EPWM12_SOCB   = 28,
    ADC_TI_TYPE4_TRIG_CPU2_TINT0    = 29,
    ADC_TI_TYPE4_TRIG_CPU2_TINT1    = 30,
    ADC_TI_TYPE4_TRIG_CPU2_TINT2    = 31,
    ADC_TI_TYPE4_TRIG_EPWM13_SOCA   = 32,
    ADC_TI_TYPE4_TRIG_EPWM13_SOCB   = 33,
    ADC_TI_TYPE4_TRIG_EPWM14_SOCA   = 34,
    ADC_TI_TYPE4_TRIG_EPWM14_SOCB   = 35,
    ADC_TI_TYPE4_TRIG_EPWM15_SOCA   = 36,
    ADC_TI_TYPE4_TRIG_EPWM15_SOCB   = 37,
    ADC_TI_TYPE4_TRIG_EPWM16_SOCA   = 38,
    ADC_TI_TYPE4_TRIG_EPWM16_SOCB   = 39,
    ADC_TI_TYPE4_TRIG_COUNT
} adc_ti_type4_trig_t;

struct adc_ti_type4_adc_settings {
    adc_ti_type4_trig_t trig;
    uint16_t acqps;
};

struct adc {
    /*@temp@*/ struct adc_ti_type4 *parent;
    size_t channel;
    size_t soc_start;
    size_t soc_end;
    adc_ti_type4_intflg_t intflg;
    adc_ti_type4_trig_t trig;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_ti_type4_adc_init(/*@out@*/ struct adc *ctx,
                          struct adc_ti_type4 *parent,
                          size_t channel,
                          size_t sample_count,
                          adc_ti_type4_intflg_t intflg);

int adc_ti_type4_adc_setup(struct adc *ctx,
                           const struct adc_ti_type4_adc_settings *settings);

/* Runtime calls:
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

#ifndef ADC_AVR_H
#define ADC_AVR_H

#include "adc.h"
#include <stdbool.h>

struct ADC_AVR;

typedef enum {
    ADC_AVR_STATE_IDLE,
    ADC_AVR_STATE_ACQ,
    ADC_AVR_STATE_COUNT
} adc_avr_state_t;

struct adc_avr {
    /*@temp@*/ struct ADC_AVR *base;
    adc_avr_state_t state;
};

typedef enum {
    ADC_AVR_PRESCALER_2     = 0,
    ADC_AVR_PRESCALER_4     = 2,
    ADC_AVR_PRESCALER_8     = 3,
    ADC_AVR_PRESCALER_16    = 4,
    ADC_AVR_PRESCALER_32    = 5,
    ADC_AVR_PRESCALER_64    = 6,
    ADC_AVR_PRESCALER_128   = 7,
    ADC_AVR_PRESCALER_COUNT
} adc_avr_prescaler_t;

typedef enum {
    ADC_AVR_REFS_AREF           = 0,
    ADC_AVR_REFS_AVCC           = 1,
    ADC_AVR_REFS_INTERNAL_1_1V  = 2,
    ADC_AVR_REFS_INTERNAL_2_56V = 3,
    ADC_AVR_REFS_COUNT
} adc_avr_refs_t;

/* available channels */
#define ADC_AVR_CHANNEL_0            0
#define ADC_AVR_CHANNEL_1            1
#define ADC_AVR_CHANNEL_2            2
#define ADC_AVR_CHANNEL_3            3
#define ADC_AVR_CHANNEL_4            4
#define ADC_AVR_CHANNEL_5            5
#define ADC_AVR_CHANNEL_6            6
#define ADC_AVR_CHANNEL_7            7
#define ADC_AVR_CHANNEL_P0_N0_10x    8
#define ADC_AVR_CHANNEL_P1_N0_10x    9
#define ADC_AVR_CHANNEL_P0_N0_200x   10
#define ADC_AVR_CHANNEL_P1_N0_200x   11
#define ADC_AVR_CHANNEL_P2_N2_10x    12
#define ADC_AVR_CHANNEL_P3_N2_10x    13
#define ADC_AVR_CHANNEL_P2_N2_200x   14
#define ADC_AVR_CHANNEL_P3_N2_200x   15
#define ADC_AVR_CHANNEL_P0_N1        16
#define ADC_AVR_CHANNEL_P1_N1        17
#define ADC_AVR_CHANNEL_P2_N1        18
#define ADC_AVR_CHANNEL_P3_N1        19
#define ADC_AVR_CHANNEL_P4_N1        20
#define ADC_AVR_CHANNEL_P5_N1        21
#define ADC_AVR_CHANNEL_P6_N1        22
#define ADC_AVR_CHANNEL_P7_N1        23
#define ADC_AVR_CHANNEL_P0_N2        24
#define ADC_AVR_CHANNEL_P1_N2        25
#define ADC_AVR_CHANNEL_P2_N2        26
#define ADC_AVR_CHANNEL_P3_N2        27
#define ADC_AVR_CHANNEL_P4_N2        28
#define ADC_AVR_CHANNEL_P5_N2        29
#define ADC_AVR_CHANNEL_1_1V         30
#define ADC_AVR_CHANNEL_0V           31
#define ADC_AVR_CHANNEL_8            32
#define ADC_AVR_CHANNEL_9            33
#define ADC_AVR_CHANNEL_10           34
#define ADC_AVR_CHANNEL_11           35
#define ADC_AVR_CHANNEL_12           36
#define ADC_AVR_CHANNEL_13           37
#define ADC_AVR_CHANNEL_14           38
#define ADC_AVR_CHANNEL_15           39
#define ADC_AVR_CHANNEL_P8_N8_10x    40
#define ADC_AVR_CHANNEL_P9_N8_10x    41
#define ADC_AVR_CHANNEL_P8_N8_200x   42
#define ADC_AVR_CHANNEL_P9_N8_200x   43
#define ADC_AVR_CHANNEL_P10_N10_10x  44
#define ADC_AVR_CHANNEL_P11_N10_10x  45
#define ADC_AVR_CHANNEL_P10_N10_200x 46
#define ADC_AVR_CHANNEL_P11_N10_200x 47
#define ADC_AVR_CHANNEL_P8_N9        48
#define ADC_AVR_CHANNEL_P9_N9        49
#define ADC_AVR_CHANNEL_P10_N9       50
#define ADC_AVR_CHANNEL_P11_N9       51
#define ADC_AVR_CHANNEL_P12_N9       52
#define ADC_AVR_CHANNEL_P13_N9       53
#define ADC_AVR_CHANNEL_P14_N9       54
#define ADC_AVR_CHANNEL_P15_N9       55
#define ADC_AVR_CHANNEL_P8_N10       56
#define ADC_AVR_CHANNEL_P9_N10       57
#define ADC_AVR_CHANNEL_P10_N10      58
#define ADC_AVR_CHANNEL_P11_N10      59
#define ADC_AVR_CHANNEL_P12_N10      60
#define ADC_AVR_CHANNEL_P13_N10      61

typedef enum {
    ADC_AVR_TRIG_MANUAL             = -1,
    ADC_AVR_TRIG_FREE_RUNNING_MODE  = 0,
    ADC_AVR_TRIG_ANALOG_COMPARATOR  = 1,
    ADC_AVR_TRIG_EXT_INT_REQ_0      = 2,
    ADC_AVR_TRIG_TC0_CMP_MATCH_A    = 3,
    ADC_AVR_TRIG_TC0_OVF            = 4,
    ADC_AVR_TRIG_TC1_CMP_MATCH_B    = 5,
    ADC_AVR_TRIG_TC1_OVF            = 6,
    ADC_AVR_TRIG_TC1_CAP_EVT        = 7,
    ADC_AVR_TRIG_COUNT
} adc_avr_trig_t;

/* ADC AVR settings definition */

struct adc_avr_settings {
    adc_avr_prescaler_t prescaler;
    adc_avr_refs_t refs;
    adc_avr_trig_t trig;
    bool left_adjust_result;
};

int adc_avr_init(/*@out@*/ struct adc_avr *ctx, int base);
int adc_avr_setup(struct adc_avr *ctx, struct adc_avr_settings *settings);

/* channels */

struct adc {
    /*@temp@*/ struct adc_avr *parent;
    size_t channel;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_avr_adc_init(/*@out@*/ struct adc *ctx, struct adc_avr *parent, size_t channel);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 *
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

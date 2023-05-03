#ifndef ADC_NXP_SAR_H
#define ADC_NXP_SAR_H

#include "adc.h"
#include <stdbool.h>

struct ADC_NXP_SAR;

struct adc_nxp_sar {
    /*@temp@*/ struct ADC_NXP_SAR *base;
};

typedef enum {
    ADC_NXP_SAR_TRIG_NORMAL,
    ADC_NXP_SAR_TRIG_INJECTED,
    ADC_NXP_SAR_TRIG_BCTU,
    ADC_NXP_SAR_TRIG_COUNT
} adc_nxp_sar_trig_t; /* per-channel */

/* NORMAL: One-Shot / Scan */
/* INJECTED: One-Shot */
/* BCTU modes: control and trigger */
/* threshold: per channel */
/* presampling: per group of channels 0, 1, 2 */

struct adc_nxp_sar_settings {
    bool overwrite;
    bool align_left;                            /* TBD */
    bool scan_mode;
    bool injection_ext_trigger_en;              /* TBD */
    bool injection_trigger_edge_sel;            /* TBD */
    bool injection_start;                       /* TBD */
    bool crosstrigger_unit_en;                  /* TBD */
    bool crosstrigger_unit_trigger_mode;        /* TBD */
    bool analog_clock_freq_equals_bus_clock;    /* TBD */
    bool auto_clock_off_en;                     /* TBD */
    /* TODO: presampling */
};

int adc_nxp_sar_init(/*@out@*/ struct adc_nxp_sar *ctx, int base);
int adc_nxp_sar_setup(struct adc_nxp_sar *ctx, struct adc_nxp_sar_settings *settings);

/* channel */

struct adc {
    /*@temp@*/ struct adc_nxp_sar *parent;
    size_t channel;
    size_t index;
    uint32_t mask;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_nxp_sar_adc_init(/*@out@*/ struct adc *ctx, struct adc_nxp_sar *parent, size_t channel);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

#ifndef ADC_GD32VF103_H
#define ADC_GD32VF103_H

#include "adc.h"
#include "dma.h"

struct ADC_GD32VF103;

#define ADC_GD32VF103_CHANNEL_COUNT 18 /* account for tvsen on adc0 */
#define ADC_GD32VF103_RSQ_COUNT     16

typedef enum {
    ADC_GD32VF103_MODE_SINGLE,
    ADC_GD32VF103_MODE_CONTINUOUS,
    ADC_GD32VF103_MODE_SCAN_SINGLE,
    ADC_GD32VF103_MODE_SCAN_CONTINUOUS,
    /* ADC_GD32VF103_MODE_DISCONTINUOUS, */
    ADC_GD32VF103_MODE_COUNT
} adc_gd32vf103_mode_t;

struct adc_gd32vf103 {
    /*@temp@*/ struct ADC_GD32VF103 *base;
    adc_gd32vf103_mode_t mode;
    size_t rsq_count; /* ADC_RSQ0[RL] */
    /* For future scan mode use: */
    /*@temp@*/ struct dma *dma;
    struct dma_xfer xfer;
    uint32_t result[ADC_GD32VF103_RSQ_COUNT];
};

struct adc_gd32vf103_settings {
    adc_gd32vf103_mode_t mode;
    /*@temp@*/ /*@null@*/ struct dma *dma;
    bool tvsen; /* temp sensor */
    /* TODO, here: */
    /* conversion result threshold */
    /* data storage mode */
    /* sample time */
    /* external trigger */
    /* resolution */
    /* oversampling */
};

int adc_gd32vf103_init(/*@out@*/ struct adc_gd32vf103 *ctx, int base);
int adc_gd32vf103_setup(struct adc_gd32vf103 *ctx, const struct adc_gd32vf103_settings *settings);

/* to start coninous scan. FIXME */
int adc_gd32vf103_start(struct adc_gd32vf103 *ctx);

/* channels */

struct adc {
    /*@temp@*/ struct adc_gd32vf103 *parent;
    size_t channel;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
    /* scan mode */
    size_t rsq_index;
};

int adc_gd32vf103_adc_init(/*@out@*/ struct adc *ctx, struct adc_gd32vf103 *parent, size_t channel);

/* Implements:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

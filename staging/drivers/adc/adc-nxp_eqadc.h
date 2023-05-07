#ifndef ADC_NXP_EQADC_H
#define ADC_NXP_EQADC_H

#include "adc.h"
#include "dma.h"

#include <stdbool.h>
#include "picoRTOS_types.h"

struct ADC_NXP_EQADC;

#define ADC_NXP_EQADC_CHANNEL_COUNT 40

typedef enum {
    ADC_NXP_EQADC_MODE0_SINGLE_SCAN,
    ADC_NXP_EQADC_MODE0_CONTINUOUS_SCAN,
    ADC_NXP_EQADC_MODE0_COUNT
} adc_nxp_eqadc_mode0_t;

struct adc_nxp_eqadc {
    /*@temp@*/ struct ADC_NXP_EQADC *base;
    size_t channel_count;
    /* cache-page aligned buffers */
    uint32_t cmd[ADC_NXP_EQADC_CHANNEL_COUNT]
    __attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));
    uint32_t result[ADC_NXP_EQADC_CHANNEL_COUNT]
    __attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));
    /* mode0 only */
    adc_nxp_eqadc_mode0_t mode0;
    /* dma */
    /*@temp@*/ /*@null@*/ struct dma *fill;
    /*@temp@*/ /*@null@*/ struct dma *drain;
    struct dma_xfer fill_xfer;
    struct dma_xfer drain_xfer;
};

struct adc_nxp_eqadc_settings {
    adc_nxp_eqadc_mode0_t mode0;
    /*@temp@*/ struct dma *fill;
    /*@temp@*/ struct dma *drain;
};

int adc_nxp_eqadc_init(/*@out@*/ struct adc_nxp_eqadc *ctx, int base);
int adc_nxp_eqadc_setup(struct adc_nxp_eqadc *ctx, struct adc_nxp_eqadc_settings *settings);

struct adc {
    /*@temp@*/ struct adc_nxp_eqadc *parent;
    size_t channel;
    size_t channel_index;
    /* claibration */
    int multiplier;
    int divider;
    int offset;
};

typedef enum {
    ADC_NXP_EQADC_LST_2CYCLES,
    ADC_NXP_EQADC_LST_8CYCLES,
    ADC_NXP_EQADC_LST_64CYCLES,
    ADC_NXP_EQADC_LST_128CYCLES,
    ADC_NXP_EQADC_LST_COUNT
} adc_nxp_eqadc_lst_t;

struct adc_nxp_eqadc_adc_settings {
    adc_nxp_eqadc_lst_t lst;
    bool cal;
};

int adc_nxp_eqadc_adc_init(struct adc *ctx, struct adc_nxp_eqadc *parent, size_t channel);
int adc_nxp_eqadc_adc_setup(struct adc *ctx, struct adc_nxp_eqadc_adc_settings *settings);

/* Runtime calls:
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

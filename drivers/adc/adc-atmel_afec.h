#ifndef ADC_ATMEL_AFEC_H
#define ADC_ATMEL_AFEC_H

#include "adc.h"
#include "clock.h"
#include <stddef.h>

struct ADC_ATMEL_AFEC;

#define ADC_ATMEL_AFEC_SEQ_COUNT     12
#define ADC_ATMEL_AFEC_CHANNEL_COUNT 12

struct adc_atmel_afec {
    /*@temp@*/ struct ADC_ATMEL_AFEC *base;
    clock_id_t clkid;
    size_t seq;
};

int adc_atmel_afec_init(/*@out@*/ struct adc_atmel_afec *ctx, int base, clock_id_t clkid);
int adc_atmel_afec_protect(struct adc_atmel_afec *ctx);

struct adc {
    /*@temp@*/ struct adc_atmel_afec *parent;
    size_t channel;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

int adc_atmel_afec_adc_init(/*@out@*/ struct adc *ctx, struct adc_atmel_afec *parent, size_t channel);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, struct adc_settings *settings);
 *
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif

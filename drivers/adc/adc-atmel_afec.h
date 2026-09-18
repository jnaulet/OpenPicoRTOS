#ifndef ADC_ATMEL_AFEC_H
#define ADC_ATMEL_AFEC_H

#include "adc+safety.h"
#include "clock.h"

#include <stddef.h>
#include <stdbool.h>

struct ADC_ATMEL_AFEC;

#define ADC_ATMEL_AFEC_SEQ_COUNT     12
#define ADC_ATMEL_AFEC_CHANNEL_COUNT 12

typedef enum {
  ADC_ATMEL_AFEC_STATE_IDLE,
  ADC_ATMEL_AFEC_STATE_GATHER,
  ADC_ATMEL_AFEC_STATE_PUBLISH,
  ADC_ATMEL_AFEC_STATE_COUNT
} adc_atmel_afec_state_t;

struct adc_atmel_afec {
    /*@temp@*/ struct ADC_ATMEL_AFEC *base;
    clock_id_t clkid;
    size_t seq;
    size_t rshift;
    /* state machine */
    adc_atmel_afec_state_t state;
    uint32_t EOCx_mask;
    int CDR[ADC_ATMEL_AFEC_CHANNEL_COUNT];
};

int adc_atmel_afec_init(/*@out@*/ struct adc_atmel_afec *ctx, int base, clock_id_t clkid);
int adc_atmel_afec_protect(struct adc_atmel_afec *ctx);

typedef enum {
  ADC_ATMEL_AFEC_RES_NO_AVERAGE = 0,
  ADC_ATMEL_AFEC_RES_LOW_RES = 1, /* 10bit */
  ADC_ATMEL_AFEC_RES_OSR4 = 2,  /* 13bit */
  ADC_ATMEL_AFEC_RES_OSR16 = 3, /* 14bit */
  ADC_ATMEL_AFEC_RES_OSR64 = 4, /* 15bit */
  ADC_ATMEL_AFEC_RES_OSR256 = 5, /* 16bit */
  ADC_ATMEL_AFEC_RES_COUNT
} adc_atmel_afec_res_t;

struct adc_atmel_afec_settings {
  adc_atmel_afec_res_t res;
};

int adc_atmel_afec_setup(struct adc_atmel_afec *ctx, const struct adc_atmel_afec_settings *settings);

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
 * struct adc *adc_claim(struct adc *ctx);
 */

#endif

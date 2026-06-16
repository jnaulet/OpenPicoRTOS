#ifndef ADC_SAFETY_H
#define ADC_SAFETY_H

#include "adc.h"

/**
 * struct adc \***adc_claim**(**struct adc** \*<ins>ctx</ins>);
 * > Claims the adc channel <ins>ctx</ins> & its dependencies
 * ### NOTES
 * > This feature is related to MPU & will only have an effect if
 * > `CONFIG_MPU` is active.
 */
struct adc *adc_claim(/*@returned@*/ struct adc *ctx);

/* TBD:
 * int adc_selftest(struct adc *ctx);
 * int adc_readback(struct adc *ctx);
 */

#endif

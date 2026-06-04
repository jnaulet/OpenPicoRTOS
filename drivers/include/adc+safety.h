#ifndef ADC_SAFETY_H
#define ADC_SAFETY_H

#include "adc.h"

/* Function: adc_claim
 * Claims the adc for this process/thread (opt. MPU-related feature)
 *
 * Parameters:
 *  adc - A adc instance
 */
struct adc *adc_claim(/*@returned@*/ struct adc *ctx);

/* TBD:
 * int adc_selftest(struct adc *ctx);
 * int adc_readback(struct adc *ctx);
 */

#endif

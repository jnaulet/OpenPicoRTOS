#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <errno.h>
#include <stdint.h>
#include <stddef.h>

struct adc;

struct adc_settings {
    int multiplier; /* step 1 */
    int divider;    /* step 2 */
    int offset;     /* step 3 */
};

/* Function: adc_setup
 * Configures an ADC
 *
 * Parameters:
 *  ctx - A pointer to an ADC instance
 *  settings - The configuration parameters to use
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int adc_setup(struct adc *ctx, const struct adc_settings *settings);

/* Function: adc_read
 * Reads a value from the adc
 *
 * Parameters:
 *  ctx - An ADC instance
 *  data - a buffer where to store the read value
 *
 * Returns:
 * The number of values read (1) in case of success, -errno otherwise
 */
int adc_read(struct adc *ctx, int *data);

/* Function: adc_read_multiple
 * Reads multiple values from the adc (oversampling)
 *
 * Parameters:
 *  ctx - An ADC instance
 *  data - a buffer where to store the read values
 *  n - The number of values to read / size of the buffer
 *
 * Returns:
 * The number of values read in case of success, -errno otherwise
 */
int adc_read_multiple(struct adc *ctx, int *data, size_t n);

#endif

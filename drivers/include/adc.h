#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <errno.h>
#include <stdint.h>
#include <stddef.h>

struct adc;

/**
 * **struct adc_settings**;
 * > Simple transfer function parameters
 * ### NOTES
 * > `adc_read()` will apply these to the raw adc value as following:
 * > - (int)adc_value * <ins>multiplier</ins> / <ins>divider</ins> + <ins>offset</ins>
 */
struct adc_settings {
    int multiplier;
    int divider;
    int offset;
};

/**
 * int **adc_setup**(**struct adc** \*<ins>ctx</ins>,
 * const **struct adc_settings** \*<ins>settings</ins>);
 * > Configures an ADC channel <ins>ctx</ins>
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int adc_setup(struct adc *ctx, const struct adc_settings *settings);

/**
 * int **adc_read**(**struct adc** \*<ins>ctx</ins>, **int** \*<ins>data</ins>);
 * > Reads a value from the adc channel <ins>ctx</ins> & stores it in
 * > \*<ins>data</ins>
 * ### RETURN
 * > Returns the number of values read (1) if success, -errno otherwise
 */
int adc_read(struct adc *ctx, int *data);

/**
 * int **adc_read_multiple**(**struct adc** \*<ins>ctx</ins>,
 * **int** \*<ins>data</ins>, **size_t** <ins>n</ins>);
 * > Reads multiple values from the adc channel <ins>ctx</ins> &
 * > stores then in <ins>data</ins>[]
 * ### RETURN
 * > Returns the number of values read if success, -errno otherwise
 */
int adc_read_multiple(struct adc *ctx, int *data, size_t n);

#endif

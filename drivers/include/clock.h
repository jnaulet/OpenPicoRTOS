#ifndef HAL_CLOCK_H
#define HAL_CLOCK_H

#include <errno.h>

typedef long clock_freq_t;
typedef unsigned int clock_id_t;

/* Constants: Main system clock
 *
 * CLOCK_SYSCLK - Main system clock ID
 */
#define CLOCK_SYSCLK (clock_id_t)0 /* always */

/* Function: clock_get_freq
 * Gets a clock frequency (in hz)
 *
 * Parameters:
 *  clkid - Clock identifier
 *
 * Returns:
 * The requested clock frequency if success, -errno otherwise
 */
clock_freq_t clock_get_freq(clock_id_t clkid);

#endif

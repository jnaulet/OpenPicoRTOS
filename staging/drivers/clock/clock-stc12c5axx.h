#ifndef CLOCK_STC12CA5XX_H
#define CLOCK_STC12CA5XX_H

#include "clock.h"

#define CLOCK_STC12C5AXX_FSYS  CLOCK_SYSCLK
#define CLOCK_STC12C5AXX_T1    (clock_id_t)1
#define CLOCK_STC12C5AXX_BRT   (clock_id_t)2

typedef enum {
    CLOCK_STC12C5AXX_X1     = 0,
    CLOCK_STC12C5AXX_X12    = 1,
    CLOCK_STC12C5AXX_XCOUNT
} clock_stc12c5axx_x_t;

struct clock_settings {
    clock_stc12c5axx_x_t t1;
    clock_stc12c5axx_x_t brt;
};

int clock_stc12c5axx_init(struct clock_settings *settings);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

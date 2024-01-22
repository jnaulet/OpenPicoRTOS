#ifndef CLOCK_CY7C6801xA_H
#define CLOCK_CY7C6801xA_H

#include "clock.h"

#define CLOCK_CY7C6801XA_CPU CLOCK_SYSCLK

typedef enum {
    CLOCK_CY7C6801XA_CLKSPD_12MHZ,
    CLOCK_CY7C6801XA_CLKSPD_24MHZ,
    CLOCK_CY7C6801XA_CLKSPD_48MHZ,
    CLOCK_CY7C6801XA_CLKSPD_COUNT
} clock_cy7c6801xa_clkspd_t;

struct clock_settings {
    clock_cy7c6801xa_clkspd_t clkspd;
};

int clock_cy7c6801xa_init(struct clock_settings *settings);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

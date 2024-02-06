#ifndef CLOCK_N76E003_H
#define CLOCK_N76E003_H

#include "clock.h"

#define CLOCK_N76E003_FSYS  CLOCK_SYSCLK
#define CLOCK_N76E003_FOSC  (clock_id_t)1
#define CLOCK_N76E003_FECLK (clock_id_t)2
#define CLOCK_N76E003_FHIRC (clock_id_t)3
#define CLOCK_N76E003_FLIRC (clock_id_t)4

typedef enum {
    CLOCK_N76E003_FOSC_FHIRC    = 0,
    CLOCK_N76E003_FOSC_FECLK    = 1,
    CLOCK_N76E003_FOSC_FLIRC    = 2,
    CLOCK_N76E003_FOSC_COUNT
} clock_n76e003_fosc_t;

typedef enum {
    CLOCK_N76E003_FHIRC_16_0MHZ,
    CLOCK_N76E003_FHIRC_16_6MHZ,
    CLOCK_N76E003_FHIRC_COUNT
} clock_n76e003_fhirc_t;

#define CLOCK_N76E003_CLKDIV_MAX 510

struct clock_settings {
    clock_n76e003_fosc_t fosc;
    clock_n76e003_fhirc_t fhirc;
    unsigned long feclk; /* 0 is allowed */
    unsigned int clkdiv;
};

int clock_n76e003_init(struct clock_settings *settings);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

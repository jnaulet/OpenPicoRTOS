#ifndef CLOCK_NUMICRO_H
#define CLOCK_NUMICRO_H

#include "clock.h"

#define CLOCK_NUMICRO_FSYS  CLOCK_SYSCLK
#define CLOCK_NUMICRO_FOSC  (clock_id_t)1
#define CLOCK_NUMICRO_FECLK (clock_id_t)2
#define CLOCK_NUMICRO_FHIRC (clock_id_t)3
#define CLOCK_NUMICRO_FLIRC (clock_id_t)4

typedef enum {
    CLOCK_NUMICRO_FOSC_FHIRC    = 0,
    CLOCK_NUMICRO_FOSC_FECLK    = 1,
    CLOCK_NUMICRO_FOSC_FLIRC    = 2,
    CLOCK_NUMICRO_FOSC_COUNT
} clock_numicro_fosc_t;

typedef enum {
    CLOCK_NUMICRO_FHIRC_16_0MHZ,
    CLOCK_NUMICRO_FHIRC_16_6MHZ,
    CLOCK_NUMICRO_FHIRC_24_0MHZ,
    CLOCK_NUMICRO_FHIRC_COUNT
} clock_numicro_fhirc_t;

#define CLOCK_NUMICRO_CLKDIV_MAX 510

struct clock_settings {
    clock_numicro_fosc_t fosc;
    clock_numicro_fhirc_t fhirc;
    unsigned long feclk; /* 0 is allowed */
    unsigned int clkdiv;
};

int clock_numicro_init(struct clock_settings *settings);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

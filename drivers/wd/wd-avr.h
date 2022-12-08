#ifndef WD_AVR_H
#define WD_AVR_H

#include "wd.h"

struct WDT_AVR;

typedef enum {
    WD_AVR_CYCLES_2K    = 0,
    WD_AVR_CYCLES_4K    = 1,
    WD_AVR_CYCLES_8K    = 2,
    WD_AVR_CYCLES_16K   = 3,
    WD_AVR_CYCLES_32K   = 4,
    WD_AVR_CYCLES_64K   = 5,
    WD_AVR_CYCLES_128K  = 6,
    WD_AVR_CYCLES_256K  = 7,
    WD_AVR_CYCLES_512K  = 8,
    WD_AVR_CYCLES_1024K = 9,
    WD_AVR_CYCLES_COUNT
} wd_avr_cycles_t;

struct wd {
    /*@temp@*/ struct WDT_AVR *base;
    wd_avr_cycles_t cycles;
};

struct wd_avr_settings {
    wd_avr_cycles_t cycles;
};

int wd_avr_init(/*@out@*/ struct wd *ctx, struct WDT_AVR *base);
int wd_avr_setup(struct wd *ctx, struct wd_avr_settings *settings);

/* Runtime calls:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

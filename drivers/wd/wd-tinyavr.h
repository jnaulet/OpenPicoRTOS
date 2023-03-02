#ifndef WD_TINYAVR_H
#define WD_TINYAVR_H

#include "wd.h"
#include "clock.h"

#include <stdint.h>

struct WDT_TINYAVR;

struct wd {
    /*@temp@*/ struct WDT_TINYAVR *base;
    uint8_t ctrla;
};

typedef enum {
    WD_TINYAVR_PERIOD_OFF   = 0,
    WD_TINYAVR_PERIOD_8MS   = 1,
    WD_TINYAVR_PERIOD_16MS  = 2,
    WD_TINYAVR_PERIOD_31MS  = 3,
    WD_TINYAVR_PERIOD_63MS  = 4,
    WD_TINYAVR_PERIOD_125MS = 5,
    WD_TINYAVR_PERIOD_250MS = 6,
    WD_TINYAVR_PERIOD_500MS = 7,
    WD_TINYAVR_PERIOD_1S    = 8,
    WD_TINYAVR_PERIOD_2S    = 9,
    WD_TINYAVR_PERIOD_4S    = 10,
    WD_TINYAVR_PERIOD_8S    = 11,
    WD_TINYAVR_PERIOD_COUNT
} wd_tinyavr_period_t;

struct wd_tinyavr_settings {
    wd_tinyavr_period_t window;
    wd_tinyavr_period_t period;
};

int wd_tinyavr_init(/*@out@*/ struct wd *ctx, struct WDT_TINYAVR *base);
int wd_tinyavr_setup(struct wd *ctx, struct wd_tinyavr_settings *settings);

/* Runtime calls:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

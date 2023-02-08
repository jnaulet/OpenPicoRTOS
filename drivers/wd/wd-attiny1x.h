#ifndef WD_ATTINY1X_H
#define WD_ATTINY1X_H

#include "wd.h"
#include "clock.h"

#include <stdint.h>

struct WDT_ATTINY1X;

struct wd {
    /*@temp@*/ struct WDT_ATTINY1X *base;
    uint8_t ctrla;
};

typedef enum {
    WD_ATTINY1X_PERIOD_OFF      = 0,
    WD_ATTINY1X_PERIOD_8MS      = 1,
    WD_ATTINY1X_PERIOD_16MS     = 2,
    WD_ATTINY1X_PERIOD_31MS     = 3,
    WD_ATTINY1X_PERIOD_63MS     = 4,
    WD_ATTINY1X_PERIOD_125MS    = 5,
    WD_ATTINY1X_PERIOD_250MS    = 6,
    WD_ATTINY1X_PERIOD_500MS    = 7,
    WD_ATTINY1X_PERIOD_1S       = 8,
    WD_ATTINY1X_PERIOD_2S       = 9,
    WD_ATTINY1X_PERIOD_4S       = 10,
    WD_ATTINY1X_PERIOD_8S       = 11,
    WD_ATTINY1X_PERIOD_COUNT
} wd_attiny1x_period_t;

struct wd_attiny1x_settings {
    wd_attiny1x_period_t window;
    wd_attiny1x_period_t period;
};

int wd_attiny1x_init(/*@out@*/ struct wd *ctx, struct WDT_ATTINY1X *base);
int wd_attiny1x_setup(struct wd *ctx, struct wd_attiny1x_settings *settings);

/* Runtime calls:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

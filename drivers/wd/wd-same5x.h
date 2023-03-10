#ifndef WD_SAME5X_H
#define WD_SAME5X_H

#include "wd.h"
#include <stdbool.h>

struct WD_SAME5X;

struct wd {
    /*@temp@*/ struct WD_SAME5X *base;
};

typedef enum {
    WD_SAME5X_PERIOD_8CYC       = 0,
    WD_SAME5X_PERIOD_16CYC      = 1,
    WD_SAME5X_PERIOD_32CYC      = 2,
    WD_SAME5X_PERIOD_64CYC      = 3,
    WD_SAME5X_PERIOD_128CYC     = 4,
    WD_SAME5X_PERIOD_256CYC     = 5,
    WD_SAME5X_PERIOD_512CYC     = 6,
    WD_SAME5X_PERIOD_1024CYC    = 7,
    WD_SAME5X_PERIOD_2028CYC    = 8,
    WD_SAME5X_PERIOD_4096CYC    = 9,
    WD_SAME5X_PERIOD_8192CYC    = 10,
    WD_SAME5X_PERIOD_16384CYC   = 11,
    WD_SAME5X_PERIOD_COUNT
} wd_same5x_period_t;

struct wd_same5x_settings {
    wd_same5x_period_t period;
    bool window_mode;
    wd_same5x_period_t window;
};

int wd_same5x_init(/*@out@*/ struct wd *ctx, struct WD_SAME5X *base);
int wd_same5x_setup(struct wd *ctx, struct wd_same5x_settings *settings);

/* Runtime calls:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

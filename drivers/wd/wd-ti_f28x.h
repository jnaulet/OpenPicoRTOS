#ifndef WD_TI_F28X_H
#define WD_TI_F28X_H

#include "wd.h"
#include "clock.h"

struct WD_REGS;

struct wd {
    /*@temp@*/ struct WD_REGS *base;
    clock_id_t clkid;
};

typedef enum {
    WD_TI_F28X_WDPS_1   = 0,
    WD_TI_F28X_WDPS_1B  = 1,
    WD_TI_F28X_WDPS_2   = 2,
    WD_TI_F28X_WDPS_4   = 3,
    WD_TI_F28X_WDPS_8   = 4,
    WD_TI_F28X_WDPS_16  = 5,
    WD_TI_F28X_WDPS_32  = 6,
    WD_TI_F28X_WDPS_64  = 7,
    WD_TI_F28X_WDPS_COUNT
} wd_ti_f28x_wdps_t;

struct wd_ti_f28x_settings {
    wd_ti_f28x_wdps_t wdps;
    unsigned int wdwcr_min;
};

int wd_ti_f28x_init(/*@out@*/ struct wd *ctx, int base, clock_id_t clkid);
int wd_ti_f28x_setup(struct wd *ctx, struct wd_ti_f28x_settings *settings);

/* Runtime calls:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

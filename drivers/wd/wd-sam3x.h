#ifndef WD_SAM3X_H
#define WD_SAM3X_H

#include "wd.h"
#include "clock.h"

struct WD_SAM3X;

struct wd {
    /*@temp@*/ struct WD_SAM3X *base;
    clock_id_t clkid;
};

struct wd_sam3x_settings {
    unsigned long timeout_ms;
};

int wd_sam3x_init(/*@out@*/ struct wd *ctx, int base, clock_id_t clkid);
int wd_sam3x_setup(struct wd *ctx, const struct wd_sam3x_settings *settings);

/* Runtime calls:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

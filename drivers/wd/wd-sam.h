#ifndef WD_SAM_H
#define WD_SAM_H

#include "wd.h"
#include "clock.h"

struct WD_SAM;

struct wd {
    /*@temp@*/ struct WD_SAM *base;
    clock_id_t clkid;
};

struct wd_sam_settings {
    unsigned long timeout_ms;
};

int wd_sam_init(/*@out@*/ struct wd *ctx, struct WD_SAM *base, clock_id_t clkid);
int wd_sam_setup(struct wd *ctx, struct wd_sam_settings *settings);

/* Runtime calls:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

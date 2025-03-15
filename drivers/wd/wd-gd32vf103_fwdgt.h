#ifndef WD_GD32VF103_FWDGT_H
#define WD_GD32VF103_FWDGT_H

#include "wd.h"
#include "clock.h"

struct WD_GD32VF103_FWDGT;

struct wd {
    /*@temp@*/ struct WD_GD32VF103_FWDGT *base;
    clock_id_t clkid;
};

struct wd_gd32vf103_fwdgt_settings {
    unsigned long timeout_us;
};

int wd_gd32vf103_fwdgt_init(/*@out@*/ struct wd *ctx, int base, clock_id_t clkid);
int wd_gd32vf103_fwdgt_setup(struct wd *ctx, const struct wd_gd32vf103_fwdgt_settings *settings);

/* Implements:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

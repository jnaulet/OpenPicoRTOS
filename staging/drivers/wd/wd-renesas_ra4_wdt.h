#ifndef WD_RENESAS_RA4_WDT_H
#define WD_RENESAS_RA4_WDT_H

#include "wd.h"
#include "clock.h"

struct WD_RENESAS_RA4_WDT;

struct wd {
    /*@temp@*/ struct WD_RENESAS_RA4_WDT *base;
    clock_id_t clkid;
};

int wd_renesas_ra4_wdt_init(/*@out@*/ struct wd *ctx, clock_id_t clkid);

/* Implements:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

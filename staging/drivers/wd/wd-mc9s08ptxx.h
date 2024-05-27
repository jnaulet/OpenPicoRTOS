#ifndef WD_MC9S08PTXX_H
#define WD_MC9S08PTXX_H

#include "wd.h"
#include "clock.h"

struct WD_MC9S08PTXX;

struct wd {
    /*@temp@*/ struct WD_MC9S08PTXX *base;
    clock_id_t clkid;
};

int wd_mc9s08ptxx_wdt_init(/*@out@*/ struct wd *ctx, clock_id_t clkid);

/* Implements:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

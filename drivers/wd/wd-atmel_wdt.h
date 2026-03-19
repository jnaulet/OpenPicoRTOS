#ifndef WD_ATMEL_WDT_H
#define WD_ATMEL_WDT_H

#include "wd.h"
#include "clock.h"

struct WD_ATMEL_WDT;

struct wd {
    /*@temp@*/ struct WD_ATMEL_WDT *base;
    clock_id_t clkid;
};

struct wd_atmel_wdt_settings {
    unsigned long timeout_ms;
};

int wd_atmel_wdt_init(/*@out@*/ struct wd *ctx, int base, clock_id_t clkid);
int wd_atmel_wdt_setup(struct wd *ctx, const struct wd_atmel_wdt_settings *settings);

/* Runtime calls:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

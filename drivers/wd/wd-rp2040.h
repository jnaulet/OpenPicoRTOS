#ifndef WD_RP2040_H
#define WD_RP2040_H

#include "wd.h"
#include "clock.h"

#include <stdint.h>

struct WD_RP2040;

struct wd {
    /*@temp@*/ struct WD_RP2040 *base;
    clock_id_t clkid;
    uint32_t load;
};

struct wd_rp2040_settings {
    unsigned long timeout_us;
};

int wd_rp2040_init(/*@out@*/ struct wd *ctx, int base, clock_id_t clkid);
int wd_rp2040_setup(struct wd *ctx, struct wd_rp2040_settings *settings);

/* Runtime calls:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

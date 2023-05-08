#ifndef WD_PIC32MX_H
#define WD_PIC32MX_H

#include "wd.h"
#include <stdbool.h>

struct WD_PIC32MX;

struct wd {
    /*@temp@*/ struct WD_PIC32MX *base;
};

typedef enum {
    WD_PIC32MX_PERIOD_1MS       = 0,
    WD_PIC32MX_PERIOD_2MS       = 1,
    WD_PIC32MX_PERIOD_4MS       = 2,
    WD_PIC32MX_PERIOD_8MS       = 3,
    WD_PIC32MX_PERIOD_16MS      = 4,
    WD_PIC32MX_PERIOD_32MS      = 5,
    WD_PIC32MX_PERIOD_64MS      = 6,
    WD_PIC32MX_PERIOD_128MS     = 7,
    WD_PIC32MX_PERIOD_256MS     = 8,
    WD_PIC32MX_PERIOD_512MS     = 9,
    WD_PIC32MX_PERIOD_1024MS    = 10,
    WD_PIC32MX_PERIOD_2048MS    = 11,
    WD_PIC32MX_PERIOD_4096MS    = 12,
    WD_PIC32MX_PERIOD_8192MS    = 13,
    WD_PIC32MX_PERIOD_16384MS   = 14,
    WD_PIC32MX_PERIOD_32768MS   = 15,
    WD_PIC32MX_PERIOD_65535MS   = 16,
    WD_PIC32MX_PERIOD_131072MS  = 17,
    WD_PIC32MX_PERIOD_262144MS  = 18,
    WD_PIC32MX_PERIOD_524288MS  = 19,
    WD_PIC32MX_PERIOD_1048576MS = 20,
    WD_PIC32MX_PERIOD_COUNT
} wd_pic32mx_period_t;

struct wd_pic32mx_settings {
    wd_pic32mx_period_t timeout;
    bool enable_windowed;
};

int wd_pic32mx_init(/*@out@*/ struct wd *ctx, int base);
int wd_pic32mx_setup(struct wd *ctx, struct wd_pic32mx_settings *settings);

/* Runtime calls:
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

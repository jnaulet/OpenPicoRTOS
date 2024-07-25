#ifndef S08PT60_EVK_H
#define S08PT60_EVK_H

#include "wd-mc9s08ptxx.h"
#include "gpio-mc9s08ptxx.h"

struct s08pt60_evk {
    struct gpio TICK;
    struct gpio GREEN;
    struct gpio RED;
    /* watchdog */
    struct wd WD;
};

int s08pt60_evk_init(/*@out@*/ struct s08pt60_evk *ctx);

#endif

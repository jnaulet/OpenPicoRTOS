#ifndef Wd_STM32H7XX_IWDG_H
#define Wd_STM32H7XX_IWDG_H

#include "wd.h"
#include "clock.h"

struct WD_STM32H7XX_IWDG;

struct wd {
    /*@temp@*/ struct WD_STM32H7XX_IWDG *base;
    clock_id_t clkid;
};

struct wd_stm32h7xx_iwdg_settings {
    unsigned long timeout_ms;
    unsigned long window_ms;
};

int wd_stm32h7xx_iwdg_init(/*@out@*/ struct wd *ctx, int base, clock_id_t clkid);
int wd_stm32h7xx_iwdg_setup(struct wd *ctx, const struct wd_stm32h7xx_iwdg_settings *settings);

/* Implements:
 *
 * int wd_start(struct wd *ctx);
 * int wd_stop(struct wd *ctx);
 * void wd_refresh(struct wd *ctx);
 */

#endif

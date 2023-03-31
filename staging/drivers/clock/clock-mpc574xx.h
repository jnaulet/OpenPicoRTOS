#ifndef CLOCK_MPC574XX_H
#define CLOCK_MPC574XX_H

#include "clock.h"
#include <stdint.h>

/* clocks list */

#define CLOCK_MPC574XX_F160  CLOCK_SYSCLK
#define CLOCK_MPC574XX_S160  (clock_id_t)1
#define CLOCK_MPC574XX_S80   (clock_id_t)2
#define CLOCK_MPC574XX_FS80  (clock_id_t)3
#define CLOCK_MPC574XX_S40   (clock_id_t)4
#define CLOCK_MPC574XX_F40   (clock_id_t)5
#define CLOCK_MPC574XX_F80   (clock_id_t)6
#define CLOCK_MPC574XX_F20   (clock_id_t)7
#define CLOCK_MPC574XX_COUNT 8

#define CLOCK_MPC574XX_FIRC_FREQ 16000000ul

typedef enum {
    CLOCK_MPC574XX_SYSCLK_FIRC  = 0,
    CLOCK_MPC574XX_SYSCLK_FXOSC = 1,
    CLOCK_MPC574XX_SYSCLK_PHI0  = 2,
    CLOCK_MPC574XX_SYSCLK_COUNT
} clock_mpc574xx_sysclk_t;

typedef enum {
    CLOCK_MPC574XX_FMPLL_CLKIN_FIRC     = 0,
    CLOCK_MPC574XX_FMPLL_CLKIN_FXOSC    = 1,
    CLOCK_MPC574XX_FMPLL_CLKIN_COUNT
} clock_mpc574xx_fmpll_clkin_t;

struct clock_settings {
    clock_mpc574xx_sysclk_t sysclk;
    clock_mpc574xx_fmpll_clkin_t fmpll_clkin;
    unsigned long fxosc;    /* fxosc current value (if used) */
    unsigned long phi0;     /* pll phi0 requested frequency */
    unsigned long s160_div; /* s160 divider (1-8, 0: disable) */
    unsigned long s80_div;  /* s80 divider (1-8, 0: disable) */
    unsigned long fs80_div; /* fs80 divider (1-8, 0: disable) */
    unsigned long s40_div;  /* s40 divider (1-16, 0: disable) */
};

int clock_mpc574xx_init(struct clock_settings *settings);

#endif

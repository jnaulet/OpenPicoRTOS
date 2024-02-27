#ifndef CLOCK_LGT8FX8P_H
#define CLOCK_LGT8FX8P_H

#include "clock.h"

#define CLOCK_LGT8FX8P_CPU_CLK  CLOCK_SYSCLK
#define CLOCK_LGT8FX8P_PERI_CLK (clock_id_t)1
#define CLOCK_LGT8FX8P_E2P_CLK  (clock_id_t)2
#define CLOCK_LGT8FX8P_ASY_CLK  (clock_id_t)3
#define CLOCK_LGT8FX8P_WDT_CLK  (clock_id_t)4
#define CLOCK_LGT8FX8P_COUNT    (clock_id_t)5

typedef enum {
    CLOCK_LGT8FX8P_MCLK_HFRC    = 0,    /* Internal 32MHz */
    CLOCK_LGT8FX8P_MCLK_HERC    = 1,    /* External 400K - 32Mhz */
    CLOCK_LGT8FX8P_MCLK_LFRC    = 2,    /* Internal 32Khz */
    CLOCK_LGT8FX8P_MCLK_LERC    = 3,    /* External 32K ~ 400K */
    CLOCK_LGT8FX8P_MCLK_COUNT
} clock_lgt8fx8p_mclk_t;

typedef enum {
    CLOCK_LGT8FX8P_PS_1     = 0,
    CLOCK_LGT8FX8P_PS_2     = 1,
    CLOCK_LGT8FX8P_PS_4     = 2,
    CLOCK_LGT8FX8P_PS_8     = 3,
    CLOCK_LGT8FX8P_PS_16    = 4,
    CLOCK_LGT8FX8P_PS_32    = 5,
    CLOCK_LGT8FX8P_PS_64    = 6,
    CLOCK_LGT8FX8P_PS_128   = 7,
    CLOCK_LGT8FX8P_PS_256   = 8,
    CLOCK_LGT8FX8P_PS_COUNT
} clock_lgt8fx8p_ps_t;

typedef enum {
    CLOCK_LGT8FX8P_WDCLKS_HFRC_DIV16    = 0,
    CLOCK_LGT8FX8P_WDCLKS_LFRC          = 1,
    CLOCK_LGT8FX8P_WDCLKS_COUNT
} clock_lgt8fx8p_wdclks_t;

struct clock_settings {
    unsigned long erc; /* external xtal freq. 0 = ignore */
    clock_lgt8fx8p_mclk_t mclk;
    clock_lgt8fx8p_ps_t ps;
    clock_lgt8fx8p_wdclks_t wdclks;
};

int clock_lgt8fx8p_init(struct clock_settings *settings);

#endif

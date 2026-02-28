#ifndef CLOCK_CH552_H
#define CLOCK_CH552_H

#include "clock.h"

#define CLOCK_CH552_FSYS  CLOCK_SYSCLK

typedef enum {
    CLOCK_CH552_OSC_INT,
    CLOCK_CH552_OSC_XT,
    CLOCK_CH552_OSC_COUNT
} clock_ch552_osc_t;

typedef enum {
    CLOCK_CH552_SYS_CK_SEL_000  = 0,    /* FSYS = FPLL / 512 */
    CLOCK_CH552_SYS_CK_SEL_001  = 1,    /* FSYS = FPLL / 128 */
    CLOCK_CH552_SYS_CK_SEL_010  = 2,    /* FSYS = FPLL / 32 */
    CLOCK_CH552_SYS_CK_SEL_011  = 3,    /* FSYS = FPLL / 16 */
    CLOCK_CH552_SYS_CK_SEL_100  = 4,    /* FSYS = FPLL / 8 */
    CLOCK_CH552_SYS_CK_SEL_101  = 5,    /* FSYS = FPLL / 6 */
    CLOCK_CH552_SYS_CK_SEL_110  = 6,    /* FSYS = FPLL / 4 */
    CLOCK_CH552_SYS_CK_SEL_COUNT,
} clock_ch552_sys_ck_sel;

struct clock_settings {
    clock_ch552_osc_t osc;
    clock_ch552_sys_ck_sel sys_ck_sel;
};

int clock_init(const struct clock_settings *settings);

#endif

#ifndef CLOCK_TINYAVR_H
#define CLOCK_TINYAVR_H

/* Clock system for tinyAVR 1-series */

#include "clock.h"
#include <stdbool.h>

#define CLOCK_TINYAVR_CLK_CPU CLOCK_SYSCLK
#define CLOCK_TINYAVR_CLK_PER CLOCK_TINYAVR_CLK_CPU
#define CLOCK_TINYAVR_CLK_RTC (clock_id_t)1
#define CLOCK_TINYAVR_CLK_WDT (clock_id_t)2
#define CLOCK_TINYAVR_CLK_BOD CLOCK_TINYAVR_CLK_WDT
#define CLOCK_TINYAVR_CLK_TCD (clock_id_t)3

#define CLOCK_TINYAVR_COUNT 4

typedef enum {
    CLOCK_TINYAVR_CLKSEL_OSC20M     = 0,
    CLOCK_TINYAVR_CLKSEL_OSCULP32K  = 1,
    CLOCK_TINYAVR_CLKSEL_XOSC32K    = 2,
    CLOCK_TINYAVR_CLKSEL_EXTCLK     = 3,
    CLOCK_TINYAVR_CLKSEL_COUNT
} clock_tinyavr_clksel_t;

typedef enum {
    CLOCK_TINYAVR_PDIV_2            = 0,
    CLOCK_TINYAVR_PDIV_4            = 1,
    CLOCK_TINYAVR_PDIV_8            = 2,
    CLOCK_TINYAVR_PDIV_16           = 3,
    CLOCK_TINYAVR_PDIV_32           = 4,
    CLOCK_TINYAVR_PDIV_64           = 5,
    CLOCK_TINYAVR_PDIV_RESERVED0    = 6,
    CLOCK_TINYAVR_PDIV_RESERVED1    = 7,
    CLOCK_TINYAVR_PDIV_6            = 8,
    CLOCK_TINYAVR_PDIV_10           = 9,
    CLOCK_TINYAVR_PDIV_12           = 10,
    CLOCK_TINYAVR_PDIV_24           = 11,
    CLOCK_TINYAVR_PDIV_48           = 12,
    CLOCK_TINYAVR_PDIV_COUNT
} clock_tinyavr_pdiv_t;

struct clock_settings {
    clock_tinyavr_clksel_t clksel;
    bool clkout_enable;
    bool prescaler_enable;
    clock_tinyavr_pdiv_t prescaler_div;
    bool lock_enable;
};

int clock_tinyavr_init(struct clock_settings *settings);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

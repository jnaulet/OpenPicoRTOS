#ifndef CLOCK_ATTINY1X_H
#define CLOCK_ATTINY1X_H

/* Clock system for tinyAVR 1-series */

#include "clock.h"
#include <stdbool.h>

#define CLOCK_ATTINY1X_CLK_CPU CLOCK_SYSCLK
#define CLOCK_ATTINY1X_CLK_PER CLOCK_ATTINY1X_CLK_CPU
#define CLOCK_ATTINY1X_CLK_RTC (clock_id_t)1
#define CLOCK_ATTINY1X_CLK_WDT (clock_id_t)2
#define CLOCK_ATTINY1X_CLK_BOD CLOCK_ATTINY1X_CLK_WDT
#define CLOCK_ATTINY1X_CLK_TCD (clock_id_t)3

#define CLOCK_ATTINY1X_COUNT 4

typedef enum {
    CLOCK_ATTINY1X_CLKSEL_OSC20M    = 0,
    CLOCK_ATTINY1X_CLKSEL_OSCULP32K = 1,
    CLOCK_ATTINY1X_CLKSEL_XOSC32K   = 2,
    CLOCK_ATTINY1X_CLKSEL_EXTCLK    = 3,
    CLOCK_ATTINY1X_CLKSEL_COUNT
} clock_attiny1x_clksel_t;

typedef enum {
    CLOCK_ATTINY1X_PDIV_2           = 0,
    CLOCK_ATTINY1X_PDIV_4           = 1,
    CLOCK_ATTINY1X_PDIV_8           = 2,
    CLOCK_ATTINY1X_PDIV_16          = 3,
    CLOCK_ATTINY1X_PDIV_32          = 4,
    CLOCK_ATTINY1X_PDIV_64          = 5,
    CLOCK_ATTINY1X_PDIV_RESERVED0   = 6,
    CLOCK_ATTINY1X_PDIV_RESERVED1   = 7,
    CLOCK_ATTINY1X_PDIV_6           = 8,
    CLOCK_ATTINY1X_PDIV_10          = 9,
    CLOCK_ATTINY1X_PDIV_12          = 10,
    CLOCK_ATTINY1X_PDIV_24          = 11,
    CLOCK_ATTINY1X_PDIV_48          = 12,
    CLOCK_ATTINY1X_PDIV_COUNT
} clock_attiny1x_pdiv_t;

struct clock_settings {
    clock_attiny1x_clksel_t clksel;
    bool clkout_enable;
    bool prescaler_enable;
    clock_attiny1x_pdiv_t prescaler_div;
    bool lock_enable;
};

int clock_attiny1x_init(struct clock_settings *settings);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

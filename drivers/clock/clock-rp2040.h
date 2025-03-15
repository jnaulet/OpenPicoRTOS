#ifndef CLOCK_RP2040_H
#define CLOCK_RP2040_H

#include "clock.h"

#define CLOCK_RP2040_SYS    CLOCK_SYSCLK
#define CLOCK_RP2040_REF    (clock_id_t)1
#define CLOCK_RP2040_PERI   (clock_id_t)2
#define CLOCK_RP2040_USB    (clock_id_t)3
#define CLOCK_RP2040_ADC    (clock_id_t)4
#define CLOCK_RP2040_RTC    (clock_id_t)5
#define CLOCK_RP2040_GPOUT0 (clock_id_t)6
#define CLOCK_RP2040_GPOUT1 (clock_id_t)7
#define CLOCK_RP2040_GPOUT2 (clock_id_t)8
#define CLOCK_RP2040_GPOUT3 (clock_id_t)9

struct clock_settings {
    unsigned long xosc;             /* xosc current freq */
    unsigned long pll_sys;          /* pll_sys requested freq */
    /* dividers */
    unsigned long ref_div;
    unsigned long sys_div;
    unsigned long clk_gpout0_div;   /* ! */
    unsigned long clk_gpout1_div;   /* ! */
    unsigned long clk_gpout2_div;   /* ! */
    unsigned long clk_gpout3_div;   /* ! */
};

int clock_rp2040_init(const struct clock_settings *settings);

int clock_rp2040_enable(clock_id_t clkid);
int clock_rp2040_disable(clock_id_t clkid);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

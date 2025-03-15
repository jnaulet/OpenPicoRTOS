#ifndef CLOCK_MC9S08PTXX_H
#define CLOCK_MC9S08PTXX_H

#include "clock.h"

#define CLOCK_NXP_MC9S08PTXX_BUS CLOCK_SYSCLK
#define CLOCK_NXP_MC9S08PTXX_LPO (clock_id_t)1
#define CLOCK_NXP_MC9S08PTXX_32K (clock_id_t)2 /* internal slow ref clk */
#define CLOCK_NXP_MC9S08PTXX_EXT (clock_id_t)3

typedef enum {
    CLOCK_MC9S08PTXX_MODE_FEI,  /* FLL Engaged Internal */
    CLOCK_MC9S08PTXX_MODE_FBI,  /* FLL Bypassed Internal */
    CLOCK_MC9S08PTXX_MODE_FEE,  /* FLL Engaged External */
    CLOCK_MC9S08PTXX_MODE_FBE,  /* FLL Bypassed External */
    CLOCK_MC9S08PTXX_MODE_COUNT
} clock_mc9s08ptxx_mode_t;

struct clock_settings {
    clock_mc9s08ptxx_mode_t mode;
    unsigned long xosc; /* 0: disable */
    unsigned long freq; /* requested freq */
};

int clock_mc9s08ptxx_init(const struct clock_settings *settings);

/* Implements:
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

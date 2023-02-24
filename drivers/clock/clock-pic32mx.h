#ifndef CLOCK_PIC32MX_H
#define CLOCK_PIC32MX_H

#include "clock.h"

#define CLOCK_PIC32MX_SYSCLK   CLOCK_SYSCLK
#define CLOCK_PIC32MX_USBCLK   (clock_id_t)1
#define CLOCK_PIC32MX_LPRC     (clock_id_t)2
#define CLOCK_PIC32MX_SOSC     (clock_id_t)3
#define CLOCK_PIC32MX_PBCLK1   (clock_id_t)4
#define CLOCK_PIC32MX_PBCLK2   (clock_id_t)5
#define CLOCK_PIC32MX_PBCLK3   (clock_id_t)6
#define CLOCK_PIC32MX_PBCLK4   (clock_id_t)7
#define CLOCK_PIC32MX_PBCLK5   (clock_id_t)8
#define CLOCK_PIC32MX_PBCLK6   (clock_id_t)9
#define CLOCK_PIC32MX_PBCLK7   (clock_id_t)10
#define CLOCK_PIC32MX_PBCLK8   (clock_id_t)11

#define CLOCK_PIC32MX_COUNT       12

typedef enum {
    CLOCK_PIC32MX_OSC_FRC       = 0,
    CLOCK_PIC32MX_OSC_SPLL      = 1,
    CLOCK_PIC32MX_OSC_POSC      = 2,
    CLOCK_PIC32MX_OSC_RESERVED  = 3,
    CLOCK_PIC32MX_OSC_SOSC      = 4,
    CLOCK_PIC32MX_OSC_LPRC      = 5,
    CLOCK_PIC32MX_OSC_BFRC      = 6,
    CLOCK_PIC32MX_OSC_FRCDIV    = 7,
    CLOCK_PIC32MX_OSC_COUNT
} clock_pic32mx_osc_t;

typedef enum {
    CLOCK_PIC32MX_PLLICLK_POSC  = 0,
    CLOCK_PIC32MX_PLLICLK_FRC   = 1,
    CLOCK_PIC32MX_PLLICLK_COUNT
} clock_pic32mx_plliclk_t;

struct clock_settings {
    unsigned long posc;                 /* provided posc freq */
    clock_pic32mx_plliclk_t plliclk;    /* pll input clock selection */
    unsigned long pllo;                 /* pll output requested freq */
    clock_pic32mx_osc_t osc;            /* main clock selection */
};

int clock_pic32mx_init(struct clock_settings *settings);

#define CLOCK_PIC32MX_PBDIV_MAX 128ul

int clock_pic32mx_pbclk_enable(clock_id_t clkid, unsigned long pbdiv);
/*@unused@*/ int clock_pic32mx_pbclk_disable(clock_id_t clkid);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 *
 */

#endif

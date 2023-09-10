#ifndef CLOCK_RENESAS_RA4_H
#define CLOCK_RENESAS_RA4_H

#include "clock.h"

#define CLOCK_RENESAS_RA4_FCLK      (clock_id_t)1   /* Flash interface clock */
#define CLOCK_RENESAS_RA4_ICLK      CLOCK_SYSCLK    /* System clock */
#define CLOCK_RENESAS_RA4_PCLKA     (clock_id_t)2   /* Peripheral module clock(s) */
#define CLOCK_RENESAS_RA4_PCLKB     (clock_id_t)3   /* Peripheral module clock(s) */
#define CLOCK_RENESAS_RA4_PCLKC     (clock_id_t)4   /* Peripheral module clock(s) */
#define CLOCK_RENESAS_RA4_PCLKD     (clock_id_t)5   /* Peripheral module clock(s) */
#define CLOCK_RENESAS_RA4_TRCLK     (clock_id_t)6   /* Trace clock */
#define CLOCK_RENESAS_RA4_LCDSRCCLK (clock_id_t)7   /* LCD clock */
#define CLOCK_RENESAS_RA4_SYSTICCLK (clock_id_t)8   /* SysTick Timer */
#define CLOCK_RENESAS_RA4_AGTSCLK   (clock_id_t)9   /* AGT clock */
#define CLOCK_RENESAS_RA4_AGTLCLK   (clock_id_t)10  /* To AGT */
#define CLOCK_RENESAS_RA4_UCLK      (clock_id_t)11  /* USB clock */
#define CLOCK_RENESAS_RA4_CLKOUT    (clock_id_t)12  /* Clock/Buzzer output */
#define CLOCK_RENESAS_RA4_CANMCLK   (clock_id_t)13  /* CAN clock */
#define CLOCK_RENESAS_RA4_IWDTCLK   (clock_id_t)14  /* IWDT clock */
/* TBD... */

#define CLOCK_RENESAS_RA4_COUNT 15

/* public interface */

typedef enum {
    CLOCK_RENESAS_RA4_MOSEL_RESONATOR       = 0,
    CLOCK_RENESAS_RA4_MOSEL_EXT_CLOCK_INPUT = 1,
    CLOCK_RENESAS_RA4_MOSEL_COUNT
} clock_renesas_ra4_mosel_t;

typedef enum {
    CLOCK_RENESAS_RA4_CKSEL_HOCO    = 0,
    CLOCK_RENESAS_RA4_CKSEL_MOCO    = 1,
    CLOCK_RENESAS_RA4_CKSEL_LOCO    = 2,
    CLOCK_RENESAS_RA4_CKSEL_MOSC    = 3,
    CLOCK_RENESAS_RA4_CKSEL_SOSC    = 4,
    CLOCK_RENESAS_RA4_CKSEL_PLL     = 5,
    CLOCK_RENESAS_RA4_CKSEL_COUNT
} clock_renesas_ra4_cksel_t;

typedef enum {
    CLOCK_RENESAS_RA4_SCKDIV_1  = 0,
    CLOCK_RENESAS_RA4_SCKDIV_2  = 1,
    CLOCK_RENESAS_RA4_SCKDIV_4  = 2,
    CLOCK_RENESAS_RA4_SCKDIV_8  = 3,
    CLOCK_RENESAS_RA4_SCKDIV_16 = 4,
    CLOCK_RENESAS_RA4_SCKDIV_32 = 5,
    CLOCK_RENESAS_RA4_SCKDIV_64 = 6,
    CLOCK_RENESAS_RA4_SCKDIV_COUNT,
} clock_renesas_ra4_sckdiv_t;

struct clock_settings {
    clock_renesas_ra4_mosel_t mosel;
    unsigned long mosc; /* oscillator frequency */
    clock_renesas_ra4_cksel_t cksel;
    unsigned long pll;  /* requested pll frequency. 0 = disable */
    clock_renesas_ra4_sckdiv_t fck;
    clock_renesas_ra4_sckdiv_t ick;
    clock_renesas_ra4_sckdiv_t pcka;
    clock_renesas_ra4_sckdiv_t pckb;
    clock_renesas_ra4_sckdiv_t pckc;
    clock_renesas_ra4_sckdiv_t pckd;
};

int clock_renesas_ra4_init(struct clock_settings *settings);

/*
 * FIXME: this is not the right place to put that, but it's more convenient
 * for the moment
 */

#define MODULE_A(x) (x)
#define MODULE_B(x) ((1 << 5) | (x))
#define MODULE_C(x) ((2 << 5) | (x))
#define MODULE_D(x) ((3 << 5) | (x))

int clock_renesas_ra4_module_start(size_t module);
int clock_renesas_ra4_module_stop(size_t module);

/* Runtime calls:
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

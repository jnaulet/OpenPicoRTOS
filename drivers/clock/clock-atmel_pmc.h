#ifndef CLOCK_ATMEL_PMC_H
#define CLOCK_ATMEL_PMC_H

#include "clock.h"

#define CLOCK_ATMEL_PMC_PCK_COUNT  8
#define CLOCK_ATMEL_PMC_PCK_OFFSET 16

#define CLOCK_ATMEL_PMC_SCLK      (clock_id_t)1
#define CLOCK_ATMEL_PMC_HCLK      CLOCK_SYSCLK
#define CLOCK_ATMEL_PMC_SYSTICK   (clock_id_t)2
#define CLOCK_ATMEL_PMC_FCLK      (clock_id_t)3
#define CLOCK_ATMEL_PMC_MCK       (clock_id_t)4
#define CLOCK_ATMEL_USB_HS        (clock_id_t)5
#define CLOCK_ATMEL_USB_FS        (clock_id_t)6
#define CLOCK_ATMEL_PMC_PCK(x)    (clock_id_t)(CLOCK_ATMEL_PMC_PCK_OFFSET + (x))

typedef enum {
    CLOCK_ATMEL_PMC_MAINCK_RC   = 0,
    CLOCK_ATMEL_PMC_MAINCK_XTAL = 1,
    CLOCK_ATMEL_PMC_MAINCK_COUNT
} clock_atmel_pmc_mainck_t;

typedef enum {
    CLOCK_ATMEL_PMC_UPLL_DISABLE,
    CLOCK_ATMEL_PMC_UPLL_DIV1,
    CLOCK_ATMEL_PMC_UPLL_DIV2,
    CLOCK_ATMEL_PMC_UPLL_COUNT
} clock_atmel_pmc_upll_t;

typedef enum {
    CLOCK_ATMEL_PMC_MCKR_CSS_SLCK       = 0,
    CLOCK_ATMEL_PMC_MCKR_CSS_MAINCK     = 1,
    CLOCK_ATMEL_PMC_MCKR_CSS_PLLACK     = 2,
    CLOCK_ATMEL_PMC_MCKR_CSS_UPLLCKDIV  = 3,
    CLOCK_ATMEL_PMC_MCKR_CSS_COUNT
} clock_atmel_pmc_mckr_css_t;

struct clock_settings {
    /* clock generator */
    clock_atmel_pmc_mainck_t mainck;    /* main oscillator (MAINCK) */
    unsigned long mosc;                 /* main oscillator frequency */
    unsigned long plla;                 /* requested plla frequency (0 = disable) */
    clock_atmel_pmc_upll_t upll;
    /* host clock controller */
    clock_atmel_pmc_mckr_css_t mckr_css;
    unsigned long mckr_prescaler;
    unsigned long mckr_mdiv;
    /* usb clock controller (TODO) */
    /* peripheral clock controller (TODO) */
};

int clock_atmel_pmc_init(const struct clock_settings *settings);

/* periph_clk */
int clock_atmel_pmc_periph_clk_enable(unsigned pid);
int clock_atmel_pmc_periph_clk_disable(unsigned pid);

/* PCKx */
typedef enum {
    CLOCK_ATMEL_PMC_PCKR_CSS_SLCK       = 0,
    CLOCK_ATMEL_PMC_PCKR_CSS_MAINCK     = 1,
    CLOCK_ATMEL_PMC_PCKR_CSS_PLLACK     = 2,
    CLOCK_ATMEL_PMC_PCKR_CSS_UPLLCKDIV  = 3,
    CLOCK_ATMEL_PMC_PCKR_CSS_MCK        = 4,
    CLOCK_ATMEL_PMC_PCKR_CSS_AUDIOCK    = 5,
    CLOCK_ATMEL_PMC_PCKR_CSS_COUNT
} clock_atmel_pmc_pckx_css_t;

int clock_atmel_pmc_pck_enable(size_t pck, unsigned prescaler, clock_atmel_pmc_pckx_css_t css);
int clock_atmel_pmc_pck_disable(size_t pck);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

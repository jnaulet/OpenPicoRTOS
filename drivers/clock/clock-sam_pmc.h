#ifndef CLOCK_SAM_PMC_H
#define CLOCK_SAM_PMC_H

#include "clock.h"

#define CLOCK_SAM_PMC_PCK_BASE    16
#define CLOCK_SAM_PMC_PERIPH_BASE 32

#define CLOCK_SAM_PMC_SCLK      (clock_id_t)1
#define CLOCK_SAM_PMC_HCLK      CLOCK_SYSCLK
#define CLOCK_SAM_PMC_SYSTICK   (clock_id_t)2
#define CLOCK_SAM_PMC_FCLK      (clock_id_t)3
#define CLOCK_SAM_PMC_MCK       (clock_id_t)4
#define CLOCK_SAM_USB_HS        (clock_id_t)5
#define CLOCK_SAM_USB_FS        (clock_id_t)6
#define CLOCK_SAM_PMC_PCK(x)    (clock_id_t)(CLOCK_SAM_PMC_PCK_BASE + (x))
#define CLOCK_SAM_PMC_PERIPH(x) (clock_id_t)(CLOCK_SAM_PMC_PERIPH_BASE + (x))

struct clock_settings {
    unsigned long moscx;    /* main crystal oscillator frequency */
    unsigned long plla;     /* requested plla frequency */
    /* dividers */
    unsigned long mck_div;
    unsigned long usb_div;
};

int clock_sam_pmc_init(struct clock_settings *settings);

typedef enum {
    CLOCK_SAM_PMC_PERIPH_DIV_1  = 0,
    CLOCK_SAM_PMC_PERIPH_DIV_2  = 1,
    CLOCK_SAM_PMC_PERIPH_DIV_4  = 2,
    CLOCK_SAM_PMC_PERIPH_DIV_COUNT
} clock_sam_pmc_periph_div_t;

int clock_sam_pmc_enable(clock_id_t clkid, clock_sam_pmc_periph_div_t periph_div);
int clock_sam_pmc_disable(clock_id_t clkid);

/* Implements:
 *
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif

#include "wd-avr.h"
#include "picoRTOS.h"

#include <stdint.h>

struct WDT_AVR {
    volatile uint8_t WDTCSR;
};

#define WDTCSR_WDP3   (1 << 5)
#define WDTCSR_WDCE   (1 << 4)
#define WDTCSR_WDE    (1 << 3)
#define WDTCSR_WDP_M  0x7u
#define WDTCSR_WDP(x) ((x) & WDTCSR_WDP_M)

/* Function: wd_avr_init
 * Initializes the watchdog timer
 *
 * Parameters:
 *  ctx - The WD to init
 *  base - The WD base address
 *
 * Returns:
 * Always 0
 */
int wd_avr_init(struct wd *ctx, int base)
{
    ctx->base = (struct WDT_AVR*)base;
    return 0;
}

/* Function: wd_avr_setup
 * Configures a watchdog timer
 *
 * Parameters:
 *  ctx - The WD to configure
 *  settings - The configuration parameters
 *
 * Return:
 * 0 if success, -errno otherwise
 */
int wd_avr_setup(struct wd *ctx, struct wd_avr_settings *settings)
{
    if (!picoRTOS_assert(settings->cycles < WD_AVR_CYCLES_COUNT)) return -EINVAL;

    ctx->cycles = settings->cycles;
    return 0;
}

int wd_start(struct wd *ctx)
{
    volatile register uint8_t *WDTCSR = &ctx->base->WDTCSR;
    register uint8_t value = (uint8_t)WDTCSR_WDP(ctx->cycles) |
                             ((ctx->cycles > WD_AVR_CYCLES_256K) ? WDTCSR_WDP3 : 0) |
                             WDTCSR_WDE;

    ASM("wdr");
    *WDTCSR = (uint8_t)(WDTCSR_WDCE | WDTCSR_WDE);
    *WDTCSR = value;

    return 0;
}

int wd_stop(struct wd *ctx)
{
    volatile register uint8_t *WDTCSR = &ctx->base->WDTCSR;

    ASM("wdr");
    *WDTCSR = (uint8_t)(WDTCSR_WDCE | WDTCSR_WDE);
    *WDTCSR = 0;

    return 0;
}

void wd_refresh(/*@unused@*/ struct wd *ctx)
{
    ASM("wdr");
}

#include "misc-rp2040_reset.h"
#include "picoRTOS.h"

#include <stdint.h>

struct RP2040_RESET {
    volatile uint32_t RESET;
    volatile uint32_t WDSEL;
    volatile uint32_t RESET_DONE;
};

#define RESET_RESET_M  0x1fffffffu
#define RESET_RESET(x) ((x) & RESET_RESET_M)

int rp2040_reset_init(struct rp2040_reset *ctx, int base)
{
    ctx->base = (struct RP2040_RESET*)base;
    /* reset all non-essential peripherals to put the system in a known state */
    ctx->base->RESET = (uint32_t)RESET_RESET(~((1 << RP2040_RESET_IO_QSPI) |
                                               (1 << RP2040_RESET_PADS_QSPI) |
                                               (1 << RP2040_RESET_PLL_SYS) |
                                               (1 << RP2040_RESET_PLL_USB) |
                                               (1 << RP2040_RESET_SYSCFG) |
                                               (1 << RP2040_RESET_USBCTRL)));
    return 0;
}

int rp2040_reset(struct rp2040_reset *ctx, rp2040_reset_t ss)
{
    if (!picoRTOS_assert(ss < RP2040_RESET_COUNT)) return -EINVAL;

    ctx->base->RESET |= (uint32_t)(1 << ss);
    return 0;
}

int rp2040_unreset(struct rp2040_reset *ctx, rp2040_reset_t ss)
{
    if (!picoRTOS_assert(ss < RP2040_RESET_COUNT)) return -EINVAL;

    int deadlock = CONFIG_DEADLOCK_COUNT;
    uint32_t mask = (uint32_t)(1 << ss);

    ctx->base->RESET &= ~(uint32_t)(1 << ss);

    while (deadlock-- != 0)
        if ((ctx->base->RESET_DONE & mask) != 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

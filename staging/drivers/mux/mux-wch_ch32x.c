#include "mux-wch_ch32x.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct MUX_WCH_CH32X {
    volatile uint32_t CFGLR;
    volatile uint32_t CFGHR;
    /* unused */
};

/* TODO: AFIO */

#define CFGLR_CNF0_M   0x3u
#define CFGLR_CNF0(x)  (((x) & CFGLR_CNF0_M) << 2)
#define CFGLR_MODE0_M  0x3u
#define CFGLR_MODE0(x) ((x) & CFGLR_MODE0_M)

#define CFGHR_CNF8_M 0x3u
#define CFGHR_CNF8(x) (((x) & CFGHR_CNF8_M) << 2)
#define CFGHR_MODE8_M  0x3u
#define CFGHR_MODE8(x) ((x) & CFGHR_MODE8_M)

int mux_wch_ch32x_init(struct mux *ctx, int base)
{
    ctx->base = (struct MUX_WCH_CH32X*)base;
    return 0;
}

int mux_wch_ch32x_output(struct mux *ctx, size_t pin, mux_wch_ch32x_output_t type)
{
    picoRTOS_assert(pin < (size_t)MUX_WCH_CH32X_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(type < MUX_WCH_CH32X_OUTPUT_COUNT, return -EINVAL);

    if (pin < (size_t)8) {
        size_t shift = pin << 2;
        ctx->base->CFGLR &= ~(CFGLR_CNF0(CFGLR_CNF0_M) << shift);
        ctx->base->CFGLR |= (CFGLR_CNF0(type) << shift);
        ctx->base->CFGLR &= ~(CFGLR_MODE0(CFGLR_MODE0_M) << shift);
        ctx->base->CFGLR |= (CFGLR_MODE0(1) << shift); /* FIXME */
    }else{
        size_t shift = (pin - (size_t)8) << 2;
        ctx->base->CFGHR &= ~(CFGHR_CNF8(CFGHR_CNF8_M) << shift);
        ctx->base->CFGHR |= (CFGHR_CNF8(type) << shift);
        ctx->base->CFGHR &= ~(CFGHR_MODE8(CFGHR_MODE8_M) << shift);
        ctx->base->CFGHR |= (CFGHR_MODE8(1) << shift); /* FIXME */
    }

    return 0;
}

int mux_wch_ch32x_input(struct mux *ctx, size_t pin, mux_wch_ch32x_input_t type)
{
    picoRTOS_assert(pin < (size_t)MUX_WCH_CH32X_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(type < MUX_WCH_CH32X_INPUT_COUNT, return -EINVAL);

    if (pin < (size_t)8) {
        size_t shift = pin << 2;
        ctx->base->CFGLR &= ~(CFGLR_CNF0(CFGLR_CNF0_M) << shift);
        ctx->base->CFGLR |= (CFGLR_CNF0(type) << shift);
        ctx->base->CFGLR &= ~(CFGLR_MODE0(CFGLR_MODE0_M) << shift);
    }else{
        size_t shift = (pin - (size_t)8) << 2;
        ctx->base->CFGHR &= ~(CFGHR_CNF8(CFGHR_CNF8_M) << shift);
        ctx->base->CFGHR |= (CFGHR_CNF8(type) << shift);
        ctx->base->CFGHR &= ~(CFGHR_MODE8(CFGHR_MODE8_M) << shift);
    }

    return 0;
}

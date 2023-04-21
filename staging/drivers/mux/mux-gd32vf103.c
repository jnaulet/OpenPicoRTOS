#include "mux-gd32vf103.h"
#include "picoRTOS.h"

#include <stdint.h>

struct MUX_GD32VF103_GPIO {
    volatile uint32_t GPIOx_CTLn[2];
    volatile uint32_t GPIOx_ISTAT;
    volatile uint32_t GPIOx_OCTL;
    volatile uint32_t GPIOx_BC;
    volatile uint32_t GPIOx_LOCK;
};

#define GPIOx_CTL0_CTL0_M  0x3u
#define GPIOx_CTL0_CTL0(x) (((x) & GPIOx_CTL0_CTL0_M) << 2)
#define GPIOx_CTL0_MD0_M   0x3u
#define GPIOx_CTL0_MD0(x)  ((x) & GPIOx_CTL0_MD0_M)

int mux_gd32vf103_init(/*@out@*/ struct mux *ctx, struct MUX_GD32VF103_GPIO *base)
{
    ctx->base = base;
    return 0;
}

int mux_gd32vf103_analog(struct mux *ctx, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_GD32VF103_PIN_COUNT)) return -EINVAL;

    size_t index = pin >> 3;
    size_t shift = (size_t)((0x7u & pin) * 4u);

    ctx->base->GPIOx_CTLn[index] &= ~((GPIOx_CTL0_CTL0(GPIOx_CTL0_CTL0_M) |
                                       GPIOx_CTL0_MD0(GPIOx_CTL0_MD0_M)) << shift);

    return 0;
}

int mux_gd32vf103_input(struct mux *ctx, size_t pin, mux_gd32vf103_mode_t mode)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_GD32VF103_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(mode < MUX_GD32VF103_MODE_COUNT)) return -EINVAL;

    size_t index = pin >> 3;
    size_t shift = (size_t)((0x7u & pin) * 4u);

    /* zero */
    ctx->base->GPIOx_CTLn[index] &= ~((GPIOx_CTL0_CTL0(GPIOx_CTL0_CTL0_M) |
                                       GPIOx_CTL0_MD0(GPIOx_CTL0_MD0_M)) << shift);

    /* input */
    if (mode == MUX_GD32VF103_MODE_PUSH_PULL)
        ctx->base->GPIOx_CTLn[index] |= (GPIOx_CTL0_CTL0(2) << shift);
    else
        ctx->base->GPIOx_CTLn[index] |= (GPIOx_CTL0_CTL0(1) << shift);

    return 0;
}

int mux_gd32vf103_output(struct mux *ctx, size_t pin, mux_gd32vf103_mode_t mode)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_GD32VF103_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(mode < MUX_GD32VF103_MODE_COUNT)) return -EINVAL;

    size_t index = pin >> 3;
    size_t shift = (size_t)((0x7u & pin) * 4u);

    /* zero */
    ctx->base->GPIOx_CTLn[index] &= ~((GPIOx_CTL0_CTL0(GPIOx_CTL0_CTL0_M) |
                                       GPIOx_CTL0_MD0(GPIOx_CTL0_MD0_M)) << shift);

    /* input */
    if (mode == MUX_GD32VF103_MODE_PUSH_PULL)
        ctx->base->GPIOx_CTLn[index] |= (GPIOx_CTL0_MD0(0) << shift);
    else
        ctx->base->GPIOx_CTLn[index] |= (GPIOx_CTL0_MD0(1) << shift);

    return 0;
}

int mux_gd32vf103_afio(struct mux *ctx, size_t pin, mux_gd32vf103_mode_t mode)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_GD32VF103_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(mode < MUX_GD32VF103_MODE_COUNT)) return -EINVAL;

    size_t index = pin >> 3;
    size_t shift = (size_t)((0x7u & pin) * 4u);

    /* zero */
    ctx->base->GPIOx_CTLn[index] &= ~((GPIOx_CTL0_CTL0(GPIOx_CTL0_CTL0_M) |
                                       GPIOx_CTL0_MD0(GPIOx_CTL0_MD0_M)) << shift);

    /* input */
    if (mode == MUX_GD32VF103_MODE_PUSH_PULL)
        ctx->base->GPIOx_CTLn[index] |= (GPIOx_CTL0_MD0(2) << shift);
    else
        ctx->base->GPIOx_CTLn[index] |= (GPIOx_CTL0_MD0(3) << shift);

    return 0;
}

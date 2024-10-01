#include "mux-stm8.h"
#include "picoRTOS.h"

#include <stdint.h>

struct MUX_STM8 {
    volatile uint8_t ODR;
    volatile uint8_t IDR;
    volatile uint8_t DDR;
    volatile uint8_t CR1;
    volatile uint8_t CR2;
};

int mux_stm8_init(struct mux *ctx, int base)
{
    ctx->base = (struct MUX_STM8*)base;
    return 0;
}

int mux_stm8_output(struct mux *ctx, size_t pin, mux_stm8_output_t type)
{
    picoRTOS_assert(pin < (size_t)MUX_STM8_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(type < MUX_STM8_OUTPUT_COUNT, return -EINVAL);

    ctx->base->DDR |= (1u << pin);

    switch (type) {
    case MUX_STM8_OUTPUT_OPEN_DRAIN:
        ctx->base->CR1 &= ~(1u << pin);
        ctx->base->CR2 &= ~(1u << pin);
        break;

    case MUX_STM8_OUTPUT_PUSH_PULL:
        ctx->base->CR1 |= (1u << pin);
        ctx->base->CR2 &= ~(1u << pin);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

int mux_stm8_input(struct mux *ctx, size_t pin, mux_stm8_input_t type)
{
    picoRTOS_assert(pin < (size_t)MUX_STM8_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(type < MUX_STM8_INPUT_COUNT, return -EINVAL);

    ctx->base->DDR &= ~(1u << pin);

    switch (type) {
    case MUX_STM8_INPUT_FLOATING:
        ctx->base->CR1 &= ~(1u << pin);
        ctx->base->CR2 &= ~(1u << pin);
        break;

    case MUX_STM8_INPUT_PULL_UP:
        ctx->base->CR1 |= (1u << pin);
        ctx->base->CR2 &= ~(1u << pin);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

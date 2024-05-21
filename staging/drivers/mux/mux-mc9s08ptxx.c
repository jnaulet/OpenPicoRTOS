#include "mux-mc9s08ptxx.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

int mux_mc9s08ptxx_init(struct mux *ctx, size_t port)
{
    picoRTOS_assert(port < (size_t)MUX_MC9S08PTXX_PORT_COUNT, return -EINVAL);

    ctx->PTxOE = (uint8_t*)(ADDR_POE + port);
    ctx->PTxIE = (uint8_t*)(ADDR_PIE + port);
    ctx->PTxPE = (uint8_t*)(ADDR_PPU + port);

    return 0;
}

int mux_mc9s08ptxx_output(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_MC9S08PTXX_PIN_COUNT, return -EINVAL);

    *ctx->PTxOE |= (uint8_t)(1 << pin);
    *ctx->PTxIE &= ~(uint8_t)(1 << pin);

    return 0;
}

int mux_mc9s08ptxx_input(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_MC9S08PTXX_PIN_COUNT, return -EINVAL);

    *ctx->PTxOE &= ~(uint8_t)(1 << pin);
    *ctx->PTxIE |= (uint8_t)(1 << pin);

    return 0;
}

int mux_mc9s08ptxx_pull_up(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_MC9S08PTXX_PIN_COUNT, return -EINVAL);

    *ctx->PTxPE |= (uint8_t)(1 << pin);

    return 0;
}

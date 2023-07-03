#ifndef MUX_STM32H7XX_H
#define MUX_STM32H7XX_H

#include "mux.h"
#include <stddef.h>

#define MUX_STM32H7XX_PIN_COUNT 16
#define MUX_STM32H7XX_ALT_COUNT 16

struct MUX_STM32H7XX;

struct mux {
    /*@temp@*/ struct MUX_STM32H7XX *base;
};

int mux_stm32h7xx_init(/*@out@*/ struct mux *ctx, int base);

int mux_stm32h7xx_analog(struct mux *ctx, size_t pin);
int mux_stm32h7xx_output(struct mux *ctx, size_t pin);
int mux_stm32h7xx_input(struct mux *ctx, size_t pin);
int mux_stm32h7xx_alt(struct mux *ctx, size_t pin, size_t alt);

int mux_stm32h7xx_pull_up(struct mux *ctx, size_t pin);
int mux_stm32h7xx_pull_down(struct mux *ctx, size_t pin);

#endif

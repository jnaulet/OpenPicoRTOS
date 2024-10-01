#ifndef MUX_STM8_H
#define MUX_STM8_H

#include "mux.h"
#include <stddef.h>

#define MUX_STM8_PIN_COUNT 8

struct MUX_STM8;

struct mux {
    /*@temp@*/ struct MUX_STM8 *base;
};

typedef enum {
    MUX_STM8_INPUT_FLOATING,
    MUX_STM8_INPUT_PULL_UP,
    MUX_STM8_INPUT_COUNT
} mux_stm8_input_t;

typedef enum {
    MUX_STM8_OUTPUT_OPEN_DRAIN,
    MUX_STM8_OUTPUT_PUSH_PULL,
    MUX_STM8_OUTPUT_COUNT
} mux_stm8_output_t;

int mux_stm8_init(/*@out@*/ struct mux *ctx, int base);

int mux_stm8_output(struct mux *ctx, size_t pin, mux_stm8_output_t type);
int mux_stm8_input(struct mux *ctx, size_t pin, mux_stm8_input_t type);

#endif

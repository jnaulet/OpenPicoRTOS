#ifndef MUX_STM32F10XXX_H
#define MUX_STM32F10XXX_H

#include "mux.h"
#include <stddef.h>

#define MUX_STM32F10XXX_PIN_COUNT 16
// #define MUX_STM32F10XXX_ALT_COUNT 16

struct MUX_STM32F10XXX;

struct mux {
    /*@temp@*/ struct MUX_STM32F10XXX *base;
};

typedef enum {
    MUX_STM32F10XXX_INPUT_FLOATING,
    MUX_STM32F10XXX_INPUT_PULL,
    MUX_STM32F10XXX_INPUT_COUNT
} mux_stm32f10xxx_input_t;

typedef enum {
    MUX_STM32F10XXX_OUTPUT_PUSH_PULL,
    MUX_STM32F10XXX_OUTPUT_OPEN_DRAIN,
    MUX_STM32F10XXX_OUTPUT_COUNT
} mux_stm32f10xxx_output_t;

typedef enum {
    MUX_STM32F10XXX_ALT_PUSH_PULL,
    MUX_STM32F10XXX_ALT_OPEN_DRAIN,
    MUX_STM32F10XXX_ALT_COUNT
} mux_stm32f10xxx_alt_t;

int mux_stm32f10xxx_init(/*@out@*/ struct mux *ctx, int base);

int mux_stm32f10xxx_analog(struct mux *ctx, size_t pin);
int mux_stm32f10xxx_output(struct mux *ctx, size_t pin, mux_stm32f10xxx_output_t type);
int mux_stm32f10xxx_input(struct mux *ctx, size_t pin, mux_stm32f10xxx_input_t type);
int mux_stm32f10xxx_alt(struct mux *ctx, size_t pin, mux_stm32f10xxx_alt_t type);

#endif

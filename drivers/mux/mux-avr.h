#ifndef MUX_AVR_H
#define MUX_AVR_H

#include "mux.h"
#include <stddef.h>
#include <stdbool.h>

struct MUX_AVR;

#define MUX_AVR_PIN_COUNT 8

struct mux {
    /*@temp@*/ struct MUX_AVR *base;
};

int mux_avr_init(/*@out@*/ struct mux *ctx, struct MUX_AVR *base);

int mux_avr_input(struct mux *ctx, size_t pin);
int mux_avr_output(struct mux *ctx, size_t pin, bool value);

#endif

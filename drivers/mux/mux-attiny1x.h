#ifndef MUX_ATTINY1X_H
#define MUX_ATTINY1X_H

#include "mux.h"
#include <stddef.h>
#include <stdbool.h>

struct MUX_ATTINY1X;

#define MUX_ATTINY1X_PIN_COUNT 8

struct mux {
    /*@temp@*/ struct MUX_ATTINY1X *base;
};

/* TODO: add PORTMUX to support ALT pinouts */

int mux_attiny1x_init(/*@out@*/ struct mux *ctx, struct MUX_ATTINY1X *base);

int mux_attiny1x_input(struct mux *ctx, size_t pin);
int mux_attiny1x_output(struct mux *ctx, size_t pin);

int mux_attiny1x_pull_up(struct mux *ctx, size_t pin);

#endif

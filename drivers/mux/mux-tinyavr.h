#ifndef MUX_TINYAVR_H
#define MUX_TINYAVR_H

#include "mux.h"
#include <stddef.h>
#include <stdbool.h>

struct MUX_TINYAVR;

#define MUX_TINYAVR_PIN_COUNT 8

struct mux {
    /*@temp@*/ struct MUX_TINYAVR *base;
};

/* TODO: add PORTMUX to support ALT pinouts */

int mux_tinyavr_init(/*@out@*/ struct mux *ctx, struct MUX_TINYAVR *base);

int mux_tinyavr_input(struct mux *ctx, size_t pin);
int mux_tinyavr_output(struct mux *ctx, size_t pin);

int mux_tinyavr_pull_up(struct mux *ctx, size_t pin);

#endif

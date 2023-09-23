#ifndef MUX_RENESAS_RA4_H
#define MUX_RENESAS_RA4_H

#include "mux.h"
#include <stddef.h>

#define MUX_RENESAS_RA4_PORT_COUNT 10
#define MUX_RENESAS_RA4_PIN_COUNT  16
#define MUX_RENESAS_RA4_PSEL_COUNT 32

struct MUX_RENESAS_RA4;

struct mux {
    /*@temp@*/ struct MUX_RENESAS_RA4 *base;
    size_t port;
};

int mux_renesas_ra4_init(/*@out@*/ struct mux *ctx, int base, size_t port);

int mux_renesas_ra4_output(struct mux *ctx, size_t pin);
int mux_renesas_ra4_input(struct mux *ctx, size_t pin);
int mux_renesas_ra4_analog(struct mux *ctx, size_t pin);
int mux_renesas_ra4_periph(struct mux *ctx, size_t pin, size_t psel);

int mux_renesas_ra4_pull_up(struct mux *ctx, size_t pin);

#endif

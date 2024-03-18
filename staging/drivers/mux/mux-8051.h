#ifndef MUX_8051_H
#define MUX_8051_H

#include "mux.h"
#include <stdint.h>
#include <stddef.h>

#define MUX_8051_PIN_COUNT 8

typedef enum {
    MUX_8051_MODE_QUASIBDIR = 0,
    MUX_8051_MODE_PUSHPULL  = 1,
    MUX_8051_MODE_INPUTONLY = 2,
    MUX_8051_MODE_OPENDRAIN = 3,
    MUX_8051_MODE_COUNT
} mux_8051_mode_t;

struct mux {
    size_t port;
};

int mux_8051_init(/*@out@*/ struct mux *ctx, size_t port);
int mux_8051_mode(struct mux *ctx, size_t pin, mux_8051_mode_t mode);

#endif

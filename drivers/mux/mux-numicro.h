#ifndef MUX_NUMICRO_H
#define MUX_NUMICRO_H

#include "mux.h"
#include <stdint.h>
#include <stddef.h>

#define MUX_NUMICRO_PORT_COUNT 4
#define MUX_NUMICRO_PIN_COUNT 8

typedef enum {
    MUX_NUMICRO_MODE_QUASIBDIR  = 0,
    MUX_NUMICRO_MODE_PUSHPULL   = 1,
    MUX_NUMICRO_MODE_INPUTONLY  = 2,
    MUX_NUMICRO_MODE_OPENDRAIN  = 3,
    MUX_NUMICRO_MODE_COUNT
} mux_numicro_mode_t;

struct mux {
    size_t port;
};

int mux_numicro_init(/*@out@*/ struct mux *ctx, size_t port);
int mux_numicro_mode(struct mux *ctx, size_t pin, mux_numicro_mode_t mode);

#endif

#ifndef MUX_N76E003_H
#define MUX_N76E003_H

#include "mux.h"
#include <stdint.h>
#include <stddef.h>

#define MUX_N76E003_PORT_COUNT 4
#define MUX_N76E003_PIN_COUNT 8

typedef enum {
    MUX_N76E003_MODE_QUASIBDIR  = 0,
    MUX_N76E003_MODE_PUSHPULL   = 1,
    MUX_N76E003_MODE_INPUTONLY  = 2,
    MUX_N76E003_MODE_OPENDRAIN  = 3,
    MUX_N76E003_MODE_COUNT
} mux_n76e003_mode_t;

struct mux {
    size_t port;
};

int mux_n76e003_init(/*@out@*/ struct mux *ctx, size_t port);
int mux_n76e003_mode(struct mux *ctx, size_t pin, mux_n76e003_mode_t mode);

#endif

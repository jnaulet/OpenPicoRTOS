#ifndef MUX_CH552_H
#define MUX_CH552_H

#include "mux.h"
#include <stdint.h>
#include <stddef.h>

#define MUX_CH552_PIN_COUNT 8

typedef enum {
    MUX_CH552_PORT_P1,
    MUX_CH552_PORT_P2,
    MUX_CH552_PORT_P3,
    MUX_CH552_PORT_COUNT
} mux_ch552_port_t;

typedef enum {
    MUX_CH552_MODE_HI_INPUT     = 0,
    MUX_CH552_MODE_PUSHPULL     = 1,
    MUX_CH552_MODE_OPENDRAIN    = 2,
    MUX_CH552_MODE_QUASIBDIR    = 3,
    MUX_CH552_MODE_COUNT
} mux_ch552_mode_t;

struct mux {
    mux_ch552_port_t port;
};

int mux_ch552_init(/*@out@*/ struct mux *ctx, mux_ch552_port_t port);
int mux_ch552_mode(struct mux *ctx, size_t pin, mux_ch552_mode_t mode);

/* TODO: PIN_FUNC */

#endif

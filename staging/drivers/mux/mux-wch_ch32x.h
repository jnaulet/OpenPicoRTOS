#ifndef MUX_WCH_CH32X_H
#define MUX_WCH_CH32X_H

#include "mux.h"
#include <stddef.h>

#define MUX_WCH_CH32X_PIN_COUNT 16

struct MUX_WCH_CH32X;

struct mux {
    /*@temp@*/ struct MUX_WCH_CH32X *base;
};

int mux_wch_ch32x_init(/*@out@*/ struct mux *ctx, int base);

typedef enum {
    MUX_WCH_CH32X_INPUT_ANALOG          = 0,
    MUX_WCH_CH32X_INPUT_FLOATING        = 1,
    MUX_WCH_CH32X_INPUT_PULL_UP_DOWN    = 2,
    MUX_WCH_CH32X_INPUT_COUNT
} mux_wch_ch32x_input_t;

typedef enum {
    MUX_WCH_CH32X_OUTPUT_PUSH_PULL      = 0,
    MUX_WCH_CH32X_OUTPUT_OPEN_DRAIN     = 1,
    MUX_WCH_CH32X_OUTPUT_MUX_PUSH_PULL  = 2,
    MUX_WCH_CH32X_OUTPUT_MUX_OPEN_DRAIN = 3,
    MUX_WCH_CH32X_OUTPUT_COUNT
} mux_wch_ch32x_output_t;

int mux_wch_ch32x_output(struct mux *ctx, size_t pin, mux_wch_ch32x_output_t type);
int mux_wch_ch32x_input(struct mux *ctx, size_t pin, mux_wch_ch32x_input_t type);

#endif

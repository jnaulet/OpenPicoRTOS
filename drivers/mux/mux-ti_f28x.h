#ifndef MUX_TI_F28X_H
#define MUX_TI_F28X_H

/* MUX driver for TI F28x family */

#include "mux.h"
#include <stddef.h>

#define MUX_TI_F28X_PIN_COUNT 32

struct GPIO_CTRL_REGS;

struct mux {
    /*@temp@*/ struct GPIO_CTRL_REGS *base;
};

typedef enum {
    MUX_TI_F28X_GPIO    = 0,
    MUX_TI_F28X_ALT1    = 1,
    MUX_TI_F28X_ALT2    = 2,
    MUX_TI_F28X_ALT3    = 3,
    MUX_TI_F28X_COUNT
} mux_ti_f28x_t;

typedef enum {
    MUX_TI_F28X_QSEL_SYNC   = 0,
    MUX_TI_F28X_QSEL_0_1    = 1,
    MUX_TI_F28X_QSEL_1_0    = 2,
    MUX_TI_F28X_QSEL_ASYNC  = 3,
    MUX_TI_F28X_QSEL_COUNT
} mux_ti_f28x_qsel_t;

int mux_ti_f28x_init(/*@out@*/ struct mux *ctx, struct GPIO_CTRL_REGS *base);

int mux_ti_f28x_output(struct mux *ctx, size_t pin, mux_ti_f28x_t gmux, mux_ti_f28x_t mux);
int mux_ti_f28x_input(struct mux *ctx, size_t pin, mux_ti_f28x_t gmux, mux_ti_f28x_t mux);

int mux_ti_f28x_pull_up(struct mux *ctx, size_t pin);
int mux_ti_f28x_set_qsel(struct mux *ctx, size_t pin, mux_ti_f28x_qsel_t qsel);

#endif

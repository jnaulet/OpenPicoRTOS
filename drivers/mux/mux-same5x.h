#ifndef MUX_SAME5X_H
#define MUX_SAME5X_H

#include "mux.h"

struct MUX_SAME5X; /* PORT */

#define MUX_SAME5X_PIN_COUNT 32

struct mux_same5x {
    /*@temp@*/ struct MUX_SAME5X *base;
};

typedef enum {
    MUX_PMUX_A      = 0,
    MUX_PMUX_B      = 1,
    MUX_PMUX_C      = 2,
    MUX_PMUX_D      = 3,
    MUX_PMUX_E      = 4,
    MUX_PMUX_F      = 5,
    MUX_PMUX_G      = 6,
    MUX_PMUX_H      = 7,
    MUX_PMUX_I      = 8,
    MUX_PMUX_J      = 9,
    MUX_PMUX_K      = 10,
    MUX_PMUX_L      = 11,
    MUX_PMUX_M      = 12,
    MUX_PMUX_N      = 13,
    MUX_PMUX_GPIO   = 14,
    MUX_PMUX_COUNT
} mux_pmux_t;

int mux_same5x_init(/*@out@*/ struct mux_same5x *ctx, struct MUX_SAME5X *base);

int mux_same5x_input(struct mux_same5x *ctx, size_t pin, mux_pmux_t pmux);
int mux_same5x_output(struct mux_same5x *ctx, size_t pin, mux_pmux_t pmux);

int mux_same5x_pull_up(struct mux_same5x *ctx, size_t pin);
int mux_same5x_pull_down(struct mux_same5x *ctx, size_t pin);

#endif

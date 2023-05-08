#ifndef MUX_GD32VF103_H
#define MUX_GD32VF103_H

#include "mux.h"
#include <stddef.h>

#define MUX_GD32VF103_PIN_COUNT 16

struct MUX_GD32VF103_GPIO;

struct mux {
    /*@temp@*/ struct MUX_GD32VF103_GPIO *base;
};

int mux_gd32vf103_init(/*@out@*/ struct mux *ctx, int base);

typedef enum {
    MUX_GD32VF103_MODE_PUSH_PULL    = 0,
    MUX_GD32VF103_MODE_OPEN_DRAIN   = 1,
    MUX_GD32VF103_MODE_COUNT
} mux_gd32vf103_mode_t;

int mux_gd32vf103_analog(struct mux *ctx, size_t pin);
int mux_gd32vf103_input(struct mux *ctx, size_t pin, mux_gd32vf103_mode_t mode);
int mux_gd32vf103_output(struct mux *ctx, size_t pin, mux_gd32vf103_mode_t mode);
int mux_gd32vf103_afio(struct mux *ctx, size_t pin, mux_gd32vf103_mode_t mode);

/* TODO: afio / remap */

#endif

#ifndef MUX_NXP_SIU_H
#define MUX_NXP_SIU_H

#include "mux.h"
#include <stddef.h>

typedef enum {
    MUX_NXP_SIU_GPIO    = 0,
    MUX_NXP_SIU_ALT1    = 1,
    MUX_NXP_SIU_ALT2    = 2,
    MUX_NXP_SIU_ALT3    = 3,
    MUX_NXP_SIU_COUNT
} mux_nxp_siu_t;

struct MUX_NXP_SIU;

struct mux {
    /*@temp@*/ struct MUX_NXP_SIU *base;
};

int mux_nxp_siu_init(/*@out@*/ struct mux *ctx, int base);

int mux_nxp_siu_output(struct mux *ctx, size_t pin, mux_nxp_siu_t mux);
int mux_nxp_siu_input(struct mux *ctx, size_t pin, mux_nxp_siu_t mux);

/* TODO: IMUX */

int mux_nxp_siu_pull_up(struct mux *ctx, size_t pin);
int mux_nxp_siu_pull_down(struct mux *ctx, size_t pin);

#endif

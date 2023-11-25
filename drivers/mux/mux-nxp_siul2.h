#ifndef MUX_NXP_SIUL2_H
#define MUX_NXP_SIUL2_H

#include "mux.h"
#include <stddef.h>

/*
 * see IO_Signal_Description_and_Input_Multiplexing_Tables document from NXP
 * for more info
 */

typedef enum {
    MUX_NXP_SIUL2_GPIO  = 0,
    MUX_NXP_SIUL2_ALT1  = 1,
    MUX_NXP_SIUL2_ALT2  = 2,
    MUX_NXP_SIUL2_ALT3  = 3,
    MUX_NXP_SIUL2_ALT4  = 4,
    MUX_NXP_SIUL2_ALT5  = 5,
    MUX_NXP_SIUL2_COUNT
} mux_nxp_siul2_t;

/* TODO: move to devices ? */
#define MUX_NXP_SIUL2_PIN_COUNT  264
#define MUX_NXP_SIUL2_IMCR_COUNT 512

struct MUX_NXP_SIUL2;

struct mux {
    /*@temp@*/ struct MUX_NXP_SIUL2 *base;
};

int mux_nxp_siul2_init(/*@out@*/ struct mux *ctx, int base);

int mux_nxp_siul2_output(struct mux *ctx, size_t pin, mux_nxp_siul2_t mux);
int mux_nxp_siul2_input(struct mux *ctx, size_t pin, mux_nxp_siul2_t mux, size_t imcr);
int mux_nxp_siul2_analog(struct mux *ctx, size_t pin);

int mux_nxp_siul2_pull_up(struct mux *ctx, size_t pin);
int mux_nxp_siul2_pull_down(struct mux *ctx, size_t pin);

#endif

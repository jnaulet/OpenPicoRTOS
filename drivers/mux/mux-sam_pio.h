#ifndef MUX_SAM_PIO_H
#define MUX_SAM_PIO_H

#include "mux.h"

struct MUX_SAM_PIO;

#define MUX_SAM_PIN_COUNT 32

struct mux_sam_pio {
    /*@temp@*/ struct MUX_SAM_PIO *base;
};

typedef enum {
    MUX_SAM_PIO_DISABLE,
    MUX_SAM_PIO_GPIO,
    MUX_SAM_PIO_A,
    MUX_SAM_PIO_B,
    MUX_SAM_PIO_COUNT
} mux_sam_pio_t;

int mux_sam_pio_init(/*@out@*/ struct mux_sam_pio *ctx, struct MUX_SAM_PIO *base);

int mux_sam_pio_input(struct mux_sam_pio *ctx, size_t pin, mux_sam_pio_t mux);
int mux_sam_pio_output(struct mux_sam_pio *ctx, size_t pin, mux_sam_pio_t mux);

int mux_sam_pio_pull_up(struct mux_sam_pio *ctx, size_t pin);

#endif

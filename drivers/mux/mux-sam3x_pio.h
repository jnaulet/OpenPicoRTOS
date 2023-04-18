#ifndef MUX_SAM3X_PIO_H
#define MUX_SAM3X_PIO_H

#include "mux.h"

struct MUX_SAM3X_PIO;

#define MUX_SAM3X_PIN_COUNT 32

struct mux_sam3x_pio {
    /*@temp@*/ struct MUX_SAM3X_PIO *base;
};

typedef enum {
    MUX_SAM3X_PIO_DISABLE,
    MUX_SAM3X_PIO_GPIO,
    MUX_SAM3X_PIO_A,
    MUX_SAM3X_PIO_B,
    MUX_SAM3X_PIO_COUNT
} mux_sam3x_pio_t;

int mux_sam3x_pio_init(/*@out@*/ struct mux_sam3x_pio *ctx, struct MUX_SAM3X_PIO *base);

int mux_sam3x_pio_input(struct mux_sam3x_pio *ctx, size_t pin, mux_sam3x_pio_t mux);
int mux_sam3x_pio_output(struct mux_sam3x_pio *ctx, size_t pin, mux_sam3x_pio_t mux);

int mux_sam3x_pio_pull_up(struct mux_sam3x_pio *ctx, size_t pin);

#endif

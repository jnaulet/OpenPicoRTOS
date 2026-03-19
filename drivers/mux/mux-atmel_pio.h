#ifndef MUX_ATMEL_PIO_H
#define MUX_ATMEL_PIO_H

#include "mux.h"

struct MUX_ATMEL_PIO;

#define MUX_ATMEL_PIN_COUNT 32

struct mux {
    /*@temp@*/ struct MUX_ATMEL_PIO *base;
};

typedef enum {
    MUX_ATMEL_PIO_DISABLE,
    MUX_ATMEL_PIO_GPIO,
    MUX_ATMEL_PIO_A,
    MUX_ATMEL_PIO_B,
    MUX_ATMEL_PIO_C,
    MUX_ATMEL_PIO_D,
    MUX_ATMEL_PIO_COUNT
} mux_atmel_pio_t;

int mux_atmel_pio_init(/*@out@*/ struct mux *ctx, int base);

int mux_atmel_pio_input(struct mux *ctx, size_t pin, mux_atmel_pio_t mux);
int mux_atmel_pio_output(struct mux *ctx, size_t pin, mux_atmel_pio_t mux);

int mux_atmel_pio_pull_up(struct mux *ctx, size_t pin);

#endif

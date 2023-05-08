#ifndef MUX_RP2040_H
#define MUX_RP2040_H

#include "mux.h"
#include <stddef.h>
#include <stdbool.h>

struct MUX_RP2040_IO;
struct MUX_RP2040_PADS;

#define MUX_RP2040_PIN_COUNT     30

struct mux {
    /*@temp@*/ struct MUX_RP2040_IO *io;
    /*@temp@*/ struct MUX_RP2040_PADS *pads;
};

typedef enum {
    MUX_RP2040_IGNORE   = 0,
    MUX_RP2040_F1       = 1,
    MUX_RP2040_F2       = 2,
    MUX_RP2040_F3       = 3,
    MUX_RP2040_F4       = 4,
    MUX_RP2040_F5       = 5,
    MUX_RP2040_F6       = 6,
    MUX_RP2040_F7       = 7,
    MUX_RP2040_F8       = 8,
    MUX_RP2040_F9       = 9,
    MUX_RP2040_COUNT
} mux_rp2040_t;

typedef enum {
    MUX_RP2040_OVER_NORMAL      = 0,
    MUX_RP2040_OVER_INVERT      = 1,
    MUX_RP2040_OVER_DRIVE_LOW   = 2,
    MUX_RP2040_OVER_DRIVE_HIGH  = 3,
    MUX_RP2040_OVER_COUNT
} mux_rp2040_over_t;

typedef enum {
    MUX_RP2040_DRIVE_2mA    = 0,
    MUX_RP2040_DRIVE_4mA    = 1,
    MUX_RP2040_DRIVE_8mA    = 2,
    MUX_RP2040_DRIVE_12mA   = 3,
    MUX_RP2040_DRIVE_COUNT
} mux_rp2040_drive_t;

int mux_rp2040_init(/*@out@*/ struct mux *ctx, int io_base, int pads_base);

int mux_rp2040_io(struct mux *ctx, size_t pin, mux_rp2040_t funcsel);
int mux_rp2040_input(struct mux *ctx, size_t pin, mux_rp2040_t funcsel);
int mux_rp2040_output(struct mux *ctx, size_t pin, mux_rp2040_t funcsel);

int mux_rp2040_pull_up(struct mux *ctx, size_t pin);
int mux_rp2040_pull_down(struct mux *ctx, size_t pin);
int mux_rp2040_override(struct mux *ctx, size_t pin, mux_rp2040_over_t over);
int mux_rp2040_set_drive(struct mux *ctx, size_t pin, mux_rp2040_drive_t drive);

#endif

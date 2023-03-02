#ifndef MUX_TINYAVR_H
#define MUX_TINYAVR_H

#include "mux.h"
#include <stddef.h>
#include <stdbool.h>

struct MUX_TINYAVR;

#define MUX_TINYAVR_PIN_COUNT 8

struct mux {
    /*@temp@*/ struct MUX_TINYAVR *base;
};

int mux_tinyavr_init(/*@out@*/ struct mux *ctx, struct MUX_TINYAVR *base);

int mux_tinyavr_input(struct mux *ctx, size_t pin);
int mux_tinyavr_output(struct mux *ctx, size_t pin);

int mux_tinyavr_pull_up(struct mux *ctx, size_t pin);

typedef enum {
    /* CTRLA */
    MUX_TINYAVR_ALT_EVOUT0  = 0x0,
    MUX_TINYAVR_ALT_EVOUT1  = 0x1,
    MUX_TINYAVR_ALT_EVOUT2  = 0x2,
    MUX_TINYAVR_ALT_LUT0    = 0x4,
    MUX_TINYAVR_ALT_LUT1    = 0x5,
    /* CTRLB */
    MUX_TINYAVR_ALT_USART0  = 0x10,
    MUX_TINYAVR_ALT_SPI0    = 0x12,
    /* CTRLC */
    MUX_TINYAVR_ALT_TCA00   = 0x20,
    MUX_TINYAVR_ALT_TCA01   = 0x21,
    MUX_TINYAVR_ALT_TCA02   = 0x22,
    MUX_TINYAVR_ALT_TCA03   = 0x23,
    MUX_TINYAVR_ALT_TCA04   = 0x24,
    MUX_TINYAVR_ALT_TCA05   = 0x25,
    /* CTRLD */
    MUX_TINYAVR_ALT_TCB00   = 0x30,
    /* END */
    MUX_TINYAVR_ALT_COUNT
} mux_tinyavr_alt_t;

int mux_tinyavr_select_alt_pin(mux_tinyavr_alt_t alt);

#endif

#ifndef MUX_MC9S08PTXX_H
#define MUX_MC9S08PTXX_H

#include "mux.h"
#include <stdint.h>
#include <stddef.h>

#define MUX_MC9S08PTXX_PORT_COUNT 8
#define MUX_MC9S08PTXX_PIN_COUNT  8

#define MUX_MC9S08PTXX_PORT_A 0
#define MUX_MC9S08PTXX_PORT_B 1
#define MUX_MC9S08PTXX_PORT_C 2
#define MUX_MC9S08PTXX_PORT_D 3
#define MUX_MC9S08PTXX_PORT_E 4
#define MUX_MC9S08PTXX_PORT_F 5
#define MUX_MC9S08PTXX_PORT_G 6
#define MUX_MC9S08PTXX_PORT_H 7

struct mux {
    /*@temp@*/ volatile uint8_t *PTxOE;
    /*@temp@*/ volatile uint8_t *PTxIE;
    /*@temp@*/ volatile uint8_t *PTxPE;
};

int mux_mc9s08ptxx_init(/*@out@*/ struct mux *ctx, size_t port);

int mux_mc9s08ptxx_output(struct mux *ctx, size_t pin);
int mux_mc9s08ptxx_input(struct mux *ctx, size_t pin);

int mux_mc9s08ptxx_pull_up(struct mux *ctx, size_t pin);

/*
 * TODO: pin remappings
 */

#endif

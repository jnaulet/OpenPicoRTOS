#ifndef MUX_MSP430G2XXX_H
#define MUX_MSP430G2XXX_H

#include "mux.h"

#include <stdint.h>
#include <stddef.h>

#define MUX_MSP430G2XXX_PORT_COUNT 8
#define MUX_MSP430G2XXX_PIN_COUNT 8

struct MUX_MSP430G2XXX;

struct mux {
    /*@temp@*/ volatile uint8_t *PxOUT;
    /*@temp@*/ volatile uint8_t *PxDIR;
    /*@temp@*/ volatile uint8_t *PxSEL;
    /*@temp@*/ volatile uint8_t *PxSEL2;
    /*@temp@*/ volatile uint8_t *PxREN;
};

int mux_msp430g2xxx_init(/*@out@*/ struct mux *ctx, int base, size_t port);

typedef enum {
    MUX_MSP430G2XXX_PXSEL_GPIO  = 0,
    MUX_MSP430G2XXX_PXSEL_FUNC1 = 1,
    MUX_MSP430G2XXX_PXSEL_FUNC2 = 2,
    MUX_MSP430G2XXX_PXSEL_FUNC3 = 3,
    MUX_MSP430G2XXX_PXSEL_COUNT
} mux_msp430g2xxx_pxsel_t;

int mux_msp430g2xxx_input(struct mux *ctx, size_t pin, mux_msp430g2xxx_pxsel_t pxsel);
int mux_msp430g2xxx_output(struct mux *ctx, size_t pin, mux_msp430g2xxx_pxsel_t pxsel);

int mux_msp430g2xxx_pull_up(struct mux *ctx, size_t pin);
int mux_msp430g2xxx_pull_down(struct mux *ctx, size_t pin);

#endif

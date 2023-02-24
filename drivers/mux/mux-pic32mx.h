#ifndef MUX_PIC32MX_H
#define MUX_PIC32MX_H

#include "mux.h"
#include <stddef.h>

struct MUX_PIC32MX;

#define MUX_PIC32MX_PIN_COUNT 16

struct mux_pic32mx {
    /*@temp@*/ struct MUX_PIC32MX *base;
};

typedef enum {
    MUX_PIC32MX_PPSI_INT1R      = 0,
    MUX_PIC32MX_PPSI_INT2R      = 1,
    MUX_PIC32MX_PPSI_INT3R      = 2,
    MUX_PIC32MX_PPSI_INT4R      = 3,
    MUX_PIC32MX_PPSI_T2CKR      = 4,
    MUX_PIC32MX_PPSI_T3CKR      = 5,
    MUX_PIC32MX_PPSI_T4CKR      = 6,
    MUX_PIC32MX_PPSI_T5CKR      = 7,
    MUX_PIC32MX_PPSI_T6CKR      = 8,
    MUX_PIC32MX_PPSI_T7CKR      = 9,
    MUX_PIC32MX_PPSI_T8CKR      = 10,
    MUX_PIC32MX_PPSI_T9CKR      = 11,
    MUX_PIC32MX_PPSI_ICxR       = 12,
    MUX_PIC32MX_PPSI_IC1R       = 13,
    MUX_PIC32MX_PPSI_IC2R       = 14,
    MUX_PIC32MX_PPSI_IC3R       = 15,
    MUX_PIC32MX_PPSI_IC4R       = 16,
    MUX_PIC32MX_PPSI_IC5R       = 17,
    MUX_PIC32MX_PPSI_IC6R       = 18,
    MUX_PIC32MX_PPSI_IC7R       = 19,
    MUX_PIC32MX_PPSI_IC8R       = 20,
    MUX_PIC32MX_PPSI_IC9R       = 21,
    /* ! */
    MUX_PIC32MX_PPSI_OCFAR      = 23,
    /* ! */
    MUX_PIC32MX_PPSI_U1RXR      = 25,
    MUX_PIC32MX_PPSI_U1CTSR     = 26,
    MUX_PIC32MX_PPSI_U2RXR      = 27,
    MUX_PIC32MX_PPSI_U2CTSR     = 28,
    MUX_PIC32MX_PPSI_U3RXR      = 29,
    MUX_PIC32MX_PPSI_U3CTSR     = 30,
    MUX_PIC32MX_PPSI_U4RXR      = 31,
    MUX_PIC32MX_PPSI_U4CTSR     = 32,
    MUX_PIC32MX_PPSI_U5RXR      = 33,
    MUX_PIC32MX_PPSI_U5CTSR     = 34,
    MUX_PIC32MX_PPSI_U6RXR      = 35,
    MUX_PIC32MX_PPSI_U6CTSR     = 36,
    /* ! */
    MUX_PIC32MX_PPSI_SDI1R      = 38,
    MUX_PIC32MX_PPSI_SS1R       = 39,
    /* ! */
    MUX_PIC32MX_PPSI_SDI2R      = 41,
    MUX_PIC32MX_PPSI_SS2R       = 42,
    /* ! */
    MUX_PIC32MX_PPSI_SDI3R      = 44,
    MUX_PIC32MX_PPSI_SS3R       = 45,
    /* ! */
    MUX_PIC32MX_PPSI_SDI4R      = 47,
    MUX_PIC32MX_PPSI_SS4R       = 48,
    /* ! */
    MUX_PIC32MX_PPSI_SDI5R      = 50,
    MUX_PIC32MX_PPSI_SS5R       = 51,
    /* ! */
    MUX_PIC32MX_PPSI_SDI6R      = 53,
    MUX_PIC32MX_PPSI_SS6R       = 54,
    /* ! */
    MUX_PIC32MX_PPSI_C1RXR      = 55,
    MUX_PIC32MX_PPSI_C2RXR      = 56,
    MUX_PIC32MX_PPSI_REFCLKI1R  = 57,
    /* ! */
    MUX_PIC32MX_PPSI_REFCLKI3R  = 59,
    MUX_PIC32MX_PPSI_REFCLKI5R  = 60,
    MUX_PIC32MX_PPSI_COUNT
} mux_pic32mx_ppsi_t;

typedef enum {
    MUX_PIC32MX_PPSO_RPA14R = 0,
    MUX_PIC32MX_PPSO_RPA15R = 1,
    /* ! */
    MUX_PIC32MX_PPSO_RPB1R  = 3,
    MUX_PIC32MX_PPSO_RPB2R  = 4,
    MUX_PIC32MX_PPSO_RPB3R  = 5,
    /* ! */
    MUX_PIC32MX_PPSO_RPB5R  = 7,
    MUX_PIC32MX_PPSO_RPB6R  = 8,
    MUX_PIC32MX_PPSO_RPB7R  = 9,
    MUX_PIC32MX_PPSO_RPB8R  = 10,
    MUX_PIC32MX_PPSO_RPB9R  = 11,
    MUX_PIC32MX_PPSO_RPB10R = 12,
    /* ! */
    MUX_PIC32MX_PPSO_RPB14R = 16,
    MUX_PIC32MX_PPSO_RPB15R = 17,
    /* ! */
    MUX_PIC32MX_PPSO_RPC1R  = 19,
    MUX_PIC32MX_PPSO_RPC2R  = 20,
    MUX_PIC32MX_PPSO_RPC3R  = 21,
    MUX_PIC32MX_PPSO_RPC4R  = 22,
    /* ! */
    MUX_PIC32MX_PPSO_RPC13R = 31,
    MUX_PIC32MX_PPSO_RPC14R = 32,
    /* ! */
    MUX_PIC32MX_PPSO_RPD0R  = 34,
    MUX_PIC32MX_PPSO_RPD1R  = 35,
    MUX_PIC32MX_PPSO_RPD2R  = 36,
    MUX_PIC32MX_PPSO_RPD3R  = 37,
    MUX_PIC32MX_PPSO_RPD4R  = 38,
    MUX_PIC32MX_PPSO_RPD5R  = 39,
    MUX_PIC32MX_PPSO_RPD6R  = 40,
    MUX_PIC32MX_PPSO_RPD7R  = 41,
    /* ! */
    MUX_PIC32MX_PPSO_RPD9R  = 43,
    MUX_PIC32MX_PPSO_RPD10R = 44,
    MUX_PIC32MX_PPSO_RPD11R = 45,
    MUX_PIC32MX_PPSO_RPD12R = 46,
    /* ! */
    MUX_PIC32MX_PPSO_RPD14R = 48,
    MUX_PIC32MX_PPSO_RPD15R = 49,
    /* ! */
    MUX_PIC32MX_PPSO_RPE3R  = 53,
    /* ! */
    MUX_PIC32MX_PPSO_RPE5R  = 55,
    /* ! */
    MUX_PIC32MX_PPSO_RPE8R  = 58,
    MUX_PIC32MX_PPSO_RPE9R  = 59,
    /* ! */
    MUX_PIC32MX_PPSO_RPF0R  = 66,
    MUX_PIC32MX_PPSO_RPF1R  = 67,
    MUX_PIC32MX_PPSO_RPF2R  = 68,
    MUX_PIC32MX_PPSO_RPF3R  = 69,
    MUX_PIC32MX_PPSO_RPF4R  = 70,
    MUX_PIC32MX_PPSO_RPF5R  = 71,
    /* ! */
    MUX_PIC32MX_PPSO_RPF8R  = 74,
    /* ! */
    MUX_PIC32MX_PPSO_RPF12R = 78,
    MUX_PIC32MX_PPSO_RPF13R = 79,
    /* ! */
    MUX_PIC32MX_PPSO_RPG0R  = 82,
    MUX_PIC32MX_PPSO_RPG1R  = 83,
    /* ! */
    MUX_PIC32MX_PPSO_RPG6R  = 88,
    MUX_PIC32MX_PPSO_RPG7R  = 89,
    MUX_PIC32MX_PPSO_RPG8R  = 90,
    MUX_PIC32MX_PPSO_RPG9R  = 91,
    MUX_PIC32MX_PPSO_COUNT
} mux_pic32mx_ppso_t;

int mux_pic32mx_init(/*@out@*/ struct mux_pic32mx *ctx, struct MUX_PIC32MX *base);

int mux_pic32mx_analog(struct mux_pic32mx *ctx, size_t pin);
int mux_pic32mx_input(struct mux_pic32mx *ctx, size_t pin);
int mux_pic32mx_output(struct mux_pic32mx *ctx, size_t pin);

/* PPS calls */
#define MUX_PIC32MX_PPSx_OPT_COUNT 16

int mux_pic32mx_peripheral_pin_select_input(mux_pic32mx_ppsi_t ppsi, size_t opt);
int mux_pic32mx_peripheral_pin_select_output(mux_pic32mx_ppso_t ppso, size_t opt);

int mux_pic32mx_pull_up(struct mux_pic32mx *ctx, size_t pin);
int mux_pic32mx_pull_down(struct mux_pic32mx *ctx, size_t pin);

#endif

#ifndef LIN_NXP_LINFLEXD_H
#define LIN_NXP_LINFLEXD_H

#include "lin.h"
#include "clock.h"

typedef enum {
    LIN_NXP_LINFLEXD_STATE_IDLE,
    LIN_NXP_LINFLEXD_STATE_XFER,
    LIN_NXP_LINFLEXD_STATE_COUNT
} lin_nxp_linflexd_state;

struct LIN_NXP_LINFLEXD;

struct lin {
    /*@temp@*/ struct LIN_NXP_LINFLEXD *base;
    clock_id_t clkid;
    lin_mode_t mode;
    lin_nxp_linflexd_state state;
};

int lin_nxp_linflexd_init(/*@out@*/ struct lin *ctx, int base, clock_id_t clkid);

/* Implements:
 * int lin_setup(struct lin *ctx, struct lin_settings *settings);
 * int lin_write(struct lin *ctx, uint8_t id, const void *buf, size_t n):
 * int lin_read(struct lin *ctx, uint8_t *id, void *buf, size_t n);
 */

#endif

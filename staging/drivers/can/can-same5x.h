#ifndef CAN_SAME5X_H
#define CAN_SAME5X_H

#include "can.h"
#include "clock.h"
#include <stdint.h>

/* FIXME: forced to max values */
#define CAN_SAME5X_SIDF_WORD_COUNT 128
#define CAN_SAME5X_RXF0_WORD_COUNT 1152
#define CAN_SAME5X_TXB_WORD_COUNT  576

struct CAN_SAME5X;

struct can {
    /*@temp@*/ struct CAN_SAME5X *base;
    clock_id_t clkid;
    /* counters */
    size_t sidf_count;
    /* can only be located in the first 64k of RAM, might need
     * to alter the linker files at some point */
    uint32_t sidf[CAN_SAME5X_SIDF_WORD_COUNT];
    uint32_t rxf0[CAN_SAME5X_RXF0_WORD_COUNT];
    uint32_t txb[CAN_SAME5X_TXB_WORD_COUNT];
};

int can_same5x_init(/*@out@*/ struct can *ctx, struct CAN_SAME5X *base, clock_id_t clkid);

/* implements:
 * int can_setup(struct can *ctx, struct can_settings *settings);
 * int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask);
 * int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n);
 * int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n);
 * int can_request_frame(struct can *ctx, can_id_t id);
 */

#endif

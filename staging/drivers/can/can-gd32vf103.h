#ifndef CAN_GD32VF103_H
#define CAN_GD32VF103_H

#include "can.h"
#include "clock.h"

struct CAN_GD32VF103;

struct can {
    /*@temp@*/ struct CAN_GD32VF103 *base;
    clock_id_t clkid;
    /* filters (CAN0) */
    size_t filter_count;
};

int can_gd32vf103_init(/*@out@*/ struct can *ctx, int base, clock_id_t clkid);

/* Implements:
 * int can_setup(struct can *ctx, struct can_settings *settings);
 * int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask);
 * int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n);
 * int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n);
 * int can_request_frame(struct can *ctx, can_id_t id);
 */

#endif

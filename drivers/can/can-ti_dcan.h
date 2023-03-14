#ifndef CAN_TI_DCAN_H
#define CAN_TI_DCAN_H

#include "can.h"
#include "clock.h"

struct CAN_REGS;

#include <stdbool.h>

#define CAN_TI_DCAN_MB_COUNT 32

struct can {
    /*@temp@*/ struct CAN_REGS *base;
    clock_id_t clkid;
    /* mb allocation */
    size_t rx_mailbox_index;
    size_t rx_mailbox_count;
    size_t tx_mailbox_index;
    /* settings */
    size_t tx_mailbox_count;
    bool tx_auto_abort;
    bool rx_overwrite;
};

int can_ti_dcan_init(/*@out@*/ struct can *ctx, struct CAN_REGS *base, clock_id_t clkid);

/* Runtime calls:
 * int can_setup(struct can *ctx, struct can_settings *settings);
 * int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask);
 * int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n);
 * int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n);
 * int can_request_frame(struct can *ctx, can_id_t id);
 */

#endif

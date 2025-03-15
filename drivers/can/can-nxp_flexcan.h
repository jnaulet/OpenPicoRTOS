#ifndef CAN_NXP_FLEXCAN_H
#define CAN_NXP_FLEXCAN_H

/*
 * Driver for NXP's FlexCAN IP
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "can.h"
#include "clock.h"

struct CAN_NXP_FLEXCAN;

struct can {
    /*@temp@*/ struct CAN_NXP_FLEXCAN *base;
    clock_id_t clkid;
    /* mbs */
    size_t mailbox_count;
    /* mb allocation */
    size_t rx_mailbox_index;
    size_t rx_mailbox_count;
    /* settings */
    size_t tx_mailbox_index;
    size_t tx_mailbox_count;
    bool tx_auto_abort;
    bool rx_overwrite;
    /* timestamp */
    uint32_t timestamp;
};

struct can_nxp_flexcan_settings {
    bool clksrc_is_per_clock;
};

int can_nxp_flexcan_init(/*@out@*/ struct can *ctx, int base, clock_id_t clkid, size_t mailbox_count);
int can_nxp_flexcan_setup(struct can *ctx, const struct can_nxp_flexcan_settings *settings);

/* Runtime calls:
 * int can_register(struct can *ctx, can_id_t id, can_f_t f, unsigned long flags);
 * int can_send(struct can *ctx, can_id_t id, const void *data, size_t n);
 * int can_recv(struct can *ctx, can_id_t *id, void *data, size_t n);
 * int can_abort(struct can *ctx, can_id_t id);
 */

#endif

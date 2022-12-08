#ifndef CAN_SAM_H
#define CAN_SAM_H

#include "can.h"
#include "clock.h"
#include "picoRTOS_futex.h"

struct CAN_SAM;

#define CAN_SAM_MB_COUNT 8

struct can {
    /*@temp@*/ struct CAN_SAM *base;
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

int can_sam_init(/*@out@*/ struct can *ctx, struct CAN_SAM *base, clock_id_t clkid);

/* Runtime calls:
 */

#endif

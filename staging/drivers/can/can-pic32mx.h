#ifndef CAN_PIC32MX_H
#define CAN_PIC32MX_H

#include "can.h"
#include "clock.h"

#include <stdint.h>
#include <stdbool.h>

struct CAN_PIC32MX;

#define CAN_PIC32MX_FIFO_COUNT    32
#define CAN_PIC32MX_FIFO_SIZE_MAX 32

struct can {
    /*@temp@*/ struct CAN_PIC32MX *base;
    clock_id_t clkid;
    /* mb allocation */
    size_t rx_mailbox_count;
    size_t tx_mailbox_index;
    /* filters */
    size_t rxm_count;
    size_t rxf_count;
    /* settings */
    size_t tx_mailbox_count;
    bool tx_auto_abort;
    bool rx_overwrite;
    /* state machine */
    uint32_t opmod;
    uint32_t kva_base;
    /* FIFOs: fixed & maxed out for the moment */
    uint32_t FIFO[CAN_PIC32MX_FIFO_COUNT * CAN_PIC32MX_FIFO_SIZE_MAX * 4];
};

struct can_pic32mx_settings {
    uint32_t *fifo_base;                    /* fifo base address in RAM */
    size_t fsize[CAN_PIC32MX_FIFO_COUNT];   /* min: 1 */
};

int can_pic32mx_init(/*@out@*/ struct can *ctx, int base, clock_id_t clkid);
/* At some point, add a can_pic32mx_setup() to setup all FIFOs */

/* Runtime calls:
 * int can_setup(struct can *ctx, struct can_settings *settings);
 * int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask);
 * int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n);
 * int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n);
 * int can_request_frame(struct can *ctx, can_id_t id);
 */

#endif

#ifndef CAN_STM32H7XX_FDCAN_H
#define CAN_STM32H7XX_FDCAN_H

/*
 * Driver for STMicro FDCAN IP
 */

#include "can.h"
#include "clock.h"

#include <stddef.h>
#include <stdint.h>

struct CAN_STM32H7XX_FDCAN;

struct can {
    /*@temp@*/ struct CAN_STM32H7XX_FDCAN *base;
    clock_id_t clkid;
    /* internals */
    /*@temp@*/ volatile void *filter_11bit;
    /*@temp@*/ volatile void *filter_29bit;
    /*@temp@*/ volatile void *rx_fifo0;
    /*@temp@*/ volatile void *rx_fifo1;
    /*@temp@*/ volatile void *rx_buffer;
    /*@temp@*/ volatile void *tx_event_fifo;
    /*@temp@*/ volatile void *tx_buffers;
    /*@temp@*/ volatile void *trigger_memory;
    /* counters */
    size_t n_filter_11bit;
    size_t n_filter_29bit;
    /* use later */
    size_t n_words;
  /* bus off */
  int bus_off_recovery;
};

int can_stm32h7xx_fdcan_init(/*@out@*/ struct can *ctx, int base, clock_id_t clkid,
                             uint32_t start_address, size_t n_words);

/* Runtime calls:
 * int can_register(struct can *ctx, can_id_t id, can_f_t f, unsigned long flags);
 * int can_send(struct can *ctx, can_id_t id, const void *data, size_t n);
 * int can_recv(struct can *ctx, can_id_t *id, void *data, size_t n);
 * int can_abort(struct can *ctx, can_id_t id);
 */

#endif

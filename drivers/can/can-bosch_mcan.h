#ifndef CAN_BOSCH_MCAN_H
#define CAN_BOSCH_MCAN_H

/*
 * Driver for Bosch M_CAN IP
 * Used on misc microcontrollers, like stm32h7 & Atmel SAMV70
 */

#include "can.h"
#include "clock.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct CAN_BOSCH_MCAN;

struct can {
    /*@temp@*/ struct CAN_BOSCH_MCAN *base;
    clock_id_t clkid;
    /* internals */
    /*@temp@*/ volatile uint32_t *filter_11bit;
    /*@temp@*/ volatile uint32_t *filter_29bit;
    /*@temp@*/ volatile uint32_t *rx_fifo0;
    /*@temp@*/ volatile uint32_t *rx_fifo1;
    /*@temp@*/ volatile uint32_t *rx_buffer;
    /*@temp@*/ volatile uint32_t *tx_event_fifo;
    /*@temp@*/ volatile uint32_t *tx_buffers;
    /*@temp@*/ volatile uint32_t *trigger_memory;
    /* counters */
    size_t n_filter_11bit;
    size_t n_filter_29bit;
    /* use later */
    size_t n_words;
    /* bus off */
    bool bus_off_recovery;
};

int can_bosch_mcan_init(/*@out@*/ struct can *ctx, int base, clock_id_t clkid,
                        uint32_t *message_ram, uint32_t start_address, size_t n_words);

/* Runtime calls:
 * int can_register(struct can *ctx, can_id_t id, can_f_t f, unsigned long flags);
 * int can_send(struct can *ctx, can_id_t id, const void *data, size_t n);
 * int can_recv(struct can *ctx, can_id_t *id, void *data, size_t n);
 * int can_abort(struct can *ctx, can_id_t id);
 */

#endif

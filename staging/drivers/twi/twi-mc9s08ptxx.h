#ifndef TWI_MC9S08PTXX_H
#define TWI_MC9S08PTXX_H

#include "twi.h"
#include "clock.h"

/*
 * This is a variant of twi-nxp_i2c
 * but we prefer not to merge them right now
 */

struct TWI_MC9S08PTXX;

typedef enum {
    TWI_MC9S08PTXX_STATE_IDLE,
    TWI_MC9S08PTXX_STATE_DATA,
    TWI_MC9S08PTXX_STATE_STOP,
    TWI_MC9S08PTXX_STATE_COUNT
} twi_mc9s08ptxx_state_t;

struct twi {
    /*@temp@*/ struct TWI_MC9S08PTXX *base;
    clock_id_t clkid;
    twi_mc9s08ptxx_state_t state;
    twi_mode_t mode;
    twi_addr_t slave_addr;
    int last;
};

int twi_mc9s08ptxx_init(/*@out@*/ struct twi *ctx, int base, clock_id_t clkid);

/* Implements:
 * int twi_setup(struct twi *ctx, struct twi_settings *settings);
 * int twi_poll(struct twi *ctx);
 * int twi_write(struct twi *ctx, const void *buf, size_t n);
 * int twi_read(struct twi *ctx, void *buf, size_t n);
 */

#endif

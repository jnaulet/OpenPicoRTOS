#ifndef TWI_GD32VF103_H
#define TWI_GD32VF103_H

#include "twi.h"
#include "clock.h"

struct TWI_GD32VF103_I2C;

typedef enum {
    TWI_GD32VF103_STATE_IDLE,
    TWI_GD32VF103_STATE_START,
    TWI_GD32VF103_STATE_SLA,
    TWI_GD32VF103_STATE_ACK,
    TWI_GD32VF103_STATE_DATA,
    TWI_GD32VF103_STATE_STOP,
    TWI_GD32VF103_STATE_COUNT
} twi_gd32vf103_state_t;

struct twi {
    /*@temp@*/ struct TWI_GD32VF103_I2C *base;
    clock_id_t clkid;
    twi_mode_t mode;
    twi_gd32vf103_state_t state;
    twi_addr_t slave_addr;
    int last;
};

int twi_gd32vf103_init(/*@out@*/ struct twi *ctx, int base, clock_id_t clkid);

/* Implements:
 * int twi_setup(struct twi *ctx, struct twi_settings *settings);
 * int twi_write(struct twi *ctx, const void *buf, size_t n);
 * int twi_read(struct twi *ctx, void *buf, size_t n);
 */

#endif

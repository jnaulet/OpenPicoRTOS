#ifndef TWI_DW_APB_I2C_H
#define TWI_DW_APB_I2C_H

#include "twi.h"
#include "clock.h"

/*
 * Driver for Synopsis DW_apb_i2c v2.01 IP
 */

struct TWI_DW_APB_I2C;

typedef enum {
    TWI_DW_APB_I2C_STATE_IDLE,
    TWI_DW_APB_I2C_STATE_REQ,
    TWI_DW_APB_I2C_STATE_XFER,
    TWI_DW_APB_I2C_STATE_COUNT
} twi_dw_apb_i2c_state_t;

struct twi {
    /*@temp@*/ struct TWI_DW_APB_I2C *base;
    clock_id_t clkid;
    twi_dw_apb_i2c_state_t state;
    twi_mode_t mode;
    twi_addr_t slave_addr;
    int last;
};

int twi_dw_apb_i2c_init(/*@out@*/ struct twi *ctx, struct TWI_DW_APB_I2C *base, clock_id_t clkid);

/* Runtime calls:
 * int twi_setup(struct twi *ctx, struct twi_settings *settings);
 * int twi_poll(struct twi *ctx);
 * int twi_write(struct twi *ctx, const void *buf, size_t n);
 * int twi_read(struct twi *ctx, void *buf, size_t n);
 */

#endif

#ifndef TWI_NXP_I2C_H
#define TWI_NXP_I2C_H

/* TWI driver for NXP I2C */

#include "twi.h"
#include "clock.h"

struct TWI_NXP_I2C;

typedef enum {
    TWI_NXP_I2C_STATE_IDLE,
    TWI_NXP_I2C_STATE_DATA,
    TWI_NXP_I2C_STATE_STOP,
    TWI_NXP_I2C_STATE_COUNT
} twi_nxp_i2c_state_t;

struct twi {
    /*@temp@*/ struct TWI_NXP_I2C *base;
    clock_id_t clkid;
    twi_nxp_i2c_state_t state;
    twi_mode_t mode;
    twi_addr_t slave_addr;
    int last;
};

int twi_nxp_i2c_init(/*@out@*/ struct twi *ctx, int base, clock_id_t clkid);

#endif

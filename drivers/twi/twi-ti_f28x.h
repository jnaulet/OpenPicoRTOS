#ifndef TWI_TI_F28X_H
#define TWI_TI_F28X_H

#include "twi.h"
#include "clock.h"

struct I2C_REGS;

typedef enum {
    TWI_TI_F28X_STATE_IDLE,
    TWI_TI_F28X_STATE_XFER,
    TWI_TI_F28X_STATE_COUNT
} twi_ti_f28x_state_t;

struct twi {
    /*@temp@*/ struct I2C_REGS *base;
    clock_id_t clkid;
    twi_mode_t mode;
    twi_ti_f28x_state_t state;
};

int twi_ti_f28x_init(/*@out@*/ struct twi *ctx, struct I2C_REGS *base, clock_id_t clkid);

/* Runtime calls:
 * int twi_setup(struct twi *ctx, struct twi_settings *settings);
 *
 * int twi_write(struct twi *ctx, const void *buf, size_t n);
 * int twi_read(struct twi *ctx, void *buf, size_t n);
 */

#endif

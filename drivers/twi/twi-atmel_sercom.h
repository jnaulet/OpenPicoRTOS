#ifndef TWI_ATMEL_SERCOM_H
#define TWI_ATMEL_SERCOM_H

#include "twi.h"
#include "clock.h"

struct TWI_ATMEL_SERCOM;

typedef enum {
    TWI_ATMEL_SERCOM_STATE_IDLE,
    TWI_ATMEL_SERCOM_STATE_ACK,
    TWI_ATMEL_SERCOM_STATE_DATA,
    TWI_ATMEL_SERCOM_STATE_STOP,
    TWI_ATMEL_SERCOM_STATE_COUNT
} twi_atmel_sercom_state_t;

struct twi {
    /*@temp@*/ struct TWI_ATMEL_SERCOM *base;
    clock_id_t clkid;
    twi_mode_t mode;
    twi_atmel_sercom_state_t state;
    twi_addr_t slave_addr;
    int last;
};

int twi_atmel_sercom_init(/*@out@*/ struct twi *ctx, struct TWI_ATMEL_SERCOM *base, clock_id_t clkid);

/* Runtime calls:
 * int twi_setup(struct twi *ctx, struct twi_settings *settings);
 *
 * int twi_write(struct twi *ctx, const void *buf, size_t n);
 * int twi_read(struct twi *ctx, void *buf, size_t n);
 */

#endif

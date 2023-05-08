#ifndef TWI_AVR_H
#define TWI_AVR_H

#include "twi.h"
#include "clock.h"

struct TWI_AVR;

typedef enum {
    TWI_AVR_STATE_IDLE,
    TWI_AVR_STATE_START,
    TWI_AVR_STATE_SLA,
    TWI_AVR_STATE_DATA,
    TWI_AVR_STATE_STOP,
    TWI_AVR_STATE_COUNT
} twi_avr_state_t;

struct twi {
    /*@temp@*/ struct TWI_AVR *base;
    clock_id_t clkid;
    twi_mode_t mode;
    twi_avr_state_t state;
    twi_addr_t slave_addr;
    int last;
};

int twi_avr_init(/*@out@*/ struct twi *ctx, int base, clock_id_t clkid);

/* Runtime calls:
 * int twi_setup(struct twi *ctx, struct twi_settings *settings);
 *
 * int twi_write(struct twi *ctx, const void *buf, size_t n);
 * int twi_read(struct twi *ctx, void *buf, size_t n);
 */

#endif

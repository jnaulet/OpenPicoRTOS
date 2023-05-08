#ifndef TWI_PIC32MX_H
#define TWI_PIC32MX_H

#include "twi.h"
#include "clock.h"

struct TWI_PIC32MX;

typedef enum {
    TWI_PIC32MX_STATE_IDLE,
    TWI_PIC32MX_STATE_START,
    TWI_PIC32MX_STATE_SLA,
    TWI_PIC32MX_STATE_DATA,
    TWI_PIC32MX_STATE_STOP,
    TWI_PIC32MX_STATE_COUNT
} twi_pic32mx_state_t;

struct twi {
    /*@temp@*/ struct TWI_PIC32MX *base;
    clock_id_t clkid;
    twi_mode_t mode;
    twi_pic32mx_state_t state;
    twi_addr_t slave_addr;
    int last;
};

int twi_pic32mx_init(/*@out@*/ struct twi *ctx, int base, clock_id_t clkid);

/* Runtime calls:
 * int twi_setup(struct twi *ctx, struct twi_settings *settings);
 *
 * int twi_write(struct twi *ctx, const void *buf, size_t n);
 * int twi_read(struct twi *ctx, void *buf, size_t n);
 */

#endif

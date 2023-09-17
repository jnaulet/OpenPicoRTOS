#ifndef TWI_RENESAS_RA4_H
#define TWI_RENESAS_RA4_H

#include "twi.h"
#include "clock.h"

struct TWI_RENESAS_RA4;

typedef enum {
  TWI_RENESAS_RA4_STATE_IDLE,
  TWI_RENESAS_RA4_STATE_SLA,
  TWI_RENESAS_RA4_STATE_WAIT,
  TWI_RENESAS_RA4_STATE_XFER,
  TWI_RENESAS_RA4_STATE_END,
  TWI_RENESAS_RA4_STATE_STOP,
  TWI_RENESAS_RA4_STATE_COUNT
} twi_renesas_ra4_state_t;

struct twi {
    /*@temp@*/ struct TWI_RENESAS_RA4 *base;
    clock_id_t clkid;
    twi_renesas_ra4_state_t state;
    int last;
    /* properties */
    twi_mode_t mode;
    twi_addr_t slave_addr;
};

int twi_renesas_ra4_init(/*@out@*/ struct twi *ctx, int base, clock_id_t clkid);

/* Implements:
 * int twi_setup(struct twi *ctx, struct twi_settings *settings);
 * int twi_poll(struct twi *ctx);
 * int twi_write(struct twi *ctx, const void *buf, size_t n);
 * int twi_read(struct twi *ctx, void *buf, size_t n);
 */

#endif

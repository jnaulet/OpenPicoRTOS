#ifndef TWI_SAM_H
#define TWI_SAM_H

#include "twi.h"
#include "clock.h"

#include <stdint.h>

struct TWI_SAM;

typedef enum {
    TWI_SAM_STATE_IDLE,
    TWI_SAM_STATE_XFER,
    TWI_SAM_STATE_COUNT
} twi_sam_state_t;

struct twi {
    /*@temp@*/ struct TWI_SAM *base;
    clock_id_t clkid;
    twi_mode_t mode;
    twi_sam_state_t state;
    uint32_t twi_cr;
};

int twi_sam_init(/*@out@*/ struct twi *ctx, struct TWI_SAM *base, clock_id_t clkid);

/* Runtime calls:
 * int twi_setup(struct twi *ctx, struct twi_settings *settings);
 *
 * int twi_write(struct twi *ctx, const void *buf, size_t n);
 * int twi_read(struct twi *ctx, void *buf, size_t n);
 */

#endif

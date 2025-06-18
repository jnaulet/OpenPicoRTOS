#ifndef TWI_N76E003_H
#define TWI_N76E003_H

#include "twi.h"
#include "clock.h"

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    TWI_N76E003_STATE_IDLE,
    TWI_N76E003_STATE_START,
    TWI_N76E003_STATE_DATA,
    TWI_N76E003_STATE_COUNT
} twi_n76e003_state_t;

struct twi {
    clock_id_t clkid;
    bool pending;
    twi_n76e003_state_t state;
    unsigned char addr;
};

int twi_n76e003_init(/*@out@*/ struct twi *ctx, clock_id_t clkid, bool i2cpx);

/*
 * Implements
 * int twi_setup(struct twi *ctx, const struct twi_settings *settings);
 * int twi_write(struct twi *ctx, const void *buf, size_t n, int flags);
 * int twi_read(struct twi *ctx, void *buf, size_t n, int flags);
 */

#endif

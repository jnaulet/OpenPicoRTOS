#ifndef TWI_STM32H7XX_H
#define TWI_STM32H7XX_H

#include "twi.h"
#include "clock.h"

struct TWI_STM32H7XX;

struct twi {
    /*@temp@*/ struct TWI_STM32H7XX *base;
    clock_id_t clkid;
};

int twi_stm32h7xx_init(/*@out@*/ struct twi *ctx, int base, clock_id_t clkid);

/* Implements:
 * int twi_setup(struct twi *ctx, struct twi_settings *settings);
 * int twi_poll(struct twi *ctx);
 * int twi_write(struct twi *ctx, const void *buf, size_t n);
 * int twi_read(struct twi *ctx, void *buf, size_t n);
 */

#endif

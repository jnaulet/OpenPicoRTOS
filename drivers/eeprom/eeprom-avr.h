#ifndef C99_EEPROM_AVR_H
#define C99_EEPROM_AVR_H

#include "eeprom.h"
#include <stdint.h>

#define C99_EEPROM_AVR_ADDR_COUNT 4096

struct C99_EEPROM_AVR;

struct eeprom {
    /*@temp@*/ struct C99_EEPROM_AVR *base;
    /*@temp@*/ volatile uint8_t *SPMCSR;
};

int eeprom_avr_init(/*@out@*/ struct eeprom *ctx, struct C99_EEPROM_AVR *base, uint8_t *SPMCSR);

/* Runtime calls:
 * int eeprom_read(struct eeprom *ctx, size_t addr, void *buf, size_t n);
 * int eeprom_erase(struct eeprom *ctx, size_t addr, size_t n);
 * int eeprom_write(struct eeprom *ctx, size_t addr, const void *buf, size_t n);
 */

#endif

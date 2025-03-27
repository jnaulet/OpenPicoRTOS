#ifndef C99_EEPROM_CY15B004J_H
#define C99_EEPROM_CY15B004J_H

#include "twi.h"
#include "eeprom.h"

#include <stdint.h>

#define C99_EEPROM_CY15B004J_ADDR_COUNT   512
#define C99_EEPROM_CY15B004J_DEVICE_COUNT 4

typedef enum {
    CY15B004J_STATE_SETUP,
    CY15B004J_STATE_ADDRESS,
    CY15B004J_STATE_DATA,
    CY15B004J_STATE_COUNT
} cy15b004j_state_t;

struct eeprom {
    /*@temp@*/ struct twi *twi;
    struct twi_settings settings;
    cy15b004j_state_t state;
};

int eeprom_cy15b004j_init(/*@out@*/ struct eeprom *ctx, struct twi *twi, int device);

/* Runtime calls:
 * int eeprom_read(struct eeprom *ctx, size_t addr, void *buf, size_t n);
 * int eeprom_erase(struct eeprom *ctx, size_t addr, size_t n);
 * int eeprom_write(struct eeprom *ctx, size_t addr, const void *buf, size_t n);
 */

#endif

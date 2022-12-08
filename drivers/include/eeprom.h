#ifndef HAL_EEPROM_H
#define HAL_EEPROM_H

#include <errno.h>
#include <stddef.h>

struct eeprom;

/* Function: eeprom_read
 * Reads value from EEPROM
 *
 * Parameters:
 *  ctx - An EEPROM instance
 *  addr - The EEPROM addr to read from
 *  buf - A buffer where the read data will be stored
 *  n - The number of bytes to read / buffer size
 *
 * Returns:
 * The number of bytes read if success, -errno otherwise
 */
int eeprom_read(struct eeprom *ctx, size_t addr, void *buf, size_t n);

/* Function: eeprom_erase
 * Erase data from EEPROM
 *
 * Parameters:
 *  ctx - An EEPROM instance
 *  addr - The EEPROM address to start erasing from
 *  n - The umber of bytes to erase
 *
 * Returns:
 * The number of bytes erased if success, -errno otherwise
 */
int eeprom_erase(struct eeprom *ctx, size_t addr, size_t n);

/* Function: eeprom_write
 * Write values to EEPROM
 *
 * Parameters:
 *  ctx - An EEPROM instance
 *  addr - The EEPROM address to write to
 *  buf - A buffer containing the data to write
 *  n - The size in bytes of the data to write
 *
 * Returns:
 * The number of bytes written if success, -errno otherwise
 */
int eeprom_write(struct eeprom *ctx, size_t addr, const void *buf, size_t n);

#endif

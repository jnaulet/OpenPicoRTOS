#ifndef HAL_EEPROM_H
#define HAL_EEPROM_H

#include <errno.h>
#include <stddef.h>

struct eeprom;

/**
 * int **eeprom_read**(**struct eeprom** \*<ins>ctx</ins>, 
 * **size_t** <ins>addr</ins>, **void** \*<ins>buf</ins>, 
 * **size_t** <ins>n</ins>);
 * > Reads value from EEPROM
 *
 * ### RETURN
 * > Returns the number of bytes read if success, -errno otherwise
 */
int eeprom_read(struct eeprom *ctx, size_t addr, void *buf, size_t n);

/**
 * int **eeprom_erase**(**struct eeprom** \*<ins>ctx</ins>, 
 * **size_t** <ins>addr</ins>, **size_t** <ins>n</ins>);
 * > Erase data from EEPROM
 *
 * ### RETURN
 * > Returns the number of bytes erased if success, -errno otherwise
 */
int eeprom_erase(struct eeprom *ctx, size_t addr, size_t n);

/** 
 * int **eeprom_write**(**struct eeprom** \*<ins>ctx</ins>, 
 * **size_t** <ins>addr</ins>, const **void** \*<ins>buf</ins>,
 * **size_t** <ins>n</ins>);
 * > Write values to EEPROM
 *
 * ### RETURN
 * > Returns the number of bytes written if success, -errno otherwise
 */
int eeprom_write(struct eeprom *ctx, size_t addr, const void *buf, size_t n);

#endif

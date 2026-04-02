#ifndef FLASH_ATTRIBUTES_H
#define FLASH_ATTRIBUTES_H

#include <stddef.h>

struct flash_attributes {
    size_t erase_unit_len;      /* min erase unit length */
    size_t erase_unit_count;    /* the flash size in min erase units */
    size_t write_unit_len;      /* min data length for a write operation */
    size_t lock_unit_len;       /* min data length for lock protection */
    size_t total_size;          /* total flash size, in bytes */
};

#endif

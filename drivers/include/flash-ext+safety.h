#ifndef FLASH_EXT_SAFETY_H
#define FLASH_EXT_SAFETY_H

#include "flash-ext.h"

struct flash_ext *flash_ext_claim(/*@returned@*/ struct flash_ext *ctx);

#endif

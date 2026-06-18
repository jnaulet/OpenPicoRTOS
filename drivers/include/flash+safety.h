#ifndef FLASH_SAFETY_H
#define FLASH_SAFETY_H

#include "flash.h"

struct flash *flash_claim(/*@returned@*/ struct flash*ctx);

#endif

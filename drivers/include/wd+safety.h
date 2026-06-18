#ifndef HAL_WD_SAFETY_H
#define HAL_WD_SAFETY_H

#include "wd.h"

struct wd *wd_claim(/*@returned@*/ struct wd *ctx);

#endif

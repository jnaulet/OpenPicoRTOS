#ifndef HAL_WD_SAFETY_H
#define HAL_WD_SAFETY_H

#include "wd.h"

/**
 * struct wd \***wd_claim**(**struct wd** \*<ins>ctx</ins>);
 * > Claims the wd
 * ### NOTES
 * > This feature is related to MPU & will only have an effect if
 * > `CONFIG_MPU` is active.
 */
struct wd *wd_claim(/*@returned@*/ struct wd *ctx);

#endif

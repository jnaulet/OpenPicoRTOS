#ifndef HAL_WD_H
#define HAL_WD_H

#include <errno.h>

struct wd;

/**
 * int **wd_start**(**struct wd** \*<ins>ctx</ins>);
 * > Starts the watchdog
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int wd_start(struct wd *ctx);

/**
 * int **wd_stop**(**struct wd** \*<ins>ctx</ins>);
 * > Stops the watchdog
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int wd_stop(struct wd *ctx);

/**
 * void **wd_refresh**(**struct wd** \*<ins>ctx</ins>);
 * > Refreshes the watchdog
 */
void wd_refresh(struct wd *ctx);

#endif

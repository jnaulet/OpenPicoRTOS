#ifndef HAL_WD_H
#define HAL_WD_H

#include <errno.h>

struct wd;

/* Function: wd_start
 * Starts the watchdog
 *
 * Parameters:
 *  ctx - A watchdog instance
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int wd_start(struct wd *ctx);

/* Function: wd_stop
 * Stops the watchdog
 *
 * Parameters:
 *  ctx - A watchdog instance
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int wd_stop(struct wd *ctx);

/* Function: wd_refresh
 * Refreshes the watchdog
 *
 * Parameters:
 *  ctx - A watchdog instance
 */
void wd_refresh(struct wd *ctx);

#endif

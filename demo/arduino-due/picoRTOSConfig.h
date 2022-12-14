#ifndef PICORTOSCONFIG_H
#define PICORTOSCONFIG_H

/* CLOCKS */
#define CONFIG_SYSCLK_HZ        84000000
#define CONFIG_TICK_HZ          1000

/* TASKS */
#define CONFIG_TASK_COUNT       12
/* fixed priorities */
#define TASK_TICK_PRIO          0

/* STACK */
#define CONFIG_DEFAULT_STACK_COUNT 128

/* IPC */
#define CONFIG_DEADLOCK_COUNT 1000

#endif

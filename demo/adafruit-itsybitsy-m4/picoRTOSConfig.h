#ifndef PICORTOSCONFIG_H
#define PICORTOSCONFIG_H

/* CLOCKS */
#define CONFIG_SYSCLK_HZ        120000000
#define CONFIG_TICK_HZ          1000

/* TASKS */
#define CONFIG_TASK_COUNT      5

/* STACK */
#define CONFIG_DEFAULT_STACK_COUNT 256

/* DRIVERS */
#define CONFIG_DEADLOCK_COUNT     1000

#endif
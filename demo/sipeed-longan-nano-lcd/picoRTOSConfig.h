#ifndef PICORTOSCONFIG_H
#define PICORTOSCONFIG_H

/* CLOCKS */
#define CONFIG_SYSCLK_HZ        (108000000 / 4) /* According to clock tree */
#define CONFIG_TICK_HZ          1000

/* TASKS */
#define CONFIG_TASK_COUNT 2

/* STACK */
#define CONFIG_DEFAULT_STACK_COUNT 256

/* DEADLOCK */
#define CONFIG_DEADLOCK_COUNT     CONFIG_TICK_HZ

#endif

#ifndef PICORTOSCONFIG_H
#define PICORTOSCONFIG_H

/* CLOCKS */
#define CONFIG_SYSCLK_HZ        125000000
#define CONFIG_TICK_HZ          1000

/* TASKS */
#define CONFIG_TASK_COUNT      2
#define TASK_TICK_PRIO         0
#define TASK_LED_PRIO          1

/* STACK */
#define CONFIG_DEFAULT_STACK_COUNT 256

/* DEBUG */
#define CONFIG_CHECK_STACK_INTEGRITY

#endif

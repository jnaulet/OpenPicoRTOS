#ifndef PICORTOSCONFIG_H
#define PICORTOSCONFIG_H

/* SMP */
#define CONFIG_SMP_CORES 2

/* CLOCKS */
#define CONFIG_SYSCLK_HZ        192000000
#define CONFIG_TICK_HZ          1000

/* TASKS */
#define CONFIG_TASK_COUNT       5
#define TASK_TICK_PRIO          0
#define TASK_LED0_PRIO          1
#define TASK_LED1_PRIO          2
#define TASK_ADC_PRIO           3
#define TASK_PWM_PRIO           4

/* STACK */
#define CONFIG_DEFAULT_STACK_COUNT 256

/* MUTEX */
#define CONFIG_DEADLOCK_COUNT 1000

#endif

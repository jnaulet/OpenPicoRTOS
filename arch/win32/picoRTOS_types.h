#ifndef PICORTOS_TYPES_H
#define PICORTOS_TYPES_H

#include <assert.h>

typedef unsigned long picoRTOS_stack_t;
typedef unsigned long picoRTOS_priority_t;
typedef unsigned long picoRTOS_pid_t;
typedef unsigned long picoRTOS_tick_t;
typedef unsigned long picoRTOS_atomic_t;
typedef unsigned long picoRTOS_irq_t;
typedef unsigned long picoRTOS_cycles_t;

#define ARCH_INITIAL_STACK_COUNT 10
#define ARCH_MIN_STACK_COUNT ARCH_INITIAL_STACK_COUNT

#define arch_break() __debugbreak()

#endif

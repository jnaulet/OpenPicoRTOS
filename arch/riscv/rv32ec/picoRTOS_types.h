#ifndef PICORTOS_TYPES_H
#define PICORTOS_TYPES_H

#include <generated/autoconf.h>

typedef unsigned long picoRTOS_stack_t;
typedef unsigned long picoRTOS_tick_t;
typedef unsigned long picoRTOS_priority_t;
typedef unsigned long picoRTOS_pid_t;
typedef unsigned long picoRTOS_atomic_t;
typedef unsigned long picoRTOS_irq_t;
typedef unsigned long picoRTOS_cycles_t;

typedef long picoRTOS_intptr_t;

#define ARCH_INITIAL_STACK_COUNT 14
#define ARCH_MIN_STACK_COUNT     (ARCH_INITIAL_STACK_COUNT + 8)

/* no cache */
#define ARCH_L1_DCACHE_LINESIZE sizeof(unsigned long)

/* splint doesn't like inline assembly */
#ifdef S_SPLINT_S
# define ASM(x) {}
#else
# define ASM(x) { __asm__ volatile (x); }
# define arch_break() ASM("ebreak")
#endif

#endif

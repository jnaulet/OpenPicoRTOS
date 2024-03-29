#ifndef PICORTOS_TYPES_H
#define PICORTOS_TYPES_H

#include "picoRTOS_device.h"
#include <generated/autoconf.h>

typedef unsigned long picoRTOS_stack_t;
typedef unsigned long picoRTOS_tick_t;
typedef unsigned long picoRTOS_priority_t;
typedef unsigned long picoRTOS_pid_t;
typedef unsigned long picoRTOS_atomic_t;
typedef unsigned long picoRTOS_irq_t;
typedef unsigned long picoRTOS_cycles_t;

typedef long picoRTOS_intptr_t;

#define ARCH_L1_DCACHE_LINESIZE    16
#define ARCH_L1_DCACHE_STACK_COUNT (ARCH_L1_DCACHE_LINESIZE / 4)

#define ARCH_INITIAL_CPU_STACK_COUNT 29
#define ARCH_INITIAL_FPU_STACK_COUNT 0
#define ARCH_INITIAL_DSP_STACK_COUNT 0

#ifdef CONFIG_M51XX_FPU64
# undef ARCH_INITIAL_FPU_STACK_COUNT
# define ARCH_INITIAL_FPU_STACK_COUNT 66
#endif
#ifdef CONFIG_M51XX_DSP
# undef ARCH_INITIAL_DSP_STACK_COUNT
# define ARCH_INITIAL_DSP_STACK_COUNT 6
#endif

#define ARCH_INITIAL_STACK_COUNT (ARCH_INITIAL_CPU_STACK_COUNT +  \
                                  ARCH_INITIAL_FPU_STACK_COUNT +  \
                                  ARCH_INITIAL_DSP_STACK_COUNT)

#define ARCH_MIN_STACK_COUNT     (ARCH_INITIAL_STACK_COUNT +    \
                                  ARCH_L1_DCACHE_STACK_COUNT +  \
                                  4)

/* splint doesn't like inline assembly */
#ifdef S_SPLINT_S
# define ASM(x) {}
#else
# define ASM(x) { __asm__ volatile (x); }
# define arch_break() ASM("sdbbp")
#endif

#endif

#ifndef PICORTOS_TYPES_H
#define PICORTOS_TYPES_H

typedef unsigned long picoRTOS_stack_t;
typedef unsigned long picoRTOS_tick_t;
typedef unsigned long picoRTOS_priority_t;
typedef unsigned long picoRTOS_atomic_t;
typedef unsigned long picoRTOS_irq_t;
typedef unsigned long picoRTOS_cycles_t;

#define ARCH_INITIAL_STACK_COUNT 19
#define ARCH_MIN_STACK_COUNT (ARCH_INITIAL_STACK_COUNT + 8)

/* splint doesn't like inline assembly */
#ifdef S_SPLINT_S
# define ASM(x) {}
#else
# define ASM(x) __asm__ volatile (x)
# define arch_break() ({ ASM("break.n 1"); })
#endif

#endif

#ifndef PICORTOS_TYPES_H
#define PICORTOS_TYPES_H

typedef unsigned int picoRTOS_stack_t;
typedef unsigned long picoRTOS_tick_t;
typedef unsigned int picoRTOS_priority_t;
typedef unsigned int picoRTOS_pid_t;
typedef unsigned int picoRTOS_atomic_t;
typedef unsigned int picoRTOS_irq_t;
typedef unsigned long picoRTOS_cycles_t;

#if (__TMS320C28XX_FPU32__ == 1) || \
    (__TMS320C28XX_FPU64__ == 1)
# define ARCH_INTIAL_STACK_COUNT 72
#else
# define ARCH_INTIAL_STACK_COUNT 36
#endif

/* +4 to account for -O0 */
#define ARCH_MIN_STACK_COUNT (ARCH_INTIAL_STACK_COUNT + 4)

/* no cache */
#define ARCH_L1_DCACHE_LINESIZE sizeof(int)

/* splint cannot check inline assembly */
#ifdef S_SPLINT_S
# define ASM(x) {}
#else
# define ASM(x) { asm (x); }
# define arch_break() ASM("  estop0")
#endif

#endif

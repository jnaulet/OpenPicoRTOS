#ifndef PICORTOS_TYPES_H
#define PICORTOS_TYPES_H

/* optimize for speed */
typedef unsigned char picoRTOS_stack_t;
typedef unsigned int picoRTOS_tick_t;
typedef unsigned int picoRTOS_priority_t;
typedef unsigned char picoRTOS_atomic_t;
typedef unsigned int picoRTOS_irq_t;
typedef unsigned int picoRTOS_cycles_t;

#define ARCH_INTIAL_STACK_COUNT 35
#define ARCH_MIN_STACK_COUNT (ARCH_INTIAL_STACK_COUNT + 6)

/* splint cannot check inline assembly */
#ifdef S_SPLINT_S
# define ASM(x) {}
#else
# define ASM(x) __asm__ volatile (x)
# define arch_break() ({ ASM("break"); })
#endif

/* Unfortunately avr-gcc defines all errnos to ENOERR,
 * so we have to redefine our own here */
#include <errno.h>

#undef EIO
#undef ENXIO
#undef EAGAIN
#undef ENOMEM
#undef EFAULT
#undef EBUSY
#undef EINVAL
#undef EPIPE

#define EIO              5      /* I/O error */
#define ENXIO            6      /* No such device or address */
#define EAGAIN          11      /* No more processes */
#define ENOMEM          12      /* Out of memory */
#define EFAULT          14      /* Bad address */
#define EBUSY           16      /* Device or resource busy */
#define EINVAL          22      /* Invalid argument */
#define EPIPE           32      /* Broken pipe */

#endif

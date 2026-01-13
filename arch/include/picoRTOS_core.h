#ifndef PICORTOS_CORE_H
#define PICORTOS_CORE_H

#ifdef S_SPLINT_S
/*@external@*/ void picoRTOS_core_sef(/*@sef@*/ bool pred);
/*@external@*/ void ASM(/*@observer@*/ const char *s);
#else
# define picoRTOS_core_sef(x)
/* define ASM for each known compiler */
# if defined(__GNUC__)
#  define ASM(x) { __asm__ volatile (x); }
# elif defined(__SDCC)
#  define ASM(x) { __asm__ (x); }
/* SDCC hack for attributes */
#  define __attribute__(x)
# else
#  define ASM(x) { __asm(x); }
# endif
#endif

#endif

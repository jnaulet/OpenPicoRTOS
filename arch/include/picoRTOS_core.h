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

/* Macro: PRIVILEGED_DATA
 * Put the following data in the privileged/kernel section
 */
#define PRIVILEGED_DATA   __attribute__((section(".pdata")))
/* Macro: UNPRIVILEGED_DATA
 * Put the following daa in the unprivileged/shared section
 * Typical use:
 * (start code)
 * static struct picoRTOS_mutex UNPRIVILEGED_DATA mutex = PICORTOS_MUTEX_INITIALIZER;
 * static struct picoRTOS_cond UNPRIVILEGED_DATA cond = PICORTOS_COND_INITIALIZER;
 * (end)
 */
#define UNPRIVILEGED_DATA  __attribute__((section(".udata")))
#define PRIVILEGED_STACK   __attribute__((section(".pstack")))
#define UNPRIVILEGED_STACK __attribute__((section(".ustack")))
#define PRIVILEGED_BSS     __attribute__((section(".pbss")))
#define UNPRIVILEGED_BSS   __attribute__((section(".ubss")))
#define PRIVILEGED_TEXT    __attribute__((section(".ptext")))
#define UNPRIVILEGED_TEXT  __attribute__((section(".utext")))

#endif

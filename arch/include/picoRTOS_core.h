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

/**
 * **PRIVILEGED_TEXT**
 * > Puts the following code in the kernel privileged section `.ptext`
 * ### NOTES
 * > If `CONFIG_MPU`, this section is only accessible through privilege
 * > escalation & no task can access & fetch instructions (SEGFAULT)<br>
 * > This is where the kernel code sits & you **SHOULDN'T** put
 * > anything here
 */
#define PRIVILEGED_TEXT    __attribute__((section(".ptext")))

/**
 * **UNPRIVILEGED_TEXT**
 * > Puts the following code in the unprivileged section `.utext`
 * ### NOTES
 * > If `CONFIG_MPU`, this section is accessible to everyoneto read or fetch
 * > instructions.<br>
 * > If you want better code protection, you **SHOULD** use custom sections
 * > & add them through `picoRTOS_mpu_add_region()`
 */
#define UNPRIVILEGED_TEXT  __attribute__((section(".utext")))

/**
 * **PRIVILEGED_DATA**
 * > Puts the following data in the kernel privileged section `.pdata`
 * ### NOTES
 * > If `CONFIG_MPU`, this section is only accessible through privilege
 * > escalation & no task can access it (SEGFAULT)
 */
#define PRIVILEGED_DATA   __attribute__((section(".pdata")))

/**
 * **UNPRIVILEGED_DATA**
 * > Put the following data in the unprivileged section `.udata`
 * > ### NOTES
 * > Typical use:
 * ```c
 *     static struct picoRTOS_mutex UNPRIVILEGED_DATA mutex = PICORTOS_MUTEX_INITIALIZER;
 *     static struct picoRTOS_cond UNPRIVILEGED_DATA cond = PICORTOS_COND_INITIALIZER;
 * ```
 * > If `CONFIG_MPU`, this section is accessible to any task, regardless of
 * > privileges. This **SHOULD** be of very limited use (IPCs, typically)
 */
#define UNPRIVILEGED_DATA  __attribute__((section(".udata")))

/**
 * **PRIVILEGED_STACK**
 * > Put the following data in the privileged data section `.pstack`
 * ### NOTES
 * > You **SHOULDN'T** use this section, it is reserved for internal kernel
 * > use (main stacks for single-core & SMP).
 */
#define PRIVILEGED_STACK   __attribute__((section(".pstack")))

/**
 * **UNPRIVILEGED_STACK**
 * > Put the following data in the unprivileged data section `.ustack`
 * ### NOTES
 * > You **SHOULDN'T** use this section, it is reserved for internal kernel
 * > use (idle stacks)
 */
#define UNPRIVILEGED_STACK __attribute__((section(".ustack")))

/**
 * **PRIVILEGED_BSS**
 * > Put the following data in the privileged data section `.pbss`
 * ### NOTES
 * > You **SHOULDN'T** use this section, it is reserved for internal kernel
 * > use (main stacks for single-core & SMP).
 */
#define PRIVILEGED_BSS     __attribute__((section(".pbss")))

/**
 * **UNPRIVILEGED_BSS**
 * > Put the following data in the unprivileged bss section `.bss`
 * ### NOTES
 * > This section is ignored by picoRTOS & any access without prior
 * > use of `picoRTOS_mpu_add_region()` will lead to a SEGFAULT.
 * >
 * > A notable exception are the tasks' stacks, who sit there but
 * > are made R/W accessible to their owners automatically after
 * > `picoRTOS_start()`
 */
#define UNPRIVILEGED_BSS   __attribute__((section(".bss")))

#endif

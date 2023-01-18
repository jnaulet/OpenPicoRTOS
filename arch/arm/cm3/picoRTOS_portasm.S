	.syntax unified

	/* Declarations */
	.global arch_start_first_task
	.global arch_syscall
	.global arch_SYSTICK
	.global arch_PENDSV

	.global arch_compare_and_swap

	.extern picoRTOS_tick
	.extern picoRTOS_syscall

	#include "picoRTOS_device.h"

	.equ NVIC_ICSR, 0xe000ed04

	.macro SAVE_CONTEXT rS
	clrex				/* clear exclusive bit */
	mrs \rS, psp			/* use r0 as psp stack */
	stmdb \rS!, {r4-r11}		/* save registers r4-r11 */
	.endm

	.macro RESTORE_CONTEXT rS
	ldmia \rS!, {r4-r11}	/* restore registers r4-11 */
	msr psp, \rS		/* reclaim stack space */
	.endm

	.align 1
	.thumb_func
	.type arch_start_first_task, %function
arch_start_first_task:
	RESTORE_CONTEXT r0
	/* switch to psp stack */
	movs r0, #2
	msr CONTROL, r0
	isb

	pop {r0-r3, r12, lr}	/* pop the registers that are saved automatically */
	pop {r2-r3}		/* return addr and xspr */
	cpsie i		/* enable interrupts */
	bx r2

	.align 1
	.thumb_func
	.type arch_syscall, %function
arch_syscall:
	push {lr}
	/* call arch_PENDSV directly (values will be auto-stacked) */
	mov r2, #0x10000000
	ldr r3, =NVIC_ICSR
	str r2, [r3]
	pop {pc}

	.align 1
	.thumb_func
	.type arch_SYSTICK, %function
arch_SYSTICK:
	push {lr}		/* store lr for interrupt return */
	SAVE_CONTEXT r0
	bl picoRTOS_tick
	RESTORE_CONTEXT r0
	pop {pc}	/* return */

	.align 1
	.thumb_func
	.type arch_PENDSV, %function
arch_PENDSV:
	push {lr}		/* store lr (interrupt return) */
	SAVE_CONTEXT r0
	ldr r1, [r0, #32]	/* get syscall from psp (auto-stacked r0) */
	ldr r2, [r0, #36]	/* get priv from psp (auto-stacked r1) */
	bl picoRTOS_syscall
	RESTORE_CONTEXT r0
	pop {pc}		/* return */

	.align 1
	.thumb_func
	.type arch_compare_and_swap, %function
arch_compare_and_swap:
	dmb ish
	ldrex r3, [r0]
	cmp r3, r1
	bne .arch_compare_and_swap_fail_cmp
	strex r3, r2, [r0]
	cmp r3, #0
	bne .arch_compare_and_swap_fail_strex
	mov r0, r1	/* success */
	dmb ish
	bx lr
.arch_compare_and_swap_fail_cmp:
	clrex
.arch_compare_and_swap_fail_strex:
	mov r0, r2
	dmb ish
	bx lr

	/* INTERRUPT HANDLING */
	.global arch_NVIC_handler
	.global ISR_TABLE

	.align 1
	.thumb_func
	.type arch_NVIC_handler, %function
arch_NVIC_handler:
	push {lr}
	SAVE_CONTEXT r0
	push {r0}
	/* get active exception in r0 */
	ldr r2, =NVIC_ICSR
	ldr r0, [r2]
	ubfx r0, r0, #0, #9
	subs r0, #16
	ldr r3, =ISR_TABLE
	lsls r0, r0, #3	/* x8 to get the right entry */
	adds r3, r3, r0
	ldr r1, [r3]
	ldr r0, [r3, #4]
	blx r1
	pop {r0}
	RESTORE_CONTEXT r0
	pop {pc}

	.bss
	.align 2
	.type ISR_TABLE, %object
ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.long 0
	.endr

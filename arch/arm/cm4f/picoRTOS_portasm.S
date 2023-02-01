	.syntax unified

	/* Declarations */
	.global arch_enable_vfp
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
	clrex			/* clear exclusive bit */
	mrs \rS, psp		/* use rS as psp stack */
	stmdb \rS!, {r4-r11}	/* save registers r4-r11 */
	vstmdb \rS!, {s16-s31}	/* save fpu high registers */
	.endm

	.macro RESTORE_CONTEXT rS
	vldmia \rS!, {s16-s31}	/* restore fpu high registers */
	ldmia \rS!, {r4-r11}	/* restore registers r4-11 */
	msr psp, \rS		/* reclaim stack space */
	.endm

	.equ CPACR, 0xe000ed88
	.equ FPCCR, 0xe000ef34

	.align 1
	.thumb_func
	.type arch_enable_vfp, %function
arch_enable_vfp:
	/* enable CP10 & CP11 */
	ldr.w r0, =CPACR
	ldr r1, [r0]
	orr r1, r1, #0xf00000
	str r1, [r0]
	/* extended FPU frame stacking (no lazy frame) */
	ldr.w r0, =FPCCR
	movs r1, #0x80000000
	str r1, [r0]
	bx lr

	.align 1
	.thumb_func
	.type arch_start_first_task, %function
arch_start_first_task:
	RESTORE_CONTEXT r0
	/* switch to psp stack & activate FP */
	movs r0, #6
	msr CONTROL, r0
	isb

	pop {r0-r3, r12, lr}	/* pop the registers that are saved automatically */
	pop {r2, r3}		/* return address and xspr */

	/* pop fpu */
	mrs r4, psp
	add r4, r4, #72
	msr psp, r4

	cpsie if		/* enable interrupts */
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
	ldr r1, [r0, #96]	/* get syscall from psp (auto-stacked r0) */
	ldr r2, [r0, #100]	/* get priv from psp (auto-stacked r1) */
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

	.syntax unified
	.arch armv7-m

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

	.thumb_func
	.type arch_save_first_context, %function
	.global arch_save_first_context
arch_save_first_context:
	push {r4-r11, lr}
	/* auto-save */
	mov r4, r2		/* r0 (priv) */
	movs r5, #0		/* r1 */
	movs r6, #0		/* r2 */
	movs r7, #0		/* r3 */
	mov r8, r12		/* r12 */
	mov r9, lr		/* lr (r14) */
	mov r10, r1		/* return address (fn) */
	ldr r11, =0x1000000	/* xspr */
	stmdb r0!, {r4-r11}
	/* user context */
	msr psp, r0
	SAVE_CONTEXT r0
	pop {r4-r11, pc}

	.equ SYSTICK_CSR, 0xe000e010

	.thumb_func
	.type arch_start_first_task, %function
	.global arch_start_first_task
arch_start_first_task:
	RESTORE_CONTEXT r0
	/* switch to psp stack */
	movs r0, #2
	msr CONTROL, r0
	isb

	pop {r0-r3, r12, lr}	/* pop auto-save */
	pop {r4-r5}		/* return addr and xspr */

	/* start systick */
	movs r1, #1
	ldr r2, =SYSTICK_CSR
	ldr r3, [r2]
	orrs r1, r1, r3
	str r1, [r2]

	cpsie i		/* enable interrupts */
	bx r4

	.thumb_func
	.type arch_syscall, %function
	.global arch_syscall
arch_syscall:
	push {lr}
	svc #0
	pop {pc}

	.thumb_func
	.type SysTick_Handler, %function
	.global SysTick_Handler
SysTick_Handler:
	push {lr}	/* store lr for interrupt return */
	SAVE_CONTEXT r0
	bl picoRTOS_tick
	RESTORE_CONTEXT r0
	pop {pc}	/* return */

	.thumb_func
	.type SVC_Handler, %function
	.global SVC_Handler
SVC_Handler:
	push {lr}	/* store lr (interrupt return) */
	SAVE_CONTEXT r0
	ldr r1, [r0, #32]	/* get syscall from psp (auto-stacked r0) */
	ldr r2, [r0, #36]	/* get priv from psp (auto-stacked r1) */
	bl picoRTOS_syscall
	RESTORE_CONTEXT r0
	pop {pc}		/* return */

	.thumb_func
	.type arch_compare_and_swap, %function
	.global arch_compare_and_swap
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

	.thumb_func
	.type Default_Handler, %function
	.global Default_Handler
Default_Handler:
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
	add r3, r3, r0
	ldr r1, [r3]
	ldr r0, [r3, #4]
	/* check if handled */
	cmp r1, #0
	beq isr_fn_is_null
	/* call fn(priv) */
	blx r1
	pop {r0}
	RESTORE_CONTEXT r0
	pop {pc}

isr_fn_is_null:
	/* isr call is null, use picoRTOS_register_interrupt() */
	bkpt #0
	b isr_fn_is_null

	.bss
	.align 2
	.type ISR_TABLE, %object
	.global ISR_TABLE
ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.long 0
	.endr

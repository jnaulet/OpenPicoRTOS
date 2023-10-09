	.syntax unified
	.arch armv7e-m

	.extern picoRTOS_tick
	.extern picoRTOS_syscall

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

	.text

	.thumb_func
	.type arch_save_first_context, %function
	.global arch_save_first_context
arch_save_first_context:
	push {r4-r11, lr}
	/* auto-save */
	subs r0, r0, #8	/* blank + fpscr */
	vstmdb r0!, {s0-s15}	/* fpu low registers */
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
	/* switch to psp stack & activate FP */
	movs r0, #6
	msr CONTROL, r0
	isb

	pop {r0-r3, r12, lr}	/* pop auto-save */
	pop {r4-r5}		/* return addr and xspr */

	mrs r5, psp
	vldmia r5!, {s0-s15}	/* pop auto-save */
	add r5, r5, #8		/* fpscr + blank */
	msr psp, r5

	/* start systick */
	movs r1, #1
	ldr r2, =SYSTICK_CSR
	ldr r3, [r2]
	orrs r1, r1, r3
	str r1, [r2]

	cpsie if		/* enable interrupts */
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
	ldr r1, [r0, #96]	/* get syscall from psp (auto-stacked r0) */
	ldr r2, [r0, #100]	/* get priv from psp (auto-stacked r1) */
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

	#include "picoRTOS_device.h"

	.align 2
	.type ISR_TABLE, %object
	.global ISR_TABLE
ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.long 0
	.endr

	.section .vectors, "ax"

	.word __StackTop
	.word Reset_Handler
	.word NMI_Handler
	.word HardFault_Handler
	.word MemManage_Handler
	.word BusFault_Handler
	.word UsageFault_Handler
	.word 0 /* reserved */
	.word 0 /* reserved */
	.word 0 /* reserved */
	.word 0 /* reserved */
	.word SVC_Handler
	.word DebugMon_Handler
	.word 0 /* reserved */
	.word PendSV_Handler
	.word SysTick_Handler
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.word Default_Handler
	.endr

	#include "../include/macros.S"

	/* Weak handlers */
	def_irq_handler NMI_Handler
	def_irq_handler HardFault_Handler
	def_irq_handler MemManage_Handler
	def_irq_handler BusFault_Handler
	def_irq_handler UsageFault_Handler
	def_irq_handler DebugMon_Handler
	def_irq_handler PendSV_Handler

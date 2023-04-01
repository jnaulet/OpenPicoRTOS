	.syntax unified
	.cpu cortex-m0plus

	.extern picoRTOS_tick
	.extern picoRTOS_syscall

	#include "picoRTOS_device.h"

	.equ NVIC_ICSR, 0xe000ed04

	.macro SAVE_CONTEXT rS
	mrs \rS, psp
	subs \rS, \rS, #16	/* store low registers */
	stmia \rS!, {r4-r7}
	mov r4, r8
	mov r5, r9
	mov r6, r10
	mov r7, r11
	subs \rS, \rS, #32	/* store high registers */
	stmia \rS!, {r4-r7}
	subs \rS, \rS, #16	/* revert auto-increment */
	.endm

	.macro RESTORE_CONTEXT rS
	ldmia \rS!, {r4-r7}	/* load high registers */
	mov r8, r4
	mov r9, r5
	mov r10, r6
	mov r11, r7
	ldmia \rS!, {r4-r7}	/* load low registers */
	msr psp, \rS
	.endm

	.thumb_func
	.type arch_save_first_context, %function
	.global arch_save_first_context
arch_save_first_context:
	push {r4-r7, lr}
	/* auto-save */
	subs r0, r0, #32
	mov r4, r2		/* r0 (priv) */
	movs r5, #0		/* r1 */
	movs r6, #0		/* r2 */
	movs r7, #0		/* r3 */
	stmia r0!, {r4-r7}
	mov r4, r12		/* r12 */
	mov r5, lr		/* lr (r14) */
	mov r6, r1		/* return address (fn) */
	ldr r7, =0x1000000	/* xspr */
	stmia r0!, {r4-r7}
	/* user context */
	subs r0, r0, #32
	msr psp, r0
	SAVE_CONTEXT r0
	pop {r4-r7, pc}

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

	/* pop auto-save by hand */
	pop {r0-r7}
	mov r12, r4
	mov lr, r5

	/* start systick */
	movs r1, #1
	ldr r2, =SYSTICK_CSR
	ldr r3, [r2]
	orrs r1, r1, r3
	str r1, [r2]

	cpsie i		/* enable interrupts */
	bx r6

	.thumb_func
	.type arch_syscall, %function
	.global arch_syscall
arch_syscall:
	push {lr}
	/* call arch_PENDSV directly (values will be auto-stacked) */
	movs r2, #0x80
	lsls r2, r2, #21
	ldr r3, =NVIC_ICSR
	str r2, [r3]
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
	.type PendSV_Handler, %function
	.global PendSV_Handler
PendSV_Handler:
	push {lr}	/* store lr (interrupt return) */
	SAVE_CONTEXT r0
	ldr r1, [r0, #32]	/* get syscall from psp (auto-stacked r0) */
	ldr r2, [r0, #36]	/* get priv from psp (auto-stacked r1) */
	bl picoRTOS_syscall
	RESTORE_CONTEXT r0
	pop {pc}	/* return */

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
	lsls r0, r0, #23
	lsrs r0, r0, #23
	subs r0, #16
	ldr r3, =ISR_TABLE
	lsls r0, r0, #3	/* x8 to get the right isr table index */
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

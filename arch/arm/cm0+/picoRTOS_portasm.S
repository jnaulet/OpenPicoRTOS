	.syntax unified

	/* Declarations */
	.global arch_start_first_task
	.global arch_syscall
	.global arch_SYSTICK
	.global arch_PENDSV

	.extern picoRTOS_tick
	.extern picoRTOS_switch_context

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

	.align 1
	.thumb_func
	.type arch_start_first_task, %function
arch_start_first_task:
	RESTORE_CONTEXT r0
	/* switch to psp stack */
	movs r0, #2
	msr CONTROL, r0
	isb

	pop {r0-r5}	/* pop the registers that are saved automatically */
	mov lr, r5	/* lr is copied from r5 */
	pop {r2, r3}	/* return address + xspr */
	cpsie i	/* enable interrupts */
	bx r2		/* finally, jump to the user defined task code */

	.align 1
	.thumb_func
	.type arch_syscall, %function
arch_syscall:
	push {lr}
	/* call arch_PENDSV directly (values will be auto-stacked) */
	movs r2, #0x80
	lsls r2, r2, #21
	ldr r3, =NVIC_ICSR
	str r2, [r3]
	pop {pc}

	.align 1
	.thumb_func
	.type arch_SYSTICK, %function
arch_SYSTICK:
	push {lr}	/* store lr for interrupt return */
	SAVE_CONTEXT r0
	bl picoRTOS_tick
	RESTORE_CONTEXT r0
	pop {pc}	/* return */

	.align 1
	.thumb_func
	.type arch_PENDSV, %function
arch_PENDSV:
	push {lr}	/* store lr (interrupt return) */
	SAVE_CONTEXT r0
	ldr r1, [r0, #32]	/* get syscall from psp (auto-stacked r0) */
	ldr r2, [r0, #36]	/* get priv from psp (auto-stacked r1) */
	bl picoRTOS_syscall
	RESTORE_CONTEXT r0
	pop {pc}	/* return */

	/* INTERRUPTS */
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
	lsls r0, r0, #23
	lsrs r0, r0, #23
	subs r0, #16
	ldr r3, =ISR_TABLE
	lsls r0, r0, #3	/* x8 to get the right isr table index */
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

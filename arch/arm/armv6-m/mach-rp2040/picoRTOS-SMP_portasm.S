	.syntax unified
	.cpu cortex-m0plus

	.extern picoRTOS_tick

	#include "picoRTOS_device.h"
	#include <generated/autoconf.h>

	#define PWM_CHn  7
	#define PWM_CSR  (ADDR_PWM + (PWM_CHn * 0x14))
	#define PWM_INTR (ADDR_PWM + 0xa4)

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

	.text

	.thumb_func
	.type arch_start_first_task, %function
	.global arch_start_first_task
arch_start_first_task:
	RESTORE_CONTEXT r0
	/* switch to psp stack */
	movs r0, #2
	msr CONTROL, r0
	isb

	/* start pwm */
	movs r0, #1
	ldr r3, =PWM_CSR
	str r0, [r3]

	/* pop auto-save by hand */
	pop {r0-r7}
	mov r12, r4
	mov lr, r5

	cpsie i		/* enable interrupts */
	bx r6

	.thumb_func
	.type PwmWrap_Handler, %function
	.global PwmWrap_Handler
PwmWrap_Handler:
	push {lr}	/* store lr for interrupt return */
	SAVE_CONTEXT r0

	/* clear interrupt */
	ldr r2, =PWM_INTR
	movs r3, #(1 << PWM_CHn)
	str r3, [r2]

	bl picoRTOS_tick
	RESTORE_CONTEXT r0
	pop {pc}	/* return */

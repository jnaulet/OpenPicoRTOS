	.syntax unified

	/* Declarations */
	.global arch_PWM_WRAP

	#include "picoRTOS_device.h"
	#include "../picoRTOS_portasm.S"

	.equ PWM_CHn, 7
	.equ PWM_INTR, ADDR_PWM + 0xa4

	.text

	.align 1
	.thumb_func
	.type arch_PWM_WRAP, %function
arch_PWM_WRAP:
	push {lr}	/* store lr for interrupt return */
	SAVE_CONTEXT r0

	/* ack pwm */
	ldr r3, =PWM_INTR
	movs r2, #(1 << PWM_CHn)
	str r2, [r3]

	bl picoRTOS_tick
	RESTORE_CONTEXT r0
	pop {pc}	/* return */

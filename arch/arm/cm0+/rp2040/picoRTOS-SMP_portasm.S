	.syntax unified

	/* Declarations */
	.global arch_SIO_PROC1

	#include "picoRTOS_device.h"
	#include "../picoRTOS_portasm.S"

	.equ SYSTICK_CVR, 0xe000e018
	.equ SIO_FIFO_RD, ADDR_SIO + 0x58

	.text

	.align 1
	.thumb_func
	.type arch_SIO_PROC1, %function
arch_SIO_PROC1:
	push {lr}	/* store lr for interrupt return */
	SAVE_CONTEXT r0

	/* reset local timer */
	ldr r3, =SYSTICK_CVR
	str r3, [r3]

	/* empty fifo */
	ldr r3, =SIO_FIFO_RD
	ldr r3, [r3]

	bl picoRTOS_tick

	RESTORE_CONTEXT r0
	pop {pc}	/* return */

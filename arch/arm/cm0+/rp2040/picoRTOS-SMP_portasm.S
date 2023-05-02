	.syntax unified
	.cpu cortex-m0plus

	#include "picoRTOS_device.h"
	#include "../picoRTOS_portasm.S"

	.equ SYSTICK_CVR, 0xe000e018

	.text

	.thumb_func
	.type arch_SIO_PROC1, %function
	.global arch_SIO_PROC1
arch_SIO_PROC1:
	pop {r0}	/* get task sp */
	push {lr}	/* store lr for return */

	/* reset local timer */
	ldr r3, =SYSTICK_CVR
	str r3, [r3]

	bl picoRTOS_tick

	/* return */
	pop {r1}
	push {r0}	/* send back new sp */
	bx r1

	/* Declarations */
	.global arch_start_first_task
	.global arch_SYSTICK
	.global arch_syscall

	.extern picoRTOS_tick
	.extern picoRTOS_syscall

	#include "picoRTOS_device.h"

	.equ GPIOR1, ADDR_GPIO + 0x1
	.equ GPIOR2, ADDR_GPIO + 0x2
	.equ SPL, ADDR_CPU + 0xd
	.equ SPH, ADDR_CPU + 0xe
	.equ SREG, ADDR_CPU + 0xf

	.macro PRESAVE_CONTEXT
	push r28
	push r29
	.endm

	.macro SWAP_STACK
	in r21, GPIOR2
	in r20, GPIOR1
	in r23, SPH
	in r22, SPL
	out GPIOR2, r23
	out GPIOR1, r22
	out SPH, r21
	out SPL, r20
	.endm

	.macro SAVE_CONTEXT
	push r0
	in r0, SREG
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
	SWAP_STACK
	.endm

	.macro RESTORE_CONTEXT
	SWAP_STACK
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	out SREG, r0
	pop r0
	pop r29
	pop r28
	.endm

	.text

	.type arch_start_first_task, %function
arch_start_first_task:
	/* prepare stack sp */
	out GPIOR2, r25
	out GPIOR1, r24
	RESTORE_CONTEXT
	reti	/* auto-enable interrupts */

	.type arch_SYSTICK, %function
arch_SYSTICK:
	/* ack RTC OVF */
	ldi r24, 0x1
	sts ADDR_RTC + 0x3, r24
	/* get task sp to r25:r24 */
	in r25, GPIOR2
	in r24, GPIOR1
	/* execute tick */
	call picoRTOS_tick
	/* store back task sp */
	out GPIOR2, r25
	out GPIOR1, r24
	ret

	.type arch_syscall, %function
arch_syscall:
	cli	/* mimic interrupt entry behavior */
	PRESAVE_CONTEXT
	SAVE_CONTEXT
	/* set task sp to Y */
	in r29, GPIOR2
	in r28, GPIOR1
	/* get priv to r21:r20 */
	ldd r21, Y+7
	ldd r20, Y+8
	/* get syscall to r23:r22 */
	ldd r23, Y+5
	ldd r22, Y+6
	/* get task sp to r25:r24 */
	mov r25, r29
	mov r24, r28
	/* syscall */
	call picoRTOS_syscall
	/* jump out */
	out GPIOR2, r25
	out GPIOR1, r24
	RESTORE_CONTEXT
	reti	/* automatically re-activate interrupts */

	/* INTERRUPTS */
	.global ISR_TABLE
	.comm ISR_TABLE, DEVICE_INTERRUPT_VECTOR_COUNT, 4

vector_isr_handler:
	SAVE_CONTEXT
	ld  r30, Y
	ldd r31, Y+1
	ldd r24, Y+2
	ldd r25, Y+3
	icall
	RESTORE_CONTEXT
	reti

	.macro EXPORT__VECTOR_ n
	.global __vector_\n
	.endm

	.macro __VECTOR_ n
	__vector_\n:
	.endm

	.set i, 0
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.altmacro
	EXPORT__VECTOR_ %i+1
__VECTOR_ %i+1
	.noaltmacro
	PRESAVE_CONTEXT
	ldi r28, lo8(ISR_TABLE+(i*4))
	ldi r29, hi8(ISR_TABLE+(i*4))
	rjmp vector_isr_handler
	.set i, i+1
	.endr

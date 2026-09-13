	/* Declarations */
	.extern picoRTOS_syscall
	.extern picoRTOS_irq

	.local BSP
	.comm  BSP, 2, 2

	.macro SWAP_STACK
	mov &BSP, R15
	mov R1, &BSP
	mov R15, R1
	.endm

	.macro SAVE_CONTEXT_IRQ_STEP1
	push R14
	.endm

	.macro SAVE_CONTEXT_IRQ_STEP2
	push R4
	push R5
	push R6
	push R7
	push R8
	push R9
	push R10
	push R11
	push R12
	push R13
	push R15
	SWAP_STACK
	.endm

	.macro SAVE_CONTEXT
	SAVE_CONTEXT_IRQ_STEP1
	SAVE_CONTEXT_IRQ_STEP2
	.endm

	.macro RESTORE_CONTEXT
	SWAP_STACK
	pop R15
	pop R13
	pop R12
	pop R11
	pop R10
	pop R9
	pop R8
	pop R7
	pop R6
	pop R5
	pop R4
	pop R14
	.endm

	.text

	.type arch_save_first_context, %function
	.global arch_save_first_context
arch_save_first_context:
	/* switch to task sp */
	mov R15, &BSP
	SWAP_STACK
	/* ret pc (fn) */
	push R14
	push #0x8 /* GIE */
	/* priv is R13 */
	mov R13, R15
	SAVE_CONTEXT
	mov &BSP, R15
	ret

	.type arch_start_first_task, %function
	.global arch_start_first_task
arch_start_first_task:
	/* prepare stack sp */
	mov R15, &BSP
	RESTORE_CONTEXT
	reti	/* auto-enable interrupts */

	.type arch_syscall, %function
	.global arch_syscall
arch_syscall:
	/* mimic interrupt entry behavior */
	dint
	push R2
	SAVE_CONTEXT
	mov &BSP, R15		; task sp
	mov 0(R15), R14		; syscall
	mov 22(R15), R13	; priv
	call #picoRTOS_syscall
	mov R15, &BSP
	RESTORE_CONTEXT
	reti

	/* INTERRUPTS */

	.global SysTick_Handler
SysTick_Handler:
	SAVE_CONTEXT
	mov &BSP, R15
	call #picoRTOS_tick
	mov R15, &BSP
	RESTORE_CONTEXT
	reti

	.global NMI_Handler
NMI_Handler:
	jmp .
	
	#include "picoRTOS_device.h"

Default_Handler:
	SAVE_CONTEXT_IRQ_STEP2
	mov &BSP, R15		; task sp
	call #picoRTOS_irq
	RESTORE_CONTEXT
	reti

	.macro EXPORT__VECTOR_ n
	.global __vector_\n
	.endm

	.macro __VECTOR_ n
	__vector_\n:
	.endm

	.set i, 0
	.rept DEVICE_INTERRUPT_VECTOR_COUNT - 1
	.altmacro
	EXPORT__VECTOR_ %i
__VECTOR_ %i
	.noaltmacro
	SAVE_CONTEXT_IRQ_STEP1
	/* get irq to R14 */
	mov #i, R14
	jmp Default_Handler
	.set i, i+1
	.endr

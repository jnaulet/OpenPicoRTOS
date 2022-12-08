	/* Declarations */
	.global arch_start_first_task
	.global arch_SYSTICK
	.global arch_syscall

	.extern picoRTOS_tick
	.extern picoRTOS_syscall

	.equ GPIOR1, 0x2a
	.equ GPIOR2, 0x2b
	.equ SPL, 0x3d
	.equ SPH, 0x3e
	.equ SREG, 0x3f

	#include "picoRTOS_device.h"

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
	.global __vector_1
	.global __vector_2
	.global __vector_3
	.global __vector_4
	.global __vector_5
	.global __vector_6
	.global __vector_7
	.global __vector_8
	.global __vector_9
	.global __vector_10
	.global __vector_11
	.global __vector_12
	.global __vector_13
	.global __vector_14
	.global __vector_15
	.global __vector_16
	.global __vector_17
	.global __vector_18
	.global __vector_19
	.global __vector_20
	.global __vector_21
	.global __vector_22
	.global __vector_23
	.global __vector_24
	.global __vector_25
	.global __vector_26
	.global __vector_27
	.global __vector_28
	.global __vector_29
	.global __vector_30
	.global __vector_31
	.global __vector_32
	.global __vector_33
	.global __vector_34
	.global __vector_35
	.global __vector_36
	.global __vector_37
	.global __vector_38
	.global __vector_39
	.global __vector_40
	.global __vector_41
	.global __vector_42
	.global __vector_43
	.global __vector_44
	.global __vector_45
	.global __vector_46
	.global __vector_47
	.global __vector_48
	.global __vector_49
	.global __vector_50
	.global __vector_51
	.global __vector_52
	.global __vector_53
	.global __vector_54
	.global __vector_55
	.global __vector_56
	.global ISR_TABLE
	.comm   ISR_TABLE, DEVICE_INTERRUPT_VECTOR_COUNT, 4

vector_isr_handler:
	SAVE_CONTEXT
	ld  r30, Y
	ldd r31, Y+1
	ldd r24, Y+2
	ldd r25, Y+3
	icall
	RESTORE_CONTEXT
	reti

	.macro VECTOR_ISR imm
	PRESAVE_CONTEXT
	ldi r28, lo8(ISR_TABLE+(\imm*4))
	ldi r29, hi8(ISR_TABLE+(\imm*4))
	rjmp vector_isr_handler
	.endm

__vector_1:
	VECTOR_ISR 0
__vector_2:
	VECTOR_ISR 1
__vector_3:
	VECTOR_ISR 2
__vector_4:
	VECTOR_ISR 3
__vector_5:
	VECTOR_ISR 4
__vector_6:
	VECTOR_ISR 5
__vector_7:
	VECTOR_ISR 6
__vector_8:
	VECTOR_ISR 7
__vector_9:
	VECTOR_ISR 8
__vector_10:
	VECTOR_ISR 9
__vector_11:
	VECTOR_ISR 10
__vector_12:
	VECTOR_ISR 11
__vector_13:
	VECTOR_ISR 12
__vector_14:
	VECTOR_ISR 13
__vector_15:
	VECTOR_ISR 14
__vector_16:
	VECTOR_ISR 15
__vector_17:
	VECTOR_ISR 16
__vector_18:
	VECTOR_ISR 17
__vector_19:
	VECTOR_ISR 18
__vector_20:
	VECTOR_ISR 19
__vector_21:
	VECTOR_ISR 20
__vector_22:
	VECTOR_ISR 21
__vector_23:
	VECTOR_ISR 22
__vector_24:
	VECTOR_ISR 23
__vector_25:
	VECTOR_ISR 24
__vector_26:
	VECTOR_ISR 25
__vector_27:
	VECTOR_ISR 26
__vector_28:
	VECTOR_ISR 27
__vector_29:
	VECTOR_ISR 28
__vector_30:
	VECTOR_ISR 29
__vector_31:
	VECTOR_ISR 30
__vector_32:
	VECTOR_ISR 31
__vector_33:
	VECTOR_ISR 32
__vector_34:
	VECTOR_ISR 33
__vector_35:
	VECTOR_ISR 34
__vector_36:
	VECTOR_ISR 35
__vector_37:
	VECTOR_ISR 36
__vector_38:
	VECTOR_ISR 37
__vector_39:
	VECTOR_ISR 38
__vector_40:
	VECTOR_ISR 39
__vector_41:
	VECTOR_ISR 40
__vector_42:
	VECTOR_ISR 41
__vector_43:
	VECTOR_ISR 42
__vector_44:
	VECTOR_ISR 43
__vector_45:
	VECTOR_ISR 44
__vector_46:
	VECTOR_ISR 45
__vector_47:
	VECTOR_ISR 46
__vector_48:
	VECTOR_ISR 47
__vector_49:
	VECTOR_ISR 48
__vector_50:
	VECTOR_ISR 49
__vector_51:
	VECTOR_ISR 50
__vector_52:
	VECTOR_ISR 51
__vector_53:
	VECTOR_ISR 52
__vector_54:
	VECTOR_ISR 53
__vector_55:
	VECTOR_ISR 54
__vector_56:
	VECTOR_ISR 55

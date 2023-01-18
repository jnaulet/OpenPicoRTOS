	/* Declarations */
	.global arch_init_timer
	.global arch_start_first_task
	.global arch_syscall
	.global arch_SYSTICK

	.global arch_compare_and_swap
	.global arch_counter

	.extern picoRTOS_tick
	.extern picoRTOS_syscall

	#include "picoRTOS_device.h"

	.macro SAVE_CONTEXT
	/* reserve stack space */
	addi sp, sp, -76
	/* push registers */
	s32i a15, sp, 0
	s32i a14, sp, 4
	s32i a13, sp, 8
	s32i a12, sp, 12
	s32i a11, sp, 16
	s32i a10, sp, 20
	s32i a9, sp, 24
	s32i a8, sp, 28
	s32i a7, sp, 32
	s32i a6, sp, 36
	s32i a5, sp, 40
	s32i a4, sp, 44
	s32i a3, sp, 48
	s32i a2, sp, 52
	s32i a0, sp, 56
	/* save loop status */
	rsr a2, lcount
	s32i a2, sp, 60
	rsr a2, lbeg
	s32i a2, sp, 64
	rsr a2, lend
	s32i a2, sp, 68
	/* save shift status */
	rsr a2, sar
	s32i a2, sp, 72
	/* exchange stacks */
	wsr sp, excsave1
	rsr sp, excsave2
	.endm

	.macro RESTORE_CONTEXT
	/* exchange stacks */
	wsr sp, excsave2
	rsr sp, excsave1
	/* pop registers */
	l32i a15, sp, 0
	l32i a14, sp, 4
	l32i a13, sp, 8
	l32i a12, sp, 12
	l32i a11, sp, 16
	l32i a10, sp, 20
	l32i a9, sp, 24
	l32i a8, sp, 28
	l32i a7, sp, 32
	l32i a6, sp, 36
	l32i a5, sp, 40
	l32i a4, sp, 44
	l32i a3, sp, 48
	l32i a2, sp, 52
	l32i a0, sp, 56
	/* restore loop status */
	l32i a2, sp, 60
	wsr a2, lcount
	l32i a2, sp, 64
	wsr a2, lbeg
	l32i a2, sp, 68
	wsr a2, lend
	/* restore shift status */
	l32i a2, sp, 72
	wsr a2, sar
	/* re-claim stack space */
	addi sp, sp, 76
	.endm

	.text

	.align 4
	.type arch_init_timer, %function
arch_init_timer:
	/* save period */
	movi a3, _timer_period
	s32i a2, a3, 0
	/* set timer inital value */
	rsr a3, ccount
	add a2, a2, a3
	wsr a2, ccompare0
	ret

	.align 4
	.type arch_start_first_task, %function
arch_start_first_task:
	wsr a2, excsave1
	RESTORE_CONTEXT
	ret

	.align 4
	.type arch_SYSTICK, %function
arch_SYSTICK:
	rsr a2, excsave1
	call0 picoRTOS_tick
	wsr a2, excsave1	/* store next task stack */
	/* prepare next ccompare */
	/* load period */
	movi a3, _timer_period
	l32i a2, a3, 0
	/* set compare value */
	rsr a3, ccompare0
	add a3, a3, a2
	wsr a3, ccompare0
	rsync
	ret

	.align 4
	.type arch_SYSCALL, %function
arch_SYSCALL:
	/* prepare picoRTOS_syscall params */
	mov a4, a3
	mov a3, a2
	rsr a2, excsave1
	call0 picoRTOS_syscall
	wsr a2, excsave1
	/* increment EPC1 by 3 (see doc) */
	rsr a2, epc1
	addi a2, a2, 3
	wsr a2, epc1
	/* jump out */
	j UserException_Handler_exit

	.align 4
	.type arch_syscall, %function
arch_syscall:
	syscall
	ret

	.align 4
	.type arch_compare_and_swap, %function
arch_compare_and_swap:
	l32ai a5, a2, 0
	wsr a5, scompare1
	s32c1i a4, a2, 0
	bne a4, a3, .arch_compare_and_swap_fail
	mov a2, a3	/* success */
.arch_compare_and_swap_fail:
	ret

	.align 4
	.type arch_counter, %function
arch_counter:
	rsr a2, ccount
	rsr a3, ccompare0
	movi a4, _timer_period
	s32i a4, a4, 0
	sub a3, a3, a4
	sub a2, a2, a3
	ret

	/* EXCEPTIONS */
	.global ISR_TABLE
	.global arch_intenable
	.global UserException_Handler

	.align 4
	.type arch_intenable, %function
arch_intenable:
	xsr a2, intenable
	esync
	ret

	.align 4
arch_L1INTR:
	rsr a2, intenable
	rsr a3, interrupt
	and a2, a2, a3
	/* extract msbit index in a3 */
	movi a3, 31
	nsau a2, a2
	sub a3, a3, a2
	/* find entry in table */
	movi a4, ISR_TABLE
	addx8 a3, a3, a4
	l32i a4, a3, 0
	l32i a2, a3, 4
	callx0 a4
	/* jump out */
	j UserException_Handler_exit

	.equ EXCCAUSE_SYSCALL, 1
	.equ EXCCAUSE_LEVEL1INTERRUPT, 4

	.align 4
UserException_Handler:
	SAVE_CONTEXT
	rsr a0, exccause
	beqi a0, EXCCAUSE_SYSCALL, arch_SYSCALL
	beqi a0, EXCCAUSE_LEVEL1INTERRUPT, arch_L1INTR
UserException_Handler_exit:
	RESTORE_CONTEXT
	rsync
	rfe

	/* DATA */

	.section .sdata

	.align 4
	.type _timer_period, %object
_timer_period:
	.long 0

	.align 4
	.type ISR_TABLE, %object
ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.long 0
	.endr

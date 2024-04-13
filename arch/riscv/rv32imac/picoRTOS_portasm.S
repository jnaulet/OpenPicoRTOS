	/* Declarations */
	.extern picoRTOS_tick
	.extern picoRTOS_syscall

	#include "picoRTOS_device.h"
	#include "../include/regs.h"
	#include <generated/autoconf.h>

	.macro SAVE_CONTEXT
	/* cpu */
	addi sp, sp, -116
	sw ra, 0 (sp)
	sw t0, 4 (sp)
	sw t1, 8 (sp)
	sw t2, 12 (sp)
	sw s0, 16 (sp)
	sw s1, 20 (sp)
	sw a0, 24 (sp)
	sw a1, 28 (sp)
	sw a2, 32 (sp)
	sw a3, 36 (sp)
	sw a4, 40 (sp)
	sw a5, 44 (sp)
	sw a6, 48 (sp)
	sw a7, 52 (sp)
	sw s2, 56 (sp)
	sw s3, 60 (sp)
	sw s4, 64 (sp)
	sw s5, 68 (sp)
	sw s6, 72 (sp)
	sw s7, 76 (sp)
	sw s8, 80 (sp)
	sw s9, 84 (sp)
	sw s10, 88 (sp)
	sw s11, 92 (sp)
	sw t3, 96 (sp)
	sw t4, 100 (sp)
	sw t5, 104 (sp)
	sw t6, 108 (sp)
	/* mepc */
	csrr t0, mepc
	sw t0, 112 (sp)
	/* swap stack */
	csrrw sp, mscratch, sp
	.endm

	.macro RESTORE_CONTEXT
	/* swap stack */
	csrrw sp, mscratch, sp
	/* mepc */
	lw t0, 112 (sp)
	csrw mepc, t0
	/* cpu */
	lw ra, 0 (sp)
	lw t0, 4 (sp)
	lw t1, 8 (sp)
	lw t2, 12 (sp)
	lw s0, 16 (sp)
	lw s1, 20 (sp)
	lw a0, 24 (sp)
	lw a1, 28 (sp)
	lw a2, 32 (sp)
	lw a3, 36 (sp)
	lw a4, 40 (sp)
	lw a5, 44 (sp)
	lw a6, 48 (sp)
	lw a7, 52 (sp)
	lw s2, 56 (sp)
	lw s3, 60 (sp)
	lw s4, 64 (sp)
	lw s5, 68 (sp)
	lw s6, 72 (sp)
	lw s7, 76 (sp)
	lw s8, 80 (sp)
	lw s9, 84 (sp)
	lw s10, 88 (sp)
	lw s11, 92 (sp)
	lw t3, 96 (sp)
	lw t4, 100 (sp)
	lw t5, 104 (sp)
	lw t6, 108 (sp)
	addi sp, sp, 116
	.endm

	.macro INCREASE_MTIMECMP src
	la t0, timer_core_period
	lw t0, 0 (t0)
	la t1, \src
	lw t2, 0 (t1) /* srcl */
	lw t3, 4 (t1) /* srch */
	add t2, t0, t2
	bltu t0, t2, 1f
	/* propagate carry */
	addi t3, t3, 1
1:
	li t0, -1
	la t1, mtimecmp
	sw t0, 0 (t1)
	sw t3, 4 (t1)
	sw t2, 0 (t1)
	.endm

	.align 4
	.type MachineTimerInterrupt_Handler, %function
	.global MachineTimerInterrupt_Handler
MachineTimerInterrupt_Handler:
	SAVE_CONTEXT
	/* tick */
	csrr a0, mscratch
	jal picoRTOS_tick
	csrw mscratch, a0
	/* reset timer */
	INCREASE_MTIMECMP mtimecmp
	/* epilogue */
	RESTORE_CONTEXT
	mret

	.align 8
	.type GeneralException_Handler, %function
	.global GeneralException_Handler
GeneralException_Handler:
	SAVE_CONTEXT
	/* check if it's a syscall or stall */
	csrr t0, mcause
	andi t0, t0, 0x7ff
	li t1, 0xb	/* only ecall for now */
	bne t0, t1, GeneralException_Unhandled
	/* fish for parameters */
	csrr a0, mscratch
	lw a1, 24 (a0)
	lw a2, 28 (a0)
	/* mepc point to ecall instr, increment it by 4 */
	lw t0, 112 (a0)
	addi t0, t0, 4
	sw t0, 112 (a0)
	/* finally, syscall */
	jal picoRTOS_syscall
	/* epilogue */
	csrw mscratch, a0
	RESTORE_CONTEXT
	mret

GeneralException_Unhandled:
	ebreak
	j GeneralException_Unhandled

	.align 4
	.type arch_save_first_context, %function
	.global arch_save_first_context
arch_save_first_context:
	/* switch to task stack */
	csrw mscratch, a0
	csrrw sp, mscratch, sp
	/* populate registers */
	or a0, a2, zero
	csrw mepc, a1
	and a1, a1, zero
	and a2, a2, zero
	SAVE_CONTEXT
	csrr a0, mscratch
	ret

	.align 4
	.type arch_start_first_task, %function
	.global arch_start_first_task
arch_start_first_task:
	/* restore first context */
	csrw mscratch, a0
	/* set first tick to now + period */
	INCREASE_MTIMECMP mtime
	RESTORE_CONTEXT
	/* emulate return from exception */
	li t0, (1 << 7) /* MPIE */
	csrs mstatus, t0
	mret

	.align 4
	.type arch_syscall, %function
	.global arch_syscall
arch_syscall:
	ecall
	ret

	.align 4
	.type arch_compare_and_swap, %function
	.global arch_compare_and_swap
arch_compare_and_swap:
	lr.w t0, (a0)
	bne t0, a1, .arch_compare_and_swap_fail_cmp
	sc.w a0, a2, (a0)
	beq a0, zero, .arch_compare_and_swap_fail_sc
	/* success */
	or a0, a1, zero
	ret
.arch_compare_and_swap_fail_cmp:
	sc.w t0, t0, (a0)	/* turn off reservation bit */
.arch_compare_and_swap_fail_sc:
	or a0, a2, zero
	ret

	.align 4
	.type PlatformDefined_Handler, %function
	.global PlatformDefined_Handler
PlatformDefined_Handler:
	SAVE_CONTEXT
	/* extract irq parameters */
	la t0, ISR_TABLE
	la t1, 0xfff
	csrr t2, mcause
	and t1, t1, t2 /* t1: exception code */
	/* find isr table entry */
	addi t1, t1, -16
	slli t1, t1, 2
	add t0, t0, t1
	lw t2, 0 (t0)
	lw a0, 4 (t0)
	/* check fn */
	beq t2, zero, isr_fn_is_null
	/* finally branch to isr */
	jr t2
	/* TODO: ack irq ? */
	/* epilogue */
	RESTORE_CONTEXT
	mret

isr_fn_is_null:
	/* fn call is null, use picoRTOS_register_interrupt() */
	ebreak
	j isr_fn_is_null

	.align 4
	.type arch_mtimel, %function
	.global arch_mtimel
arch_mtimel:
	/* mtimel */
	la t0, mtime
	lw a0, 0 (t0)
	ret

	.bss

	#include "../../include/macros.S"

	.align 4
	.type timer_core_period, %object
	.global timer_core_period
timer_core_period:
	.long 0

	.align 4
	.type ISR_TABLE, %object
	.global ISR_TABLE
ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.long 0
	.endr

	/* Declarations */
	.extern picoRTOS_tick
	.extern picoRTOS_syscall

	#include "picoRTOSConfig.h"
	#include "picoRTOS_device.h"
	/* RISC-V specifics */
	#include "../../lib/riscv/regs.h"

	.equ TIMER_CORE_PERIOD, (CONFIG_SYSCLK_HZ / CONFIG_TICK_HZ)

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
	la t0, TIMER_CORE_PERIOD
	la t1, \src
	lw a0, 0 (t1) /* srcl */
	lw a1, 4 (t1) /* srch */
	add a0, t0, a0
	bgeu t0, a0, 1f
	/* propagate carry */
	addi a1, a1, 1
1:
	li t0, -1
	la t1, mtimecmp
	sw t0, 0 (t1)
	sw a1, 4 (t1)
	sw a0, 0 (t1)
	.endm

	.align 4
	.type arch_suspend, %function
	.global arch_suspend
arch_suspend:
	la t0, ~(1 << 3) /* ~MIE */
	csrr t1, mstatus
	and t1, t1, t0
	csrw mstatus, t1
	jr ra

	.align 4
	.type arch_resume, %function
	.global arch_resume
arch_resume:
	li t0, (1 << 3) /* MIE */
	csrr t1, mstatus
	or t1, t1, t0
	csrw mstatus, t1
	jr ra

	.align 4
	.type MachineTimerInterrupt_Handler, %function
	.global MachineTimerInterrupt_Handler
MachineTimerInterrupt_Handler:
	SAVE_CONTEXT
	/* tick */
	csrr a0, mscratch
	jal picoRTOS_tick
	/* reset timer */
	INCREASE_MTIMECMP mtimecmp
	/* epilogue */
	csrw mscratch, a0
	RESTORE_CONTEXT
	mret

	.align 4
	.type MachineSoftwareInterrupt_Handler, %function
	.global MachineSoftwareInterrupt_Handler
MachineSoftwareInterrupt_Handler:
	SAVE_CONTEXT
	/* syscall */
	csrr a0, mscratch
	/* fish for parameters */
	lw a1, 28 (a0)
	lw a2, 32 (a0)
	jal picoRTOS_syscall
	/* ack irq */
	csrw msip, zero
	/* epilogue */
	csrw mscratch, a0
	RESTORE_CONTEXT
	mret

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
	jr ra

	.align 4
	.type arch_start_first_task, %function
	.global arch_start_first_task
arch_start_first_task:
	/* set first tick to now + period */
	INCREASE_MTIMECMP mtime
	/* restore first context */
	csrw mscratch, a0
	RESTORE_CONTEXT
	/* emulate return from exception */
	li t0, (1 << 7) /* MPIE */
	csrr t1, mstatus
	or t1, t1, t0
	csrw mstatus, t1
	mret

	.align 4
	.type arch_syscall, %function
	.global arch_syscall
arch_syscall:
	/* set msip */
	li t0, 1
	csrw msip, t0
	/* wait. TODO: is this necessary ? */
	wfi
	jr ra

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
	jr ra
.arch_compare_and_swap_fail_cmp:
	sc.w t0, t0, (a0)	/* turn off reservation bit */
.arch_compare_and_swap_fail_sc:
	or a0, a2, zero
	jr ra

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
	.type arch_counter, %function
	.global arch_counter
arch_counter:
	jr ra

	.bss

	.align 4
	.type ISR_TABLE, %object
	.global ISR_TABLE
ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.long 0
	.endr

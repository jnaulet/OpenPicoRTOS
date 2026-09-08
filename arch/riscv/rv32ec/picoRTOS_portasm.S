	/* Declarations */
	.extern picoRTOS_tick
	.extern picoRTOS_syscall
	.extern picoRTOS_irq

	#include "picoRTOS_device.h"
	#include "../include/regs.h"
	#include <generated/autoconf.h>

	.macro SAVE_CONTEXT
	/* cpu */
	addi sp, sp, -52
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
	/* mepc */
	csrr t0, mepc
	sw t0, 48 (sp)
	/* swap stack */
	csrrw sp, mscratch, sp
	.endm

	.macro RESTORE_CONTEXT
	/* swap stack */
	csrrw sp, mscratch, sp
	/* mepc */
	lw t0, 48 (sp)
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
	addi sp, sp, 52
	.endm

	.align 4
	.type SysTick_Handler, %function
	.global SysTick_Handler
SysTick_Handler:
	SAVE_CONTEXT
	ARCH_ACK_SYSTICK
	/* tick */
	csrr a0, mscratch
	jal picoRTOS_tick
	csrw mscratch, a0
	/* epilogue */
	RESTORE_CONTEXT
	mret

	.align 8
	.type EXC_Handler, %function
	.global EXC_Handler
EXC_Handler:
	SAVE_CONTEXT
	/* check if it's a syscall or stall */
	csrr t0, mcause
	andi t0, t0, 0x7ff
	li t1, 0xb	/* only ecall for now */
	bne t0, t1, EXC_Unhandled
	/* fish for parameters */
	csrr a0, mscratch
	lw a1, 24 (a0)
	lw a2, 28 (a0)
	/* mepc point to ecall instr, increment it by 4 */
	lw t0, 48 (a0)
	addi t0, t0, 4
	sw t0, 48 (a0)
	/* finally, syscall */
	jal picoRTOS_syscall
	/* epilogue */
	csrw mscratch, a0
	RESTORE_CONTEXT
	mret

EXC_Unhandled:
	ebreak
	j EXC_Unhandled

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
	.type PlatformDefined_Handler, %function
	.global PlatformDefined_Handler
PlatformDefined_Handler:
	SAVE_CONTEXT
	csrr a0, mscratch
	/* extract irq parameters */
	la a1, 0xfff
	csrr a2, mcause
	and a1, a1, a2 /* a1: exception code */
	/* find isr table entry */
	addi a1, a1, -16
	slli a1, a1, 2
	jal picoRTOS_irq
	/* TODO: ack irq ? */
	/* epilogue */
	RESTORE_CONTEXT
	mret


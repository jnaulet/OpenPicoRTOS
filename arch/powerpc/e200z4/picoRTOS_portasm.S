	.file "picoRTOS_portasm.S"

	/* picoRTOS */
	.extern picoRTS_syscall
	.extern picoRTOS_tick

	#include "picoRTOS_device.h"
	#include "../include/macros.S"

	/* macros */

	.macro SWAP_STACK rS
	se_mr \rS, r1
	mfsprg r1, 0
	mtsprg 0, \rS
	.endm

	.macro SAVE_CONTEXT
	e_add16i r1, r1, -140
	sthcx. r1, 0, r1	/* clear exclusive */
	e_stmvsrrw 0 (r1)	/* srr[0-1] */
	e_stmvsprw 8 (r1)	/* cr, lr, ctr, xer */
	e_stmvgprw 24 (r1)	/* r[0, 3-12] */
	e_stmw r14, 68 (r1)	/* r[14-31] */
	SWAP_STACK r0
	.endm

	.macro RESTORE_CONTEXT
	SWAP_STACK r0
	e_lmvsrrw 0 (r1)	/* srr[0-1] */
	e_lmvsprw 8 (r1)	/* cr, lr, ctr, xer */
	e_lmvgprw 24 (r1)	/* r[0, 3-12] */
	e_lmw r14, 68 (r1)	/* r[14-31] */
	e_add16i r1, r1, 140
	sync
	.endm

	.section .text

	.align 4
	.global SystemCall_Handler
SystemCall_Handler:
	SAVE_CONTEXT
	mfsprg r3, 0		/* move task stack pointer to r3 */
	e_lwz r4, 28 (r3)	/* get syscall from task stack */
	e_lwz r5, 32 (r3)	/* get priv from task stack */
	e_bl picoRTOS_syscall
	mtsprg 0, r3		/* save returned stack to sprg0 */
	RESTORE_CONTEXT
	se_rfi

	.align 4
	.type arch_save_first_context, %function
	.global arch_save_first_context
arch_save_first_context:
	/* save r30-r31 */
	e_stmw r30, -8 (r1)
	/* switch to task stack */
	mtsprg 0, r3
	SWAP_STACK r3
	e_add16i r1, r1, -148	/* -8 like __StackTop(s) */
	se_mr r30, r4		/* srr0 / pc (fn) */

	/* BUG: i can't get the user-mode PBDRIDGE to work
	 * so we'll have to sacrifice some safety here until someone
	 * at NXP can explain to us how it works */
	E_LIW r31, 0x2029000	/* SPE, CE, EE, ME */
	//E_LIW r31, 0x202d000	/* SPE, CE, EE, PR, ME */

	e_stmw r30, 0 (r1)	/* srr[0-1] */
	e_stw r5, 28 (r1)	/* r3 (priv) */
	/* swap & ret */
	SWAP_STACK r3
	/* restore r30-r31 */
	e_lmw r30, -8 (r1)
	se_blr

	.align 4
	.type arch_start_idle_task, %function
	.global arch_start_idle_task
arch_start_idle_task:
	mtsprg 0, r3
	RESTORE_CONTEXT
	se_rfi

	.align 4
	.type arch_core, %function
	.global arch_core
arch_core:
	mfpir r3
	se_blr

	.align 4
	.type arch_dcbi, %function
	.global arch_dcbi
arch_dcbi:
	dcbi 0, r3
	se_blr

	.align 4
	.type arch_dcbf, %function
	.global arch_dcbf
arch_dcbf:
	dcbf 0, r3
	se_blr

	.equ INTC_IACKR0, ADDR_INTC + 0x20
	.equ INTC_EOIR0, ADDR_INTC + 0x30

	.macro INTC_IACKRx_to_r3
	mfpir r4
	se_slwi r4, 2
	e_lis r3, INTC_IACKR0@ha
	add r3, r4, r3
	e_lwz r3, INTC_IACKR0@l(r3)
	.endm

	.macro ZERO_INTC_EOIRx
	mfpir r4
	se_slwi r4, 2
	e_lis r3, INTC_EOIR0@ha
	add r3, r4, r3
	e_li r4, 0
	e_stw r4, INTC_EOIR0@l(r3)
	.endm

	.align 4
	.global ExternalInput_Handler
ExternalInput_Handler:
	SAVE_CONTEXT
	INTC_IACKRx_to_r3
	/* dereference & check fn */
	se_lwz r4, 0 (r3)
	se_cmpi r4, 0
	se_beq isr_fn_is_null
	/* set lr */
	se_mtlr r4
	/* priv in r3 */
	e_rlwinm r4, r3, 0, 20, 30	/* intvec in r4 */
	e_lis r3, ISR_TABLE_priv@ha
	add r3, r4, r3
	e_lwz r3, ISR_TABLE_priv@l(r3)
	/* branch to isr */
	se_mr r31, r1
	se_blrl
	mbar
	/* ack */
	ZERO_INTC_EOIRx
	/* epilogue */
	RESTORE_CONTEXT
	se_rfi

isr_fn_is_null:
	/* fn call is null, use picoRTOS_register_interrupt() */
	se_illegal
	se_nop
	e_b isr_fn_is_null

	.align 4
	.global MachineCheck_Handler
	.global DataStorage_Handler
	.global InstructionStorage_Handler
MachineCheck_Handler:
	SAVE_CONTEXT
	mfmcsr r3
	mtmcsr r3
	mfmcsrr0 r5	/* faulty pc */
	e_b .segfault
DataStorage_Handler:
InstructionStorage_Handler:
	SAVE_CONTEXT
	e_li r5, 1	/* FIXME */
.segfault:
	mfsprg r3, 0
	e_li r4, 8	/* SEGFAULT (FIXME) */
	e_bl picoRTOS_syscall
	RESTORE_CONTEXT
	se_rfi

	.section .bss

	.align 12 /* VTBA is 20bits, align 2^12 (4096) */
	.type ISR_TABLE_fn, %object
	.global ISR_TABLE_fn
ISR_TABLE_fn:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.endr

	.align 4
	.type ISR_TABLE_priv, %object
	.global ISR_TABLE_priv
ISR_TABLE_priv:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.endr

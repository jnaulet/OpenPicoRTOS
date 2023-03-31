	/* picoRTOS */
	.extern picoRTS_syscall
	.extern picoRTOS_tick

	#include "picoRTOS_device.h"
	#include "../../lib/ppc/vle/macros.S"

	.equ INTC_IACKR, ADDR_INTC + 0x20
	.equ INTC_EOIR, ADDR_INTC + 0x30

	/* macros */

	.macro SWAP_STACK
	se_mr r0, r1
	mfsprg r1, 0
	mtsprg 0, r0
	.endm

	.macro SAVE_CONTEXT
	e_add16i r1, r1, -140
	e_stmvsrrw 0 (r1)	/* srr[0-1] */
	e_stmvsprw 8 (r1)	/* cr, lr, ctr, xer */
	e_stmvgprw 24 (r1)	/* r[0, 3-12] */
	e_stmw r14, 68 (r1)	/* r[14-31] */
	SWAP_STACK
	.endm

	.macro RESTORE_CONTEXT
	SWAP_STACK
	sthcx. r1, 0, r1	/* turn reservation bit off */
	e_lmvsrrw 0 (r1)	/* srr[0-1] */
	e_lmvsprw 8 (r1)	/* cr, lr, ctr, xer */
	e_lmvgprw 24 (r1)	/* r[0, 3-12] */
	e_lmw r14, 68 (r1)	/* r[14-31] */
	e_add16i r1, r1, 140
	sync
	.endm

	.macro ADD_CORE_OFFSET reg
	mfpir r0
	se_slwi r0, 2
	add \reg, r0, \reg
	.endm

	.section .text

	.align 4
	.global Decrementer_Handler
Decrementer_Handler:
	SAVE_CONTEXT
	mfsprg r3, 0		/* load task stack pointer from sprg0 */
	e_bl picoRTOS_tick
	mtsprg 0, r3		/* store returned task stack pointer */
	/* clear decrementer */
	mftsr r0
	E_LIW r1, ~(1 << 26)	/* TSR[DIS] */
	and r0, r0, r1
	mttsr r0
	/* epilogue */
	RESTORE_CONTEXT
	se_rfi

	.align 4
	.global SystemCall_Handler
SystemCall_Handler:
	SAVE_CONTEXT
	mfspr r3, 0		/* move task stack pointer to r3 */
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
	/* switch to task stack */
	mtsprg 0, r3
	SWAP_STACK
	e_add16i r1, r1, -140
	se_mr r30, r4		/* srr0 / pc (fn) */
	E_LIW r31, 0x2029000	/* SPE, CE, EE, ME */
	mfmsr r3
	or r31, r31, r3
	e_stmw r30, 0 (r1)	/* srr[0-1] */
	e_stw r5, 28 (r1)	/* r3 (priv) */
	/* ret */
	se_mr r3, r1
	SWAP_STACK
	se_blr

	.align 4
	.type arch_start_first_task, %function
	.global arch_start_first_task
arch_start_first_task:
	mtsprg 0, r3
	RESTORE_CONTEXT

	/* start timebase */
	mfhid0 r3
	e_li r4, (1 << 14) /* TBEN */
	or r3, r3, r4
	mthid0 r3

	se_rfi

	.align 4
	.type arch_syscall, %function
	.global arch_syscall
arch_syscall:
	se_sc
	se_blr

	.align 4
	.type arch_compare_and_swap, %function
	.global arch_compare_and_swap
arch_compare_and_swap:
	mbar
	lharx r7, 0, r3
	se_cmp r7, r4
	se_bne .arch_compare_and_swap_fail_cmp
	sthcx. r5, 0, r3
	se_bne .arch_compare_and_swap_fail_sthcx
	mbar
	se_mr r3, r4		/* success */
	se_blr
.arch_compare_and_swap_fail_cmp:
	sthcx. r7, 0, r3	/* turn reservation bit off */
	mbar
.arch_compare_and_swap_fail_sthcx:
	se_mr r3, r5
	se_blr

	.align 4
	.type arch_dec_init, %function
	.global arch_dec_init
arch_dec_init:
	/* set auto-reload value */
	mtdecar r3
	/* set clock to cpu & disable */
	mfhid0 r3
	E_LIW r4, ~((1 << 14) | (1 << 13)) /* ~(TBEN | SEL_TLCLK) */
	and r3, r3, r4
	mthid0 r3
	/* set auto-reload */
	mftcr r4
	E_LIW r5, ((1 << 26 | 1 << 22)) /* DIE | ARE */
	or r4, r4, r5
	mttcr r4
	se_blr

	.align 4
	.type arch_DEC, %function
	.global arch_DEC
arch_DEC:
	mfdec r3
	se_blr

	.align 4
	.type arch_core, %function
	.global arch_core
arch_core:
	mfpir r3
	se_blr

	.align 4
	.global ExternalInput_Handler
ExternalInput_Handler:
	SAVE_CONTEXT
	/* isr fn in r4 */
	e_lis r3, INTC_IACKR@ha
	ADD_CORE_OFFSET r3
	e_lwz r3, INTC_IACKR@l(r3)
	se_lwz r4, 0 (r3)
	/* check fn */
	se_cmp r4, 0
	se_beq isr_fn_is_null
	/* set lr */
	se_mtlr r4
	/* priv in r3 */
	e_rlwinm r4, r3, 0, 20, 30	/* intvec */
	e_lis r3, ISR_TABLE_priv@ha
	add r3, r4, r3
	e_lwz r3, ISR_TABLE_priv@l(r3)
	/* branch to isr */
	se_blrl
	mbar
	/* ack */
	e_lis r3, INTC_EOIR@ha
	ADD_CORE_OFFSET r3
	e_stw r3, INTC_EOIR@l(r3)
	mbar
	/* epilogue */
	RESTORE_CONTEXT
	se_rfi

isr_fn_is_null:
	/* fn call is null, use picoRTOS_register_interrupt() */
	se_illegal
	nop
	e_b isr_fn_is_null

	.section .bss

	.align 4
	.type ISR_TABLE_priv, %object
	.global ISR_TABLE_priv
ISR_TABLE_priv:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.endr

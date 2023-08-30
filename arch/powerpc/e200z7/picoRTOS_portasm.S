	/* picoRTOS */
	.extern picoRTS_syscall
	.extern picoRTOS_tick

	#include "picoRTOS_device.h"
	#include "include/macros.S"

	.extern arch_get_interrupt
	.extern arch_ack_interrupt

	/* macros */

	.macro SWAP_STACK rS
	se_mr \rS, r1
	mfsprg r1, 0
	mtsprg 0, \rS
	.endm

	.macro SAVE_CONTEXT
	e_add16i r1, r1, -140
	e_stmvsrrw 0 (r1)	/* srr[0-1] */
	e_stmvsprw 8 (r1)	/* cr, lr, ctr, xer */
	e_stmvgprw 24 (r1)	/* r[0, 3-12] */
	e_stmw r14, 68 (r1)	/* r[14-31] */
	SWAP_STACK r0
	.endm

	.macro RESTORE_CONTEXT
	SWAP_STACK r0
	sthcx. r1, 0, r1	/* turn reservation bit off */
	e_lmvsrrw 0 (r1)	/* srr[0-1] */
	e_lmvsprw 8 (r1)	/* cr, lr, ctr, xer */
	e_lmvgprw 24 (r1)	/* r[0, 3-12] */
	e_lmw r14, 68 (r1)	/* r[14-31] */
	e_add16i r1, r1, 140
	sync
	.endm

	.macro DEF_MFSPR name, SPR
	.align 4
	.type \name, %function
	.global \name
\name:
	mfspr r3, \SPR
	se_blr
	.endm
	
	.macro DEF_MTSPR name, SPR
	.align 4
	.type \name, %function
	.global \name
\name:
	mtspr \SPR, r3
	se_blr
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
	mftsr r3
	E_LIW r4, ~(1 << 26)	/* TSR[DIS] */
	and r3, r3, r4
	mttsr r3
	/* epilogue */
	RESTORE_CONTEXT
	se_rfi

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
	e_add16i r1, r1, -140
	se_mr r30, r4		/* srr0 / pc (fn) */
	E_LIW r31, 0x2029000	/* SPE, CE, EE, ME */
	mfmsr r3
	or r31, r31, r3
	e_stmw r30, 0 (r1)	/* srr[0-1] */
	e_stw r5, 28 (r1)	/* r3 (priv) */
	/* swap & ret */
	SWAP_STACK r3
	/* restore r30-r31 */
	e_lmw r30, -8 (r1)
	se_blr

	.align 4
	.type arch_start_first_task, %function
	.global arch_start_first_task
arch_start_first_task:
	/* start timebase */
	mfhid0 r7
	e_li r8, (1 << 14) /* TBEN */
	or r7, r7, r8
	mthid0 r7
	/* run */
	mtsprg 0, r3
	RESTORE_CONTEXT
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

	/* SPRs */

	DEF_MFSPR arch_mfdec, 22
	DEF_MFSPR arch_mfdecar, 54
	DEF_MFSPR arch_core, 286
	DEF_MFSPR arch_mftcr, 340
	DEF_MFSPR arch_mfhid0, 1008
	DEF_MTSPR arch_mtdec, 22
	DEF_MTSPR arch_mtdecar, 54
	DEF_MTSPR arch_mttcr, 340
	DEF_MTSPR arch_mthid0, 1008

	/* CACHES */
	
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

	.align 4
	.global ExternalInput_Handler
ExternalInput_Handler:
	SAVE_CONTEXT
	/* isr fn in r4 */
	e_bl arch_get_interrupt
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
	e_bl arch_ack_interrupt
	mbar
	/* epilogue */
	RESTORE_CONTEXT
	se_rfi

isr_fn_is_null:
	/* fn call is null, use picoRTOS_register_interrupt() */
	se_illegal
	se_nop
	e_b isr_fn_is_null

	.section .bss

	.align 4
	.type ISR_TABLE_priv, %object
	.global ISR_TABLE_priv
ISR_TABLE_priv:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.endr

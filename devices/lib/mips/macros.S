	/* MIPS32 macros & definitions (messy)
	 * Provides a few useful macros:
	 * COPY_TO_RAM
	 * ZERO_RAM
	 * INIT_EBASE
	 * SET_VECTOR_SPACING
	 * RESET_CP0_CAUSE
	*/
	#include "regs.h"

	/* COPY_TO_RAM */
	.macro COPY_TO_RAM base, start, end
	la t0, \base
	la t1, \start
	la t2, \end
	b 2f
	nop
1:
	lw at, (t0)
	sw at, (t1)
	addiu t0, t0, 4
	addiu t1, t1, 4
2:
	bne t1, t2, 1b
	nop
	.endm

	/* ZERO_RAM */
	.macro ZERO_RAM start, end
	la t0, \start
	la t1, \end
	b 2f
	nop
1:
	sw zero, (t0)
	addiu t0, t0, 4
2:
	bne t0, t1, 1b
	nop
	.endm

	/* INIT_EBASE */
	.macro INIT_EBASE base
	la t0, \base
	mtc0 t0, CP0_EBASE
	.endm

	/* SET_VECTRO_SPACING */
	.macro SET_VECTOR_SPACING spacing
	li t0, \spacing
	mtc0 t0, CP0_INTCTL
	.endm

	/* RESET_CP0_CAUSE */
	.macro RESET_CP0_CAUSE
	li t0, CP0_CAUSE_IV
	mtc0 t0, CP0_CAUSE
	.endm

	/* RESET_CP0_STATUS */
	.macro RESET_CP0_STATUS
	mfc0 t0, CP0_STATUS
	li t1, (CP0_STATUS_BEV | CP0_STATUS_SR | CP0_STATUS_NMI)
	and t0, t0, t1
	mtc0 t0, CP0_STATUS
	.endm

	/* RESET_CACHE */
	.equ OP_INDEX_STORE_TAG_I, 0x8
	.equ OP_INDEX_STORE_TAG_D, 0x9

	.macro RESET_CACHE start, size, lsize, op
	mtc0 zero, CP0_TAGLO
	la t0, \start
	la t1, \size
	b 2f
	nop
1:
	addiu t0, t0, \lsize
	addiu t1, t1, -\lsize
2:
	bne t1, zero, 1b
	cache \op, -4 (t0)
	.endm

	/* SET_CACHE_COHERENCY */
	.equ CACHE_WBACK_WALLOC,   3
	.equ CACHE_WTHRU_WALLOC,   1
	.equ CACHE_WTHRU_NOWALLOC, 0
	.equ CACHE_DISABLE,	   2

	.macro SET_CACHE_COHERENCY num
	mfc0 t0, CP0_CONFIG
	li t1, ~CP0_CONFIG_K0_M
	and t0, t0, t1
	or t0, \num
	mtc0 t0, CP0_CONFIG
	.endm

	/* CLEAR_CP0_STATUS_BEV */
	.macro CLEAR_CP0_STATUS_BEV
	mfc0 t0, CP0_STATUS
	li t1, ~CP0_STATUS_BEV
	and t0, t0, t1
	mtc0 t0, CP0_STATUS
	.endm

	/* ENABLE_FPU64 */
	.macro ENABLE_FPU64
	mfc0 t0, CP0_STATUS
	li t1, (CP0_STATUS_CU1 | CP0_STATUS_FR)
	or t0, t0, t1
	mtc0 t0, CP0_STATUS
	/* fpu control and status reg */
	li t0, 0x1000000
	ctc1 t0, $31
	.endm

	/* ENABLE_DSP */
	.macro ALLOW_DSP_ACCESS
	mfc0 t0, CP0_STATUS
	li t1, CP0_STATUS_MX
	or t0, t0, t1
	mtc0 t0, CP0_STATUS
	.endm

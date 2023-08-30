	/* picoRTOS */
	.extern arch_init_ivors
	.extern arch_start_first_task

	#include "include/macros.S"
	#include <generated/autoconf.h>

	.macro ARCH_INIT_CORE_STACK
	E_LIW r1, arch_core_sp
	mfpir r0
	e_sub16i r0, r0, 1
	se_slwi r0, 2
	add r1, r1, r0
	se_lwz r1, 0 (r1)
	.endm

	.section .fini, "ax"

	.align 4
	.type arch_core_start, %function
	.global arch_core_start
arch_core_start:
	wrteei 0	/* disable interrupts */
	TURN_WATCHDOG_OFF

	/* MMU */
	e_bl arch_init_mmu

	ENABLE_BTB
	ENABLE_MSR_ME

	INVALIDATE_AND_ENABLE_CACHE ICACHE
	INVALIDATE_AND_ENABLE_CACHE DCACHE

	ARCH_INIT_CORE_STACK
	E_LIW r2, _SDA2_BASE_	/* provided by powerpc-eabivle- toolchain */
	E_LIW r13, _SDA_BASE_	/* provided by powerpc-eabivle- toolchain */

	/* IVPR */
	E_LIW r0, __vectors_start__
	mtivpr r0

	/* IVORS */
	e_bl arch_init_ivors

	/* timebase */
	mfhid0 r3
	E_LIW r4, ~((1 << 14) | (1 << 13)) /* ~(TBEN | SEL_TLCLK) */
	and r3, r3, r4
	mthid0 r3

	/* get first task sp */
	e_lwz r3, 0 (r1)
	/* setup decrementer */
	e_lwz r4, 4 (r1)
	mtdecar r4
	mtdec r4
	/* consume stack */
	e_add16i r1, r1, 8

	e_bl arch_start_first_task

	.text

	.align 4
	.type arch_SSCIR0, %function
	.global arch_SSCIR0
arch_SSCIR0:
	/* preserve lr */
	e_add16i r1, r1, -4
	se_mflr r0
	e_stw r0, 0 (r1)
	/* reset DEC */
	mfdecar r3
	mtdec r3
	/* tick */
	mfsprg r3, 0		/* move task stack pointer to r3 */
	e_bl picoRTOS_tick
	mtsprg 0, r3		/* store returned task stack pointer */
	/* restore lr */
	se_lwz r0, 0 (r1)
	se_mtlr r0
	e_add16i r1, r1, 4
	/* out */
	se_blr

	.section .sbss

	.align 4
	.type arch_core_sp, %object
	.global arch_core_sp
arch_core_sp:
	.rept CONFIG_SMP_CORES - 1
	.long 0
	.endr

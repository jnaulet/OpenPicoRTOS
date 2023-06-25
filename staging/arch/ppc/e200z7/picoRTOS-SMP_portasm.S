	/* picoRTOS */
	.extern device_mmu_setup
	.extern device_init_ivpr_and_ivors
	.extern arch_start_first_task

	#include "picoRTOSConfig.h"
	#include "picoRTOS_device.h"
	#include "../../lib/ppc/vle/macros.S"

	.equ DECAR, (CONFIG_SYSCLK_HZ / CONFIG_TICK_HZ)

	.macro ARCH_INIT_CORE_STACK
	E_LIW r1, arch_core_sp
	mfpir r0
	e_sub16i r0, r0, 1
	se_slwi r0, 2
	add r1, r1, r0
	se_lwz r1, 0 (r1)
	.endm

	.section .text

	.align 4
	.type arch_core_start, %function
	.global arch_core_start
arch_core_start:
	wrteei 0	/* disable interrupts */

	/* MMU */
	e_bl device_mmu_setup

	TURN_WATCHDOG_OFF
	ENABLE_BTB
	ENABLE_MSR_ME

	INVALIDATE_AND_ENABLE_CACHE ICACHE
	INVALIDATE_AND_ENABLE_CACHE DCACHE

	ARCH_INIT_CORE_STACK
	E_LIW r2, __sdata2_start__	/* sdata2 */
	E_LIW r13, __sdata_start__	/* sdata */

	e_bl device_init_ivpr_and_ivors

	/* timebase */
	mfhid0 r3
	E_LIW r4, ~((1 << 14) | (1 << 13)) /* ~(TBEN | SEL_TLCLK) */
	and r3, r3, r4
	mthid0 r3
	/* no auto-reload */
	E_LIW r3, DECAR
	mtdec r3

	/* get first task sp */
	e_lwz r3, 0 (r1)
	e_add16i r1, r1, 4

	e_bl arch_start_first_task

	.align 4
	.type arch_SSCIR0, %function
	.global arch_SSCIR0
arch_SSCIR0:
	/* reset DEC */
	E_LIW r3, DECAR
	mtdec r3
	/* tick */
	mfspr r3, 0		/* move task stack pointer to r3 */
	e_bl picoRTOS_tick
	mtsprg 0, r3		/* store returned task stack pointer */
	se_blr

	.section .sbss

	.align 4
	.type arch_core_sp, %object
	.global arch_core_sp
arch_core_sp:
	.rept CONFIG_SMP_CORES - 1
	.long 0
	.endr

	.file "picoRTOS-SMP_portasm.S"

	/* picoRTOS */
	.extern arch_init_ivors
	.extern arch_start_idle_task
	.extern arch_init_mmu
	
	#include "include/macros.S"
	#include "../include/macros.S"
	#include <generated/autoconf.h>

	.section .rodata, "a"
	.altmacro

	.macro CORE_STACKTOP l, n
	.long \l\n
	.endm

	.global __StackTop
__StackTop:
	.set i, 0
	.rept CONFIG_CORE_COUNT
	CORE_STACKTOP __StackTop, %i
	.set i, i+1
	.endr

	.section .init, "ax"
	.noaltmacro

	.macro CORE_STACKTOP_to_r1
	mfpir r3
	se_slwi r3, 2
	e_lis r1, __StackTop@ha
	add r1, r1, r3
	e_lwz r1, __StackTop@l(r1)
	e_add16i r1, r1, -8	/* lr + backchain */
	.endm

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

	CORE_STACKTOP_to_r1
	E_LIW r2, _SDA2_BASE_	/* provided by powerpc-eabivle- toolchain */
	E_LIW r13, _SDA_BASE_	/* provided by powerpc-eabivle- toolchain */

	/* IVPR */
	E_LIW r0, __IVORs
	mtivpr r0

	/* IVORS */
	e_bl arch_init_ivors

	/* get first task sp */
	e_lwz r3, -4 (r1)
	e_bl arch_start_idle_task

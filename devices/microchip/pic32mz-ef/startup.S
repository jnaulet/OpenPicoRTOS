	.set noat
	.set noreorder

	#include "picoRTOS_device.h"
	#include "../../lib/mips/macros.S"

	.equ __vectors_start__, ADDR_INTC + 0x540
	.equ __vectors_end__, __vectors_start__ + (DEVICE_INTERRUPT_VECTOR_COUNT * 4)

	.section .init, "ax"

	.align 4
	.global Reset_Handler
Reset_Handler:
	la sp, __StackTop    /* stack */
	la gp, __gp	     /* gp */
	mtc0 zero, CP0_COUNT /* COUNT reset */

	/* relocate text section if needed */
	la t0, __relocate_text
	beq t0, zero, .ignore_text_relocation
	nop

	/* copy .text to ram */
	COPY_TO_RAM __etext, __text_start__, __text_end__

.ignore_text_relocation:
	/* copy .data & sdata section */
	COPY_TO_RAM __sdata, __data_start__, __data_end__
	COPY_TO_RAM __ssdata, __sdata_start__, __sdata_end__

	/* zero .bss & sbss section */
	ZERO_RAM __bss_start__, __bss_end__
	ZERO_RAM __sbss_start__, __sbss_end__

	/* Exception vectors */
	INIT_EBASE __vectors
	SET_VECTOR_SPACING 32
	COPY_TO_RAM __svectors, __vectors_start__, __vectors_end__

	/* init CP0 */
	RESET_CP0_CAUSE
	RESET_CP0_STATUS
	ENABLE_FPU64
	ALLOW_DSP_ACCESS

	/* caches */
	RESET_CACHE __text_start__, DEVICE_ICACHE_SIZE, 16, Cache_I
	RESET_CACHE __data_start__, DEVICE_DCACHE_SIZE, 16, Cache_D
	SET_CACHE_COHERENCY CACHE_WBACK_WALLOC

	/* last, clear bev & jump to main() */
	CLEAR_CP0_STATUS_BEV

	la t0, main
	jalr t0
	nop

	.type _exit, %function
	.global _exit
_exit:
	sdbbp
	b _exit
	nop

	.org Reset_Handler + 0x380
BEVException_Handler:
	sdbbp
	b Reset_Handler
	nop

	.section .vectors, "ax"
	.align 4
__vectors:
	.org __vectors + 0x180
	b GeneralException_Handler
	nop

	.align 4
__svectors:
	.word CoreTimer_Handler - __vectors
	.rept DEVICE_INTERRUPT_VECTOR_COUNT - 1
	.word Interrupt_Handler - __vectors
	.endr

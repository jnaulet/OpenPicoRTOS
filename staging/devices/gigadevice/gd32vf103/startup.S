	#include "picoRTOS_device.h"
	#include "../../lib/riscv/macros.S"

	.section .init, "ax"

	.align 4
	.global Reset_Handler
Reset_Handler:
	la sp, __StackTop	/* stack */

	/* relocate text section if needed */
	la t0, __relocate_text
	beq t0, zero, .ignore_text_relocation

	/* copy .text to ram */
	COPY_TO_RAM __etext, __text_start__, __text_end__
	COPY_TO_RAM __vectors, __vectors_start__, __vectors_end__

.ignore_text_relocation:
	INIT_MTEC __vectors_start__, MTEC_ECLIC

	/* copy .data section */
	COPY_TO_RAM __sdata, __data_start__, __data_end__
	/* zero .bss section */
	ZERO_RAM __bss_start__, __bss_end__

	call main

	.type _exit, %function
	.global _exit
_exit:
	ebreak
	j _exit

	.section .vectors
__vectors:
	.word 0
	.word 0
	.word 0
	.word MSIP_Handler
	.word 0
	.word 0
	.word MTIP_Handler
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word BWEI_Handler
	.word PMOVI_Handler
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.word MEIP_Handler
	.endr

	def_irq_handler BWEI_Handler
	def_irq_handler PMOVI_Handler

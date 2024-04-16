	#include "../../include/macros.S"
	#include "picoRTOS_device.h"

	.macro INIT_MTVEC base, flags
	la t0, \base
	ori t0, t0, \flags
	csrw mtvec, t0
	.endm

	.section .init, "ax"

	.align 4
	.global Reset_Handler
Reset_Handler:
	csrci mstatus, 0x8	/* disable interrupts */

	la gp, __gp
	la tp, __tp
	la sp, __StackTop

	/* relocate text section if needed */
	la t0, __relocate_text
	beq t0, zero, .ignore_text_relocation

	/* copy .text to ram */
	COPY_TO_RAM __edata, __text_start__, __text_end__

.ignore_text_relocation:
	/* vectors */
	INIT_MTVEC __vectors_start__, 0x3
	/* copy .data section */
	COPY_TO_RAM __etext, __data_start__, __data_end__
	/* zero .bss section */
	ZERO_RAM __bss_start__, __bss_end__

	call main

	.type _exit, %function
	.global _exit
_exit:
	ebreak
	j _exit

	.section .vectors
	.align 4
__vectors:
	j Reset_Handler
	.word 0
	.word NMI_Handler
	.word EXC_Handler
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word SysTick_Handler
	.word 0
	.word SW_Handler
	.word 0
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.word PlatformDefined_Handler
	.endr

	def_irq_handler NMI_Handler
	def_irq_handler HardFault_Handler
	def_irq_handler SW_Handler

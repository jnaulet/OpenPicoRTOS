	.syntax unified
	.arch armv7-m

	#include "picoRTOS_device.h"
	#include "../../include/macros.S"

	.section .init, "ax"

	.thumb_func
	.type Reset_Handler, %function
	.global Reset_Handler
Reset_Handler:
	/* ignore if __text_end__ < __etext */
	ldr r1, =__relocate_text
	cmp r1, #0
	beq .ignore_text_relocation

	/* copy .text to ram */
	COPY_TO_RAM __edata, __text_start__, __text_end__
	COPY_TO_RAM __vectors, __vectors_start__, __vectors_end__

.ignore_text_relocation:
	/* init vectors & stack */
	INIT_VTOR __vectors_start__
	INIT_STACK __StackTop
	/* copy .data section */
	COPY_TO_RAM __etext, __data_start__, __data_end__
	/* zero .bss section */
	ZERO_RAM __bss_start__, __bss_end__

	/* run main() */
	ldr r1, =main
	blx r1

	.thumb_func
	.type _exit, %function
	.global _exit
_exit:
	bkpt #0
	b _exit

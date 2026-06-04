	.syntax unified
	.arch armv7-m

	#include "picoRTOS_device.h"
	#include "../../include/macros.S"

	.section .init, "ax"

	.thumb_func
	.type Reset_Handler, %function
	.global Reset_Handler
Reset_Handler:
	DISABLE_MPU
	INIT_STACK __StackTop
	INIT_VTOR __vectors_start__

	/* ignore if __text_end__ < __etext */
	ldr r0, =__text_start__
	ldr r1, =__text_load__
	cmp r1, r0
	beq .ignore_text_relocation

	/* copy .text to ram */
	COPY_TO_RAM __einit, __text_start__, __text_end__
	COPY_TO_RAM __vectors, __runtime_vectors_start__, __runtime_vectors_end__
	INIT_VTOR __runtime_vectors_start__

.ignore_text_relocation:
	/* copy .data section */
	COPY_TO_RAM __etext, __data_start__, __data_end__
	/* zero .bss sections */
	ZERO_RAM __bss_start__, __bss_end__
	ZERO_RAM __pbss_start__, __pbss_end__

	/* run main() */
	ldr r1, =main
	blx r1

	.thumb_func
	.type _exit, %function
	.global _exit
_exit:
	bkpt #0
	b _exit

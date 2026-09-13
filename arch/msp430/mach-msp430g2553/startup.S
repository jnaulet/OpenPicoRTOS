	.file "startup.S"
	#include "macros.S"

	// .extern main
	// .extern Reset_Handler
	
	.section .init, "ax"

	.type Reset_Handler, %function
	.global Reset_Handler
Reset_Handler:
	mov #__StackTop, R1		; SP
	/* copy .data section */
	COPY_TO_RAM __etext, __data_start__, __data_end__
	/* zero .bss section */
	ZERO_RAM __bss_start__, __bss_end__
	/* to main() */
	br #main_no_gcc_init

	.section .vectors, "ax"

	.global Vector_Table
Vector_Table:
	.word __vector_0 	; 0
	.word __vector_1 	; 1
	.word __vector_2	; 2
	.word __vector_3	; 3
	.word __vector_4	; 4
	.word __vector_5	; 5
	.word __vector_6	; 6
	.word __vector_7	; 7
	.word __vector_8	; 8
	.word __vector_9	; 9
	.word __vector_10	; 10
	.word __vector_11	; 11
	.word __vector_12	; 12
	.word __vector_13	; 13
	.word __vector_14	; 14
	.word __vector_15	; 15
	.word __vector_16	; 16
	.word __vector_17	; 17
	.word __vector_18	; 18
	.word __vector_19	; 19
	.word __vector_20	; 20
	.word __vector_21	; 21
	.word __vector_22	; 22
	.word __vector_23	; 23
	.word __vector_24	; 24
	.word __vector_25	; 25
	.word SysTick_Handler	; 26
	.word __vector_27	; 27
	.word __vector_28	; 28
	.word __vector_29	; 29
	.word NMI_Handler	; 30
	.word Reset_Handler	; 31

	.section .bss

	.global __StackTop
	.fill 48, 2
__StackTop:

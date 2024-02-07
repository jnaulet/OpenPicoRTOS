	.equ DEVICE_INTERRUPT_VECTOR_COUNT, 18
	.equ INITIAL_IE, 0x80

	.globl _arch_ack_systick

	.macro ACK_SYSTICK
	lcall _arch_ack_systick
	.endm

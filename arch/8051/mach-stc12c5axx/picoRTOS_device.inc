	.equ DEVICE_INTERRUPT_VECTOR_COUNT, 10
	.equ INITIAL_IE, 0x80

	.globl _arch_reload_timer

	.macro ACK_SYSTICK
	lcall _arch_reload_timer
	.endm

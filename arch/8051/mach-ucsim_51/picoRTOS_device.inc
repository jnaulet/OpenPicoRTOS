	.equ DEVICE_INTERRUPT_VECTOR_COUNT, 6
	.equ INITIAL_IE, 0xa0

	.macro ACK_SYSTICK
	anl 0x8f, #0xef
	.endm

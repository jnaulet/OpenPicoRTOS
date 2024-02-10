	.equ DEVICE_INTERRUPT_VECTOR_COUNT, 12
	.equ INITIAL_IE, 0xa0

	.macro ACK_SYSTICK
	anl t2con, #0x7f
	.endm

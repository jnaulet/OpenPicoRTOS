	;; common macros for 8051s

	.macro VECTOR num, handler
	mov _vn, num
	ljmp handler
	.ds 2
	.endm

	;; Generic asm macros for stm8
	;;
	;; Provides:
	;;
	;; COPY_TO_RAM
	;; INIT_STACK
	;; ZERO_RAM
	;; IRQ_HANDLER
	;;

	.macro COPY_TO_RAM base, start, len, ?loop, ?end
	ldw x, len
	jreq end
loop:
	ld a, (base - 1, x)
	ld (start - 1, x), a
	decw x
	jrne loop
end:
	.endm

	.macro INIT_STACK base
	ldw x, base
	ldw sp, x
	.endm

	.macro ZERO_RAM start, len, ?loop, ?end
	ldw x, len
	jreq end
loop:
	clr (start - 1, x)
	decw x
	jrne loop
end:
	.endm

	.macro VECTOR name
        .dw 0x8200
	.dw name
	.endm

	.macro IRQ_HANDLER name, num
	.globl name
name:
	ld a, num
	jp Default_Handler
	.endm

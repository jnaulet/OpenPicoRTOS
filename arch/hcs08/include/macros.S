	;; Generic asm macros for hcs08
	;;
	;; Provides:
	;;
	;; COPY_TO_RAM
	;; INIT_STACK
	;; ZERO_RAM
	;; IRQ_HANDLER
	;;

	.macro COPY_TO_RAM base, start, len, ?loop, ?end
	ldhx #0
loop:
	cphx len
	beq end
	lda base,x
	sta start,x
	aix #1
	bra loop
end:
	.endm

	.macro INIT_STACK base
	ldhx base
	txs
	.endm

	.macro ZERO_RAM start, len, ?loop, ?end
	ldhx #0
	clra
loop:
	cphx len
	beq end
	sta start,x
	aix #1
	bra loop
end:
	.endm

	.macro IRQ_HANDLER name, num
	.globl name
name:
	lda num
	jmp Default_Handler
	.endm

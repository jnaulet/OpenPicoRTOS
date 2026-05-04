	;; common macros for z80s
	;;
	;; Provides:
	;;
	;; COPY_TO_RAM
	;; ZERO_RAM
	;;

	.macro COPY_TO_RAM base, start, len, ?end
	ld hl, #base
	ld de, #start
	ld bc, #len
	ld a, b
	or c
	jr Z, end
	ldir
end:
	.endm

	.macro ZERO_RAM start, len, ?loop, ?next, ?end
	ld hl, #start
	ld bc, #len
loop:
	ld a, b
	or c
	jr Z, end
next:	
	ld (hl), #0
	dec bc
	inc hl
	jr loop
end:
	.endm

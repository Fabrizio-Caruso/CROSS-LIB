	;; BC =  relocation delta
	;; DE -> data (initially _start)
	;; HL -> relocations
reloc:
	ld a,(hl)
	cp 80h			; JR doesn't support testing the sign bit
	jr nc,reloc_large

	; Apply relocation: HL += 1, DE += A, (DE) += BC, DE += 2
reloc_apply:
	inc hl

	add e
	ld e,a
	jr nc,nocarry
	inc d
nocarry:

	ld a,(de)
	add c
	ld (de),a
	inc de
	ld a,(de)
	adc b
	ld (de),a
	inc de
	jr reloc

	; Large jump (> 127 bytes) or end
	; 80h = END, 81-FFh are large jump biased by 8100h in *bigendian* order
reloc_large:
	sub 81h
	jr c,reloc_done
	inc hl
	add d
	ld d,a
	ld a,(hl)
	jr reloc_apply

reloc_done:

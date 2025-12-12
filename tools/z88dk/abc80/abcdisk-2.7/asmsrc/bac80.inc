	org 0			; This code must be position-independent

	defc END=00C9h		; END routine in BASIC ROM
	defc BOFA=65052

	ld bc,END		; END entry point in BASIC (B = 0!)
	push bc			; Save as return address
	ld hl,(BOFA)		; BOFA = start of data
	push hl			; Save for reloc routine
	ld e,l
	ld d,h
	ld c,(hl)		; First BASIC line = BASIC prefix
	add hl,bc		; Skip prefix

	;; Copy the data into place
copy_loop:
	ld a,(hl)		; BASIC line length
	sub 8			; 6 byte header, 2 byte trailer
	jr c,reloc		; END is shorter than any data line

	ld c,6			; BC <- 6 (header length)
	add hl,bc		; Skip header

	; Copy payload data
	ld c,a			; BC <- len since B = 0
	ldir

	; Skip trailer (2 bytes)
	inc hl
	inc hl

	jr copy_loop

	;; BC =  relocation delta
	;; DE -> data (initially _start)
	;; HL -> relocations

setdelta:
	defc DELTA=setdelta+1
	ld bc,0			; Delta for starting address (0 = start at DE)
	ld a,c
	add e
	ld c,a
	ld a,b
	adc d
	ld b,a

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
	jr c,done
	inc hl
	add d
	ld d,a
	ld a,(hl)
	jr reloc_apply

	; All done, jump to entry point
	; Note: top of stack is 00C9h -> END, so a RET from the
	; target routine if the same as END.
done:
	defc DENTRY=done+1
	ld bc,DENTRY		; Offset from final address ptr to entry point
	add hl,bc
	jp (hl)

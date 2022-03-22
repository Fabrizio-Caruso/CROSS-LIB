;;
;; Relocating ABC80 .bac loader loading at BOFA
;;
;; To load a device driver, move BOFA to the desired address
;; after initialization, and use the CHAIN/NEW option.
;;

	include "setup80.inc"

_start:
	include "start80.inc"

	ld de,(BOFA)		; BOFA = start of data
	push de			; Save for done
	push de			; Save for relocation

	include "copy80.inc"

set_reloc_len:
	defc NEG_RELOC_LEN=set_reloc_len+1
	ld hl,0			; Negative (length of relocation data + 1)
	add hl,de

	;;
	;; Move EOFA to the end of our data
	;; This is probably unnecessary but easy to do
	;; The C code will already have inserted a 01h byte to this address
	;;
	ld (EOFA),hl
	inc hl			; HL -> relocations
	pop de			; DE -> data (= BOFA)

	;; Set a delta if needed.  This loader only supports an
	;; 8-bit delta
	ld a,e
setdelta:
	defc DELTA=setdelta+1
	add a,0
	ld c,a
	ld a,d
	adc a,0
	ld b,a

	include "relocate.inc"

	;; All done, jump to entry point
	;; The stack is all set up for return to CHAIN, NEW or END
done:
	defc ENTRY=done+1	; relative to BOFA
	ld bc,0
	pop hl			; HL -> load address (= BOFA)
	add hl,bc
	jp (hl)

	include "end80.inc"

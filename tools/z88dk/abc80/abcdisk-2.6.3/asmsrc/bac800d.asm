;
; .bac device driver (load at BOTTOM) loader for ABC800
;
; RET returns to BASIC, which by default ENDs, see run800.inc.
;

	include "setup800.inc"
	include "run800.inc"

_start:				; HL points here
	push hl
	ld de,reloc_data - _start
	add hl,de
	ex (sp),hl		; On top of stack now: -> reloc_data

set_data_offset:
	defc DATA_OFFSET=set_data_offset+1
	ld de,0
	add hl,de
	;; HL -> input data

	ld de,(BOTM)
	push de

set_data_len:
	defc DATA_LEN=set_data_len+1
	ld bc,0
	ldir

	pop de			; DE -> data to relocate
	ld c,e			; Relocation delta == target address
	ld b,d
	pop hl			; HL -> relocations

	push de			; Save data address

	include "relocate.inc"

done:
	defc ENTRY=done+1
	ld hl,0
	pop de			; DE -> data address
	add hl,de
	jp (hl)			; Jump to entrypoint

	; Relocation data follows immediately afterwards
reloc_data:
_end:

	;; Follows: compacted relocations, program,
	;; trailer: 00 00 00 00 FF FB FF = no long variables

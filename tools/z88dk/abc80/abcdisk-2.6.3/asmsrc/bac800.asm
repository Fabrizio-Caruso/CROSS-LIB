;
; .bac application loader for ABC800
;

	include "setup800.inc"
	include "run800.inc"

	;; Relocator setup code. In order to support saving and
	;; re-loading programs, this must handle being restarted at
	;; either the same or a different address and do the right
	;; thing with the relocations having already been run before.

_start:				; HL points here
	defc NEGDATAPTR=_start+1 ; _start - <start of data>
	;; Subtract the length of the (added) relocations from this value
	;; so that 0 offset = start of program
	ld bc,0			; Current base address
	ld a,l
	sub c
	ld c,a
	ld a,h
	sbc b
	ld b,a
	or c
	jr z,done		; If the relocation delta is zero, done

	push hl
	ld de,reloc_data - _start
	add hl,de
	pop de

	include "relocate.inc"

	; All done, jump to entry point.  A RET will return to BASIC
	; which will execute an END statement.
done:
	defc ENTRY=done+1	; Relocation here
	jp 0			; Entry relative to start of program
LAST_RELOC:			; Relocation data pointer left here

	; Relocation data follows immediately afterwards
reloc_data:
	; Our internal relocations first
	defb NEGDATAPTR - _start
	defb ENTRY - NEGDATAPTR - 2
_end:
	;; Follows: compacted relocations, program,
	;; trailer: 00 00 00 00 FF FB FF = no long variables

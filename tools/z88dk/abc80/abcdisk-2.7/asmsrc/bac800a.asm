;;;
;;; .bac application loader for ABC800 for applications linked at an
;;; absolute address. As this address by necessity will have to be below the
;;; stack and above the heap, this means the application needs to exist
;;; twice in memory, but requires less of the tool chain.
;;;

	include "setup800.inc"
	include "run800.inc"

_start:				; HL points here
	ld de,_end - _start
	add hl,de		; F.C <- 0

	push hl
_ldde:
	defc ADDRESS=_ldde+1
	ld hl,0
	push hl
	ld de,(HEAP)
	sbc hl,de
	ld a,201		; Out of memory
	jp c,SOFTNOCO
	pop de
	pop hl
	
_ldbc:
	defc LENGTH=_ldbc+1
	ld bc,0
	ldir
	defc ENTRY=done+1
done:
	jp 0

_end:
	;; Follows: program,
	;; trailer: 00 00 00 00 FF FB FF = no long variables

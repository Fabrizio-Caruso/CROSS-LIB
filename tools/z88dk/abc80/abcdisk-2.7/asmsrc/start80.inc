;;; -*- asm -*-
;;;
;;; Common initialization code and chaining setup for ABC80. If an
;;; invalid string starting with : is set up as the chained string
;;; (this is set up in the prefix by bin2abc) then call END instead.
;;;
;;; This sets up everything on the stack so that this is not dependent
;;; on the program still being there on exit; a simple RET will do the
;;; right thing here.
;;;

	pop hl			; Return address
	pop de			; P-code position
	inc de			; Skip end of integer statement
	rst 56			; Evaluate P-code expression
	;; This puts the chain filename address on the stack
	pop bc			; Stack delta
	pop hl			; Address of string
	push hl			; Put it back...
	push bc

	ld de,END
	ld a,(hl)		; First character in string
	cp a,':'		; Invalid filename
	jr z,gotretn		; Return to END

	ld de,(CHAINPTR)	; Pointer to the CHAIN routine
	inc de			; Skip RST 56h (eval expression, already done)
gotretn:
	push de			; Return address

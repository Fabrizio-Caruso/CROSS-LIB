;; -*- asm -*-
;;
;; Copy ABC80 encapsulated data into place.
;; This routine explicitly can do overlapping copies as
;; long as they are *downward*.
;;
;; DE -> target location for the data
;;

	ld hl,(BOFA)
	ld b,0
	ld c,(hl)		; Skip the first line
	add hl,bc

copy_loop:
	ld a,(hl)		; BASIC line length
	sub 8			; 6 byte header, 2 byte trailer
	jr c,copy_done		; END/STOP is shorter than any data line

	; Skip length byte + line number (2 bytes) + string opcode (3 bytes)
	ld c,6
	add hl,bc		; Skip header

	; Copy payload data
	ld c,a			; BC <- len since B = 0
	ldir

	; Skip trailer = expression end + CR (2 bytes)
	inc hl
	inc hl

	jr copy_loop

copy_done:

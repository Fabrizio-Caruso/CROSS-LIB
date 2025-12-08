; -------------------------------------------------------------------
; this file is intended to be assembled with ACME.
; It has not been tested with any other assemblers or linkers.
; -------------------------------------------------------------------
; -------------------------------------------------------------------
; example usage of the standard decruncher
; this program decrunches data to memory
; -------------------------------------------------------------------
; if decrunching forwards then the following line must be uncommented.
;DECRUNCH_FORWARDS = 1
!ifndef DECRUNCH_FORWARDS {
DECRUNCH_FORWARDS = 0
}
; -------------------------------------------------------------------
* = $0801
        !byte $0b,$08,<2019,>2019,$9e,$32,$30,$36,$31,0,0,0
; -------------------------------------------------------------------
; we begin here
; -------------------------------------------------------------------
        lda $02
        sta _ebyte_lo
        lda $03
        sta _ebyte_hi

	jsr get_Emerge_byte
	sta _byte_lo
	jsr get_Emerge_byte
	cmp #0
	beq done
	sta _byte_hi

        jsr exod_split_gentable

next_part:
	jsr get_Emerge_byte
	sta _byte_lo
	jsr get_Emerge_byte
	cmp #0
	beq done
	sta _byte_hi

        jsr exod_split_decrunch
	jmp next_part

done:
	rts
; -------------------------------------------------------------------
get_Emerge_byte:
_ebyte_lo = * + 1
_ebyte_hi = * + 2
        lda $ffff
        inc _ebyte_lo
        bne _ebyte_skip_hi
        inc _ebyte_hi
_ebyte_skip_hi:
        rts
; -------------------------------------------------------------------
exod_get_crunched_byte:
!if DECRUNCH_FORWARDS = 0 {
        lda _byte_lo
        bne _byte_skip_hi
        dec _byte_hi
_byte_skip_hi:
        dec _byte_lo
}
_byte_lo = * + 1
_byte_hi = * + 2
        lda $ffff               ; needs to be set correctly before
!if DECRUNCH_FORWARDS != 0 {
        inc _byte_lo
        bne _byte_skip_hi
        inc _byte_hi
_byte_skip_hi:
}
        rts                     ; decrunch_file is called.
; -------------------------------------------------------------------
!source "../exodecrunch.asm"

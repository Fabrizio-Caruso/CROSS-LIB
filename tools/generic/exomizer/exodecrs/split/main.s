; -------------------------------------------------------------------
; this file is intended to be assembled and linked with the cc65 toolchain.
; It has not been tested with any other assemblers or linkers.
; -------------------------------------------------------------------
; -------------------------------------------------------------------
; example usage of the standard decruncher
; this program decrunches data to memory
; -------------------------------------------------------------------
; if decrunching forwards then the following line must be uncommented.
;DECRUNCH_FORWARDS = 1
.IFNDEF DECRUNCH_FORWARDS
DECRUNCH_FORWARDS = 0
.ENDIF
; -------------------------------------------------------------------
.import split_gentable
.import split_decrunch
.export get_crunched_byte

        .byte $01,$08,$0b,$08,<2003,>2003,$9e,'2','0','6','1',0,0,0
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

        jsr split_gentable

next_part:
	jsr get_Emerge_byte
	sta _byte_lo
	jsr get_Emerge_byte
	cmp #0
	beq done
	sta _byte_hi

        jsr split_decrunch
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
get_crunched_byte:
.IF DECRUNCH_FORWARDS = 0
        lda _byte_lo
        bne _byte_skip_hi
        dec _byte_hi
_byte_skip_hi:
        dec _byte_lo
.ENDIF
_byte_lo = * + 1
_byte_hi = * + 2
        lda $ffff               ; needs to be set correctly before
.IF DECRUNCH_FORWARDS <> 0
        inc _byte_lo
        bne _byte_skip_hi
        inc _byte_hi
_byte_skip_hi:
.ENDIF
        rts                     ; decrunch_file is called.
; -------------------------------------------------------------------

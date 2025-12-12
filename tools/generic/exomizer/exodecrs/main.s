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
.import decrunch
.export get_crunched_byte
.import end_of_data

        .byte $01,$08,$0b,$08,<2003,>2003,$9e,'2','0','6','1',0,0,0
; -------------------------------------------------------------------
; we begin here
; -------------------------------------------------------------------
.IF DECRUNCH_FORWARDS = 0
        lda $04
        sta _byte_lo
        lda $05
        sta _byte_hi
.ELSE
        lda $02
        sta _byte_lo
        lda $03
        sta _byte_hi
.ENDIF
        jmp decrunch
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
; end_of_data needs to point to the address just after the address
; of the last byte of crunched data.
; -------------------------------------------------------------------
